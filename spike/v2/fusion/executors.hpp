#pragma once
/** Fusion extension point 2 — the EXECUTOR: where/how a fused group runs.
 *
 * The store hands an executor, per partition:
 *   n      rows,
 *   cols   std::tuple<T*...> — ONE pointer per component type in the group
 *          (nullptr where the partition has no such column),
 *   kernel callable kernel(cols, count) running the fused systems over
 *          rows [0, count) of the given column pointers,
 * plus compile-time Info (Info::kWritten<T>) derived from declared Access.
 *
 * An executor is free to split rows (tiles, threads), move data (DMA to a
 * coprocessor's local memory) or dispatch to another engine, as long as each
 * row is processed exactly once — row-locality (legality L1) makes any
 * split/order of rows produce bit-identical results.
 *
 *   Inline            call the kernel once (the H7 fused loop)
 *   Tiled<N>          fixed-size tiles: the shape of DMA double-buffering,
 *                     cache blocking and work splitting
 *   Parallel          persistent thread pool, contiguous row chunks
 *   Offload<Dev, N>   emulated coprocessor: stage each tile into device-local
 *                     buffers, launch, copy back only WRITTEN columns;
 *                     requires device-safe systems (capability)
 */

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstring>
#include <mutex>
#include <thread>
#include <tuple>
#include <vector>

#include "access.hpp"

namespace spike::fusion
{
    template <typename... Ts>
    std::tuple<Ts*...> offsetCols(const std::tuple<Ts*...>& c, std::size_t b)
    {
        return { (std::get<Ts*>(c) ? std::get<Ts*>(c) + b : nullptr)... };
    }

    /** Compile-time facts about a fused group, handed to executors. */
    template <typename... Systems>
    struct GroupInfo
    {
        template <typename T>
        static constexpr bool kWritten = (kWrites<Systems, T> || ...);
        static constexpr bool kDeviceSafe = (fusion::kDeviceSafe<Systems> && ...);
    };

    // ---------------------------------------------------------------------------
    struct Inline
    {
        static constexpr const char* kName = "Inline";
        static constexpr bool kRequiresDeviceSafe = false;
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel) { kernel(cols, n); }
    };

    template <std::size_t Tile>
    struct Tiled
    {
        static constexpr const char* kName = "Tiled";
        static constexpr bool kRequiresDeviceSafe = false;
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            for (std::size_t b = 0; b < n; b += Tile) kernel(offsetCols(cols, b), std::min(Tile, n - b));
        }
    };

    // ---------------------------------------------------------------------------
    /** Persistent pool; the calling thread takes chunk 0. Small partitions run inline. */
    class Parallel
    {
    public:
        static constexpr const char* kName = "Parallel";
        static constexpr bool kRequiresDeviceSafe = false;
        static constexpr std::size_t kMinRowsPerChunk = 4096;

        explicit Parallel(unsigned threads = std::max(1u, std::thread::hardware_concurrency()))
        {
            for (unsigned i = 1; i < threads; ++i) workers_.emplace_back([this, i] { loop(i); });
        }
        ~Parallel()
        {
            {
                std::lock_guard lk(m_);
                stop_ = true;
                ++gen_;
            }
            cv_.notify_all();
            for (auto& t : workers_) t.join();
        }
        Parallel(const Parallel&) = delete;
        Parallel& operator=(const Parallel&) = delete;

        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            const std::size_t parts = std::min<std::size_t>(workers_.size() + 1u, n / kMinRowsPerChunk);
            if (parts <= 1) { kernel(cols, n); return; }
            Ctx<Cols, K> ctx{ &cols, &kernel };
            {
                std::lock_guard lk(m_);
                job_ = Job{ &thunk<Cols, K>, &ctx, n, (n + parts - 1) / parts, parts };
                remaining_.store(static_cast<int>(parts) - 1);
                ++gen_;
            }
            cv_.notify_all();
            thunk<Cols, K>(&ctx, 0, std::min(n, job_.chunk));
            std::unique_lock lk(m_);
            done_.wait(lk, [&] { return remaining_.load() == 0; });
        }

    private:
        template <typename Cols, typename K>
        struct Ctx { const Cols* cols; K* kernel; };

        template <typename Cols, typename K>
#if defined(__GNUC__)
        __attribute__((flatten))
#endif
        static void thunk(void* p, std::size_t b, std::size_t e)
        {
            auto* c = static_cast<Ctx<Cols, K>*>(p);
            (*c->kernel)(offsetCols(*c->cols, b), e - b);
        }

        struct Job
        {
            void (*fn)(void*, std::size_t, std::size_t) = nullptr;
            void* ctx = nullptr;
            std::size_t n = 0, chunk = 0, parts = 0;
        };

        void loop(unsigned index)
        {
            std::uint64_t seen = 0;
            for (;;)
            {
                Job job;
                {
                    std::unique_lock lk(m_);
                    cv_.wait(lk, [&] { return gen_ != seen; });
                    seen = gen_;
                    if (stop_) return;
                    job = job_;
                }
                if (index < job.parts)
                {
                    const std::size_t b = index * job.chunk;
                    job.fn(job.ctx, b, std::min(job.n, b + job.chunk));
                    if (remaining_.fetch_sub(1) == 1)
                    {
                        std::lock_guard lk(m_);
                        done_.notify_one();
                    }
                }
            }
        }

        std::vector<std::thread> workers_;
        std::mutex m_;
        std::condition_variable cv_, done_;
        std::uint64_t gen_ = 0;
        bool stop_ = false;
        Job job_;
        std::atomic<int> remaining_{ 0 };
    };

    // ---------------------------------------------------------------------------
    /** Device model: how a staged tile is launched. A real backend (ESP32-P4 LP
     *  core, a DSP, a GPU queue) implements launch() with its own API and a
     *  device build of the kernel; this host emulation just calls it. */
    struct EmulatedDevice
    {
        static constexpr const char* kName = "EmulatedDevice";
        template <typename F>
        void launch(F&& f) { f(); }
    };

    template <typename Device, std::size_t Tile>
    class Offload
    {
    public:
        static constexpr const char* kName = "Offload";
        static constexpr bool kRequiresDeviceSafe = true;

        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            static_assert(Info::kDeviceSafe, "planner must not route host-only systems to a device");
            for (std::size_t b = 0; b < n; b += Tile)
            {
                const std::size_t count = std::min(Tile, n - b);
                const Cols staged = stageIn<Info>(cols, b, count);        // DMA host -> device-local
                device_.launch([&] { kernel(staged, count); });
                stageOut<Info>(cols, staged, b, count);                   // DMA back: written columns only
            }
        }

        std::size_t bytesIn() const { return bytesIn_; }
        std::size_t bytesOut() const { return bytesOut_; }

    private:
        template <typename Info, typename... Ts>
        std::tuple<Ts*...> stageIn(const std::tuple<Ts*...>& cols, std::size_t b, std::size_t count)
        {
            buffers_.resize(sizeof...(Ts));
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                return std::tuple<Ts*...>{ stageOne<Ts>(std::get<Ts*>(cols), b, count, I)... };
            }(std::index_sequence_for<Ts...>{});
        }

        template <typename T>
        T* stageOne(T* src, std::size_t b, std::size_t count, std::size_t slot)
        {
            if (!src) return nullptr;
            auto& buf = buffers_[slot];
            buf.resize(Tile * sizeof(T));
            std::memcpy(buf.data(), src + b, count * sizeof(T));
            bytesIn_ += count * sizeof(T);
            return reinterpret_cast<T*>(buf.data());
        }

        template <typename Info, typename... Ts>
        void stageOut(const std::tuple<Ts*...>& cols, const std::tuple<Ts*...>& staged, std::size_t b, std::size_t count)
        {
            ((Info::template kWritten<Ts> && std::get<Ts*>(cols)
                  ? (std::memcpy(std::get<Ts*>(cols) + b, std::get<Ts*>(staged), count * sizeof(Ts)),
                     bytesOut_ += count * sizeof(Ts), 0)
                  : 0),
             ...);
        }

        Device device_{};
        std::vector<std::vector<std::byte>> buffers_;
        std::size_t bytesIn_ = 0, bytesOut_ = 0;
    };

} // namespace spike::fusion
