/** Skirmish determinism: every storage design (and fused vs sequential
 * execution) must play the *identical* game — same checksum at every
 * checkpoint. This is the lockstep-RTS requirement, and the guard that makes
 * the benchmark comparison meaningful.
 */
#include <memory>
#include <string>
#include <vector>

#include "../bench/skirmish/sim.hpp"
#include "../bench/skirmish/runners.hpp"
#include "../bench/skirmish/worlds.hpp"
#include <doctest/doctest.h>

using namespace skirmish;

namespace
{
    constexpr int kTicks = 360;
    constexpr int kEvery = 60;

    struct Trace
    {
        std::vector<double> checksums;
        Stats final;
    };

    template <typename W, bool Fused, typename Runner = void>
    Trace play(Config cfg)
    {
        auto world = std::make_unique<W>();
        Sim<W, Fused, Runner> sim(*world, cfg);
        Trace tr;
        for (int t = 1; t <= kTicks; ++t)
        {
            sim.tick();
            if (t % kEvery == 0) tr.checksums.push_back(sim.checksum());
        }
        tr.final = sim.stats();
        return tr;
    }

    /** First checkpoint tick where tr diverges from ref, or 0 if the traces are identical. */
    int divergence(const Trace& ref, const Trace& tr)
    {
        for (std::size_t i = 0; i < tr.checksums.size() && i < ref.checksums.size(); ++i)
            if (tr.checksums[i] != ref.checksums[i]) return static_cast<int>((i + 1) * kEvery);
        return 0;
    }

    void compare(const std::string& name, const Trace& ref, const Trace& tr)
    {
        CAPTURE(name);
        CHECK_MESSAGE(divergence(ref, tr) == 0, "first divergence at tick ", divergence(ref, tr));
        CHECK(tr.checksums == ref.checksums);
        CHECK(tr.final.entities == ref.final.entities);
        CHECK(tr.final.projectiles == ref.final.projectiles);
        CHECK(tr.final.units == ref.final.units);
        CHECK(tr.final.resources == ref.final.resources);
        CHECK(tr.final.kills == ref.final.kills);
    }

    Config config()
    {
        Config cfg;
        cfg.unitsPerTeam = 300;
        return cfg;
    }

    const Trace& reference()
    {
        static const Trace ref = play<SparseSetWorld, false>(config());
        return ref;
    }
} // namespace

TEST_CASE("Skirmish: the reference game exercises combat, projectiles and economy")
{
    const Stats& s = reference().final;
    CHECK(s.kills[0] + s.kills[1] + s.kills[2] + s.kills[3] > 0);
    CHECK(s.projectiles > 0);
    CHECK(s.resources[0] != 500);
}

TEST_CASE("Skirmish: every storage design plays the identical game")
{
    const Config cfg = config();
    const Trace& ref = reference();
    compare("Archetype", ref, play<ArchetypeWorld, false>(cfg));
    compare("SortedSoA", ref, play<SortedWorld, false>(cfg));
    compare("StaticBitmask", ref, play<StaticWorld<(1 << 13)>, false>(cfg));
    compare("QueryPart", ref, play<QueryPartWorld, false>(cfg));
    compare("QueryPart (fused)", ref, play<QueryPartWorld, true>(cfg));
    compare("QPartHinted", ref, play<QPartHintedWorld, false>(cfg));
    compare("QPartHinted (fused)", ref, play<QPartHintedWorld, true>(cfg));
}

TEST_CASE("Skirmish: planners and executors play the identical game")
{
    const Config cfg = config();
    const Trace& ref = reference();
    compare("planner NeverFuse", ref, play<QPartHintedWorld, false, PlannedRunner<fz::NeverFuse>>(cfg));
    compare("ShareColumns+Parallel", ref, play<QPartHintedWorld, false, PlannedRunner<fz::ShareColumns, fz::Parallel>>(cfg));
    compare("DeviceAware+Offload", ref, play<QPartHintedWorld, false, OffloadRunner<>>(cfg));
    compare("AutoTuned", ref, play<QPartHintedWorld, false, AutoTunedRunner>(cfg));
}

TEST_CASE("Skirmish: lockstep with data-parallel systems and per-worker command buffers")
{
    const Trace& ref = reference();
    for (unsigned threads : { 2u, 4u })
    {
        fz::Parallel pool(threads);
        Config t = config();
        t.pool = &pool;
        t.fuseMovement = false;
        compare("Lockstep x" + std::to_string(threads), ref, play<QPartHintedWorld, false>(t));
        t.fuseMovement = true;
        compare("Lockstep+fused x" + std::to_string(threads), ref, play<QPartHintedWorld, false>(t));
    }
}
