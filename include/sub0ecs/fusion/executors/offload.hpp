#pragma once
/** Offload<Device, N>: emulated coprocessor — stage each tile into
 *  device-local buffers, launch, copy back only WRITTEN columns. */

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <tuple>
#include <utility>
#include <vector>

#include "sub0ecs/fusion/executors/contract.hpp"

namespace sub0ecs::fusion
{
    /** Device model: how a staged tile is launched. A real backend (ESP32-P4 LP
     *  core, a DSP, a GPU queue) implements launch() with its own API and a
     *  device build of the kernel; this host emulation just calls it. */
    struct EmulatedDevice
    {
        static constexpr const char* kName = "EmulatedDevice";
        /** Runs a staged tile.
         *  @param f The work to run on the device; here it is called at once. */
        template <typename F>
        void launch(F&& f) { f(); }
    };

    /** Stages each tile of a device-safe group into device-local buffers, launches
     *  it, and copies back only the columns the group writes.
     *  @tparam Device A type with launch(f), such as EmulatedDevice.
     *  @tparam Tile   Rows per tile; at least 1. */
    template <typename Device, std::size_t Tile>
    class Offload
    {
        static_assert(Tile > 0, "Offload<Device, 0> would never advance");

    public:
        static constexpr const char* kName = "Offload";
        static constexpr bool kRequiresDeviceSafe = true;

        /** Runs a fused group's kernel over a partition's rows.
         *  @tparam Info  GroupInfo of the group.
         *  @param n      The number of rows.
         *  @param cols   One column pointer per component type of the group (null
         *                where the partition has no such column).
         *  @param kernel Called as kernel(cols, count) over rows [0, count). */
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

        /** Reports the traffic to the device so far.
         *  @return Bytes staged in. */
        [[nodiscard]] std::size_t bytesIn() const { return bytesIn_; }

        /** Reports the traffic from the device so far.
         *  @return Bytes copied back. */
        [[nodiscard]] std::size_t bytesOut() const { return bytesOut_; }

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

} // namespace sub0ecs::fusion
