#pragma once
/** Parallel: persistent spin-then-park thread pool running contiguous row
 *  chunks (H8; see research/threading.md). */

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#if defined(__SSE2__) || defined(_M_X64)
#    include <immintrin.h>
#endif

#include "contract.hpp"

namespace spike::fusion
{
    /** Persistent pool; the calling thread takes chunk 0. Small partitions run inline. */
    class Parallel
    {
    public:
        static constexpr const char* kName = "Parallel";
        static constexpr bool kRequiresDeviceSafe = false;
        static constexpr std::size_t kMinRowsPerChunk = 4096;

        /** affinity = true: static "owner computes" scheduling — worker w always
         *  gets the same contiguous block of items, so data a worker wrote in one
         *  system is still in its cache for the next (vs dynamic load balancing). */
        explicit Parallel(unsigned threads = std::max(1u, std::thread::hardware_concurrency()), bool affinity = false)
            : affinity_(affinity)
        {
            for (unsigned i = 1; i < threads; ++i) workers_.emplace_back([this, i] { loop(i); });
        }
        ~Parallel()
        {
            stop_.store(true, std::memory_order_release);
            gen_.fetch_add(1, std::memory_order_release);
            {
                std::lock_guard lk(m_);
            }
            cv_.notify_all();
            for (auto& t : workers_) t.join();
        }
        Parallel(const Parallel&) = delete;
        Parallel& operator=(const Parallel&) = delete;

        /** Threads taking part in a dispatch (workers + the calling thread). */
        unsigned concurrency() const { return static_cast<unsigned>(workers_.size()) + 1u; }

        /** fn(item, worker) for every item in [0, items), dynamically claimed
         *  (atomic counter) for load balance; the caller runs as worker 0.
         *  Items are independent (row-local), so claim order never affects results. */
        template <typename F>
        void parallelFor(std::size_t items, F& fn)
        {
            if (items == 0) return;
            if (items == 1 || workers_.empty())
            {
                for (std::size_t i = 0; i < items; ++i) fn(i, 0u);
                return;
            }
            ForCtx<F> ctx{ &fn, items, {}, affinity_ ? concurrency() : 0u };
            dispatch(&forThunk<F>, &ctx);
        }

        /** Fused-group executor contract: contiguous row chunks of one partition. */
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
#if defined(__GNUC__)
        __attribute__((flatten))
#elif defined(_MSC_VER)
        [[msvc::flatten]]
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

        /** Publish a job to all workers, run it on this thread too, wait for all.
         *  Workers spin briefly between jobs (a tick issues many back-to-back
         *  dispatches) and only then block, so hot dispatches avoid the
         *  condition-variable wake-up latency. */
        void dispatch(void (*fn)(void*, unsigned), void* ctx)
        {
            job_ = Job{ fn, ctx };
            remaining_.store(static_cast<int>(workers_.size()), std::memory_order_relaxed);
            gen_.fetch_add(1, std::memory_order_release);
            {
                std::lock_guard lk(m_);   // pairs with sleepers' predicate check (no lost wake-up)
            }
            cv_.notify_all();
            fn(ctx, 0u);
            for (int i = 0; i < kSpin && remaining_.load(std::memory_order_acquire) != 0; ++i) relax();
            if (remaining_.load(std::memory_order_acquire) != 0)
            {
                std::unique_lock lk(m_);
                done_.wait(lk, [&] { return remaining_.load(std::memory_order_acquire) == 0; });
            }
        }

        void loop(unsigned index)
        {
            std::uint64_t seen = 0;
            for (;;)
            {
                for (int i = 0; i < kSpin && gen_.load(std::memory_order_acquire) == seen; ++i) relax();
                if (gen_.load(std::memory_order_acquire) == seen)
                {
                    std::unique_lock lk(m_);
                    cv_.wait(lk, [&] { return gen_.load(std::memory_order_acquire) != seen; });
                }
                seen = gen_.load(std::memory_order_acquire);
                if (stop_.load(std::memory_order_acquire)) return;
                const Job job = job_;
                job.fn(job.ctx, index);
                if (remaining_.fetch_sub(1, std::memory_order_acq_rel) == 1)
                {
                    std::lock_guard lk(m_);
                    done_.notify_one();
                }
            }
        }

        static constexpr int kSpin = 4000;

        bool affinity_ = false;
        std::vector<std::thread> workers_;
        std::mutex m_;
        std::condition_variable cv_, done_;
        std::atomic<std::uint64_t> gen_{ 0 };
        std::atomic<bool> stop_{ false };
        Job job_;
        std::atomic<int> remaining_{ 0 };
    };

} // namespace spike::fusion
