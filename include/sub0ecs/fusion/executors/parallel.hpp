#pragma once
/** Parallel: a spin-then-park thread pool running contiguous row chunks (see
 *  docs/research/threading.md).
 *
 *  - Sized to the machine it finds: by default one thread per performance core
 *    the process may use, not one per logical CPU. Efficiency cores and
 *    oversubscription both make data-parallel work slower, not faster. Threads
 *    are not pinned unless asked (Options::pinToPerformanceCores): with the
 *    right count the OS already places them on the performance cores, and on
 *    the machine this was measured on an affinity mask was the same or slower.
 *  - Grows on demand: no thread exists until a dispatch needs it. Work that is
 *    only ever four ways parallel starts three workers, whatever the limit.
 *  - Wakes only what a dispatch uses: each worker has its own signal, so a
 *    narrow dispatch on a wide pool leaves the other workers parked.
 *
 *  Parking uses a mutex and condition variable per worker, not C++20 atomic
 *  waiting: libstdc++ without futexes (MinGW) implements the latter with a shared
 *  waiter pool, and the pool ran several times slower than one thread on it. */

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#if defined(__SSE2__) || defined(_M_X64)
#    include <immintrin.h>
#endif

#include "sub0ecs/fusion/executors/contract.hpp"
#include "sub0ecs/fusion/executors/cpu_topology.hpp"

namespace sub0ecs::fusion
{
    /** Persistent pool; the calling thread takes part as worker 0. Small partitions run inline.
     *  One thread dispatches at a time: a pool is not safe to use from two threads at once. */
    class Parallel
    {
    public:
        static constexpr const char* kName = "Parallel";
        static constexpr bool kRequiresDeviceSafe = false;
        static constexpr std::size_t kMinRowsPerChunk = 4096;

        struct Options
        {
            /** The most threads a dispatch may use, counting the caller. 0 means one per
             *  performance core available to the process. An explicit count is taken as
             *  given, even beyond the CPUs available. */
            unsigned threads = 0;
            /** Static "owner computes" scheduling: worker w always gets the same contiguous
             *  block of items, so data a worker wrote in one system is still in its cache
             *  for the next. Off: items are claimed dynamically, for load balance. */
            bool ownerComputes = false;
            /** Restrict the workers that fit on the performance cores to those cores (the
             *  caller is assumed to hold one; workers beyond them are left to the OS). A
             *  hint: it does nothing where the CPU is not hybrid or threads cannot be
             *  restricted. Off by default, because the OS usually gets this right by
             *  itself; turn it on when measurement on the target says it helps. */
            bool pinToPerformanceCores = false;
        };

        /** Creates a pool. No thread is started until a dispatch needs one.
         *  @param options Size and scheduling choices. */
        explicit Parallel(const Options& options)
            : options_(options), limit_(options.threads != 0 ? options.threads : std::max(1u, cpuTopology().performance)),
              workers_(limit_ > 1 ? std::make_unique<Worker[]>(limit_) : nullptr)
        {
        }

        /** Creates a pool; shorthand for Options{ threads, ownerComputes }.
         *  @param threads       See Options::threads.
         *  @param ownerComputes See Options::ownerComputes. */
        explicit Parallel(unsigned threads = 0, bool ownerComputes = false) : Parallel(Options{ threads, ownerComputes, false }) {}
        ~Parallel()
        {
            stop_.store(true, std::memory_order_release);
            for (unsigned i = 1; i <= started_; ++i) wake(workers_[i], true);
            for (unsigned i = 1; i <= started_; ++i) workers_[i].thread.join();
        }
        Parallel(const Parallel&) = delete;
        Parallel& operator=(const Parallel&) = delete;

        /** Reports the most threads a dispatch uses, counting the calling thread.
         *  @return The bound on the worker index passed to callbacks; fixed for the
         *          pool's lifetime. */
        [[nodiscard]] unsigned concurrency() const { return limit_; }

        /** Reports the worker threads created so far (the caller is not one of them).
         *  @return A count that starts at zero and grows to the widest dispatch seen,
         *          never beyond concurrency() - 1. */
        [[nodiscard]] unsigned threadsStarted() const { return started_; }

        /** Runs fn for every item, on up to concurrency() threads, and waits for all.
         *  Items are claimed dynamically for load balance unless the pool was made
         *  with ownerComputes; they must be independent, so the order never matters.
         *  @param items The number of items; 0 does nothing, 1 runs on the caller.
         *  @param fn    Called as fn(item, worker) once per item in [0, items), with
         *               worker < concurrency(); the caller takes part as worker 0.
         *  @note One thread dispatches at a time: do not call from two threads or
         *        from inside fn. */
        template <typename F>
        void parallelFor(std::size_t items, F& fn)
        {
            if (items == 0) return;
            const unsigned participants = static_cast<unsigned>(std::min<std::size_t>(items, limit_));
            if (participants == 1)
            {
                for (std::size_t i = 0; i < items; ++i) fn(i, 0u);
                return;
            }
            ForCtx<F> ctx{ &fn, items, {}, options_.ownerComputes ? participants : 0u };
            dispatch(participants, &forThunk<F>, &ctx);
        }

        /** Runs a fused group's kernel over contiguous row chunks of one partition,
         *  one chunk per thread; a partition under two chunks' worth of rows runs inline.
         *  @tparam Info  GroupInfo of the group.
         *  @param n      The number of rows.
         *  @param cols   One column pointer per component type of the group.
         *  @param kernel Called as kernel(cols, count), concurrently on disjoint rows. */
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            const std::size_t parts = std::min<std::size_t>(concurrency(), n / kMinRowsPerChunk);
            if (parts <= 1) { kernel(cols, n); return; }
            const std::size_t chunk = (n + parts - 1) / parts;
            auto body = [&](std::size_t item, unsigned) {
                const std::size_t b = item * chunk;
                kernel(offsetCols(cols, b), std::min(n, b + chunk) - b);
            };
            parallelFor(parts, body);
        }

    private:
        template <typename F>
        struct ForCtx
        {
            F* fn;
            std::size_t items;
            std::atomic<std::size_t> next;
            unsigned blocks;   // 0 = dynamic claiming; else static contiguous blocks per worker
        };

        template <typename F>
        // Inline the item callback into the claim loop (GCC and Clang).
#if defined(__GNUC__)
        __attribute__((flatten))
#endif
        static void forThunk(void* p, unsigned worker)
        {
            auto* c = static_cast<ForCtx<F>*>(p);
            if (c->blocks)
            {
                const std::size_t b = c->items * worker / c->blocks, e = c->items * (worker + 1) / c->blocks;
                for (std::size_t i = b; i < e; ++i) (*c->fn)(i, worker);
                return;
            }
            for (std::size_t i; (i = c->next.fetch_add(1, std::memory_order_relaxed)) < c->items;) (*c->fn)(i, worker);
        }

        struct Job
        {
            void (*fn)(void*, unsigned) = nullptr;
            void* ctx = nullptr;
        };

        static void relax()
        {
#if defined(__SSE2__) || defined(_M_X64)
            _mm_pause();
#else
            std::this_thread::yield();
#endif
        }

        /** One slot per possible thread (slot 0, the caller's, is unused). Each worker
         *  waits on its own signal, so a dispatch wakes exactly the workers it uses.
         *  One cache line each, so workers do not share a line through their signals. */
#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4324)   // padded due to alignment: intended
#endif
        struct alignas(64) Worker
        {
            std::atomic<std::uint64_t> signal{ 0 };   // bumped once per job this worker takes part in
            std::atomic<bool> parked{ false };        // blocked on `wakeUp`, so it needs a notify
            std::mutex mutex;
            std::condition_variable wakeUp;
            std::thread thread;
        };
#if defined(_MSC_VER)
#    pragma warning(pop)
#endif

        /** Starts workers until `count` exist. Called by the dispatching thread only. */
        void grow(unsigned count)
        {
            while (started_ < count)
            {
                const unsigned index = ++started_;
                workers_[index].thread = std::thread([this, index] {
                    if (options_.pinToPerformanceCores && index < cpuTopology().performance) keepThisThreadOnPerformanceCores();
                    loop(index);
                });
            }
        }

        /** Signals one worker. A worker still spinning sees the signal by itself; only a
         *  parked one is notified. seq_cst on both sides: either the worker sees the new
         *  signal before it blocks, or this sees it parked. The empty lock closes the gap
         *  between the worker's last check and its wait. */
        static void wake(Worker& worker, bool always = false)
        {
            worker.signal.fetch_add(1, std::memory_order_seq_cst);
            if (always || worker.parked.load(std::memory_order_seq_cst))
            {
                {
                    std::lock_guard lock(worker.mutex);
                }
                worker.wakeUp.notify_one();
            }
        }

        /** Publish a job to the first `participants - 1` workers, run it on this thread
         *  too, wait for all. Workers spin briefly between jobs (a tick issues many
         *  back-to-back dispatches) and only then block, so hot dispatches avoid the
         *  wake-up latency; a worker that is still spinning is not notified. */
        void dispatch(unsigned participants, void (*fn)(void*, unsigned), void* ctx)
        {
            grow(participants - 1u);
            job_ = Job{ fn, ctx };
            remaining_.store(static_cast<int>(participants - 1u), std::memory_order_relaxed);
            for (unsigned i = 1; i < participants; ++i) wake(workers_[i]);
            fn(ctx, 0u);
            for (int i = 0; i < kSpin && remaining_.load(std::memory_order_acquire) != 0; ++i) relax();
            if (remaining_.load(std::memory_order_acquire) != 0)
            {
                std::unique_lock lock(doneMutex_);
                done_.wait(lock, [&] { return remaining_.load(std::memory_order_acquire) == 0; });
            }
        }

        void loop(unsigned index)
        {
            Worker& self = workers_[index];
            std::uint64_t seen = 0;
            for (;;)
            {
                for (int i = 0; i < kSpin && self.signal.load(std::memory_order_acquire) == seen; ++i) relax();
                if (self.signal.load(std::memory_order_acquire) == seen)
                {
                    self.parked.store(true, std::memory_order_seq_cst);
                    {
                        std::unique_lock lock(self.mutex);
                        self.wakeUp.wait(lock, [&] { return self.signal.load(std::memory_order_seq_cst) != seen; });
                    }
                    self.parked.store(false, std::memory_order_seq_cst);
                }
                seen = self.signal.load(std::memory_order_acquire);
                if (stop_.load(std::memory_order_acquire)) return;
                const Job job = job_;
                job.fn(job.ctx, index);
                if (remaining_.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    std::lock_guard lock(doneMutex_);   // pairs with the dispatcher's predicate check
                    done_.notify_one();
                }
            }
        }

        static constexpr int kSpin = 4000;

        Options options_;
        unsigned limit_ = 1;
        unsigned started_ = 0;               // worker threads created; slots 1..started_ are live
        std::unique_ptr<Worker[]> workers_;   // limit_ slots, fixed: workers never move
        std::atomic<bool> stop_{ false };
        Job job_;
        std::atomic<int> remaining_{ 0 };
        std::mutex doneMutex_;
        std::condition_variable done_;
    };

} // namespace sub0ecs::fusion
