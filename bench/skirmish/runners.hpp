#pragma once
/** Movement runners for Skirmish: fusion planner x executor combinations
 *  (fusion/planner.hpp, fusion/executors.hpp). Query-partition worlds only. */

#include "sub0ecs/fusion/executors.hpp"
#include "sub0ecs/fusion/planner.hpp"

namespace skirmish
{
    namespace fz = sub0ecs::fusion;

    /** Compile-time planner on a host executor. */
    template <typename Planner, typename Exec = fz::Inline>
    struct PlannedRunner
    {
        Exec exec{};
        fz::Inline host{};
        template <typename W, typename... S>
        void run(W& w, const S&... s) { fz::runPlanned<Planner>(w, exec, host, s...); }
    };

    /** Capability-aware offload: device-safe groups go to the (emulated)
     *  coprocessor tile by tile; Separation (host Grid pointer) stays on the host. */
    template <std::size_t Tile = 1024>
    struct OffloadRunner
    {
        fz::Offload<fz::EmulatedDevice, Tile> exec{};
        fz::Inline host{};
        template <typename W, typename... S>
        void run(W& w, const S&... s) { fz::runPlanned<fz::DeviceAware<fz::ShareColumns>>(w, exec, host, s...); }
    };

    /** Runtime decision: measure candidate plans in the live game, keep the fastest. */
    struct AutoTunedRunner
    {
        fz::AutoTuner<fz::NeverFuse, fz::ShareColumns, fz::DeviceAware<fz::ShareColumns>> tuner{};
        fz::Inline exec{};
        template <typename W, typename... S>
        void run(W& w, const S&... s) { tuner.run(w, exec, exec, s...); }
    };
} // namespace skirmish
