/** Fusion extension points: planners decide at compile time (checked with
 * static_assert), executors/planners/autotuner must all be bit-identical to
 * sequential execution, and offload must move only what Access requires.
 */
#include <memory>
#include <string>

#include <sub0ecs/fusion/executors.hpp>
#include <sub0ecs/fusion/planner.hpp>

#include "../bench/common/scenarios.hpp"
#include "../bench/common/systems.hpp"
#include "../bench/designs/query_partition.hpp"
#include "../bench/designs/sparse_set.hpp"
#include "../bench/skirmish/sim.hpp"
#include "doctest.h"

using namespace bench;
namespace fz = sub0ecs::fusion;

// ---- compile-time planning decisions ---------------------------------------
using FF = fz::Plan<fz::ShareColumns, Integrate, Forces, Wrap, RotHealthSys>;
static_assert(FF::kGroups == 2, "FusionFrame: {Integrate, Forces, Wrap} + {RotHealth}");
static_assert(FF::groupOf[0] == 0 && FF::groupOf[1] == 0 && FF::groupOf[2] == 0 && FF::groupOf[3] == 1);

using F3 = fz::Plan<fz::ShareColumns, PhysicsSys, RotHealthSys, PulseSys>;
static_assert(F3::kGroups == 3, "Frame3 shares no columns: never fused (rule G2)");

static_assert(fz::Plan<fz::NeverFuse, Integrate, Forces, Wrap>::kGroups == 3);
static_assert(fz::Plan<fz::AlwaysFuse, PhysicsSys, RotHealthSys, PulseSys>::kGroups == 1);

using SM = fz::Plan<fz::DeviceAware<fz::ShareColumns>, skirmish::Seek, skirmish::Separation, skirmish::StunFreeze,
                    skirmish::Integrate, skirmish::Friction, skirmish::Bounds>;
static_assert(fz::Plan<fz::ShareColumns, skirmish::Seek, skirmish::Separation, skirmish::StunFreeze,
                       skirmish::Integrate, skirmish::Friction, skirmish::Bounds>::kGroups == 1,
              "Skirmish movement all shares Position/Velocity: one group");
static_assert(SM::kGroups == 3 && SM::groupOf[0] == 0 && SM::groupOf[1] == 1 && SM::groupOf[2] == 2 &&
                  SM::groupOf[5] == 2,
              "DeviceAware isolates host-only Separation: Seek | Separation | StunFreeze..Bounds");

static_assert(fz::kWrites<Integrate, Position> && !fz::kWrites<Integrate, Velocity>);
static_assert(fz::GroupInfo<Integrate, Forces>::kWritten<Velocity>);
static_assert(!fz::kDeviceSafe<skirmish::Separation> && fz::kDeviceSafe<skirmish::Seek>);

// ---- runtime equivalence ------------------------------------------------------
namespace
{
    void check(bool ok, const std::string& what) { CHECK_MESSAGE(ok, what); }

    using W = qpart::HintedWorld;
    constexpr std::int64_t kN = 20'000;   // > Parallel::kMinRowsPerChunk so threads really split

    template <typename Frame>
    double play(Pattern pattern, Frame frame)
    {
        auto w = std::make_unique<W>();
        auto es = populate(*w, kN, pattern);
        tagEveryHundredth(*w, es);
        for (int i = 0; i < 4; ++i) frame(*w);
        for (std::size_t i = 0; i < es.size(); i += 4)   // structural change between frames
            if (w->template find<Scale>(es[i]) && !w->template find<Color>(es[i])) w->add(es[i], Color{});
        for (int i = 0; i < 3; ++i) frame(*w);
        return checksum(*w, es);
    }

    template <typename Planner, typename Exec>
    double planned(Pattern pattern, Exec& exec)
    {
        fz::Inline host;
        return play(pattern, [&](W& w) {
            fz::runPlanned<Planner>(w, exec, host, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
            fz::runPlanned<Planner>(w, exec, host, PhysicsSys{}, RotHealthSys{}, PulseSys{});
        });
    }
} // namespace

TEST_CASE("every planner x executor combination equals sequential execution")
{
    for (Pattern pattern : { Pattern::Coherent, Pattern::Fragmented })
    {
        const std::string tag = toString(pattern);
        const double ref = play(pattern, [](W& w) {
            runSequential(w, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
            runSequential(w, PhysicsSys{}, RotHealthSys{}, PulseSys{});
        });

        fz::Inline inl;
        fz::Tiled<1000> tiled;
        fz::Parallel pool(4);
        fz::Offload<fz::EmulatedDevice, 1024> off;

        check(planned<fz::NeverFuse>(pattern, inl) == ref, tag + ": NeverFuse/Inline");
        check(planned<fz::AlwaysFuse>(pattern, inl) == ref, tag + ": AlwaysFuse/Inline");
        check(planned<fz::ShareColumns>(pattern, inl) == ref, tag + ": ShareColumns/Inline");
        check(planned<fz::ShareColumns>(pattern, tiled) == ref, tag + ": ShareColumns/Tiled");
        check(planned<fz::ShareColumns>(pattern, pool) == ref, tag + ": ShareColumns/Parallel");
        check(planned<fz::AlwaysFuse>(pattern, pool) == ref, tag + ": AlwaysFuse/Parallel");
        check(planned<fz::DeviceAware<fz::ShareColumns>>(pattern, off) == ref, tag + ": DeviceAware/Offload");

        fz::AutoTuner<fz::NeverFuse, fz::ShareColumns, fz::AlwaysFuse> tuner;
        const double tuned = play(pattern, [&](W& w) {
            tuner.run(w, inl, inl, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
        });
        const double tunedRef = play(pattern, [](W& w) { runSequential(w, Integrate{}, Forces{}, Wrap{}, RotHealthSys{}); });
        check(tuned == tunedRef, tag + ": AutoTuner (switching plans mid-run) == sequential");
    }
}

TEST_CASE("Offload moves only what Access requires")
{
    // Integrate writes Position and reads Velocity.
    {
        auto w = std::make_unique<W>();
        populate(*w, 10'000, Pattern::Coherent);
        fz::Offload<fz::EmulatedDevice, 1024> off;
        fz::Inline host;
        fz::runPlanned<fz::ShareColumns>(*w, off, host, Integrate{});
        check(off.bytesIn() == 10'000 * (sizeof(Position) + sizeof(Velocity)), "offload stages Position+Velocity in");
        check(off.bytesOut() == 10'000 * sizeof(Position), "offload copies back only the written Position column");
    }
}
