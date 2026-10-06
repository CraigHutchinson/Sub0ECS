/** Skirmish benchmark: time per simulation tick, per storage design, with a
 * per-system breakdown (us_<system> counters, microseconds per tick).
 *
 * Cases: Tick/-/<Design>/<unitsPerTeam> and Threads/<Unfused|Fused>/t<threads>[+affinity]/<unitsPerTeam>.
 * Each case warms its game up for kWarmup ticks (armies engaged, projectiles in
 * flight) before timing; one operation is one tick of the evolving game. A group's
 * designs are compared paired at one tick per epoch with no calibration, so every
 * design plays exactly the same ticks (warmup + rounds) and ends in the same state.
 */
#include <memory>
#include <string>

#include "../common/env.hpp"
#include "../harness/harness.hpp"
#include "runners.hpp"
#include "sim.hpp"
#include "worlds.hpp"

using namespace skirmish;
using bench::harness::Case;
using bench::harness::Prepared;
using bench::harness::Record;

namespace
{
    constexpr int kWarmup = 150;

    bench::harness::Registry registry;

    template <typename W, bool Fused, typename Runner>
    struct Game
    {
        std::unique_ptr<W> world = std::make_unique<W>();
        std::unique_ptr<fz::Parallel> pool;
        std::unique_ptr<Sim<W, Fused, Runner>> sim;
        std::int64_t startTick = 0;

        explicit Game(Config cfg, unsigned threads = 1, bool affinity = false)
        {
            if (threads > 1)
            {
                pool = std::make_unique<fz::Parallel>(threads, affinity);
                cfg.pool = pool.get();
            }
            sim = std::make_unique<Sim<W, Fused, Runner>>(*world, cfg);
            for (int i = 0; i < kWarmup; ++i) sim->tick();
            sim->enableTimings(true);
            startTick = sim->stats().tick;
        }

        void report(Record& r) const
        {
            const auto end = sim->stats();
            const double ticks = static_cast<double>(end.tick - startTick);
            r.counter("entities", static_cast<double>(end.entities));
            r.counter("projectiles", static_cast<double>(end.projectiles));
            for (int i = 0; i < kSysCount; ++i)
                r.counter(std::string("us_") + kSystemNames[i], ticks > 0 ? 1e6 * sim->timings()[i] / ticks : 0.0);
        }
    };

    template <typename G>
    Prepared prepared(std::shared_ptr<G> game)
    {
        Prepared prep;
        prep.op = [game] { game->sim->tick(); };
        prep.finish = [game](Record& r) { game->report(r); };
        prep.fixture = game;
        return prep;
    }

    template <typename W, bool Fused = false, typename Runner = void>
    void tick(const char* design, int unitsPerTeam)
    {
        registry.add(Case{ "Tick", "-", design, unitsPerTeam, 1.0, "tick", false, 1, [unitsPerTeam] {
                              Config cfg;
                              cfg.unitsPerTeam = unitsPerTeam;
                              return prepared(std::make_shared<Game<W, Fused, Runner>>(cfg));
                          } });
    }

    /** Thread scaling on the store; threads = 1 runs without a pool. */
    void threads(int unitsPerTeam, unsigned threadCount, bool fused, bool affinity)
    {
        const std::string design = "t" + std::to_string(threadCount) + (affinity ? "+affinity" : "");
        registry.add(Case{ "Threads", fused ? "Fused" : "Unfused", design, unitsPerTeam, 1.0, "tick", false, 1,
                           [=] {
                               Config cfg;
                               cfg.unitsPerTeam = unitsPerTeam;
                               cfg.fuseMovement = fused;
                               return prepared(std::make_shared<Game<QPartHintedWorld, true, void>>(cfg, threadCount, affinity));
                           } });
    }
} // namespace

int main(int argc, char** argv)
{
    // SKIRMISH_UPT: units per team (4 teams); default 1K / 10K / 50K units.
    for (std::int64_t uptL : bench::env::list("SKIRMISH_UPT", { 250, 2500, 12500 }))
    {
        const int upt = static_cast<int>(uptL);
        tick<SparseSetWorld>("SparseSet", upt);   // first: the paired baseline (the conformance reference)
        tick<ArchetypeWorld>("Archetype", upt);
        tick<QueryPartWorld>("QueryPart", upt);
        tick<QueryPartWorld, true>("QueryPartFused", upt);
        tick<QPartHintedWorld>("QPartHinted", upt);
        tick<QPartHintedWorld, true>("QPartHintedFused", upt);
        // fusion extension points on the real game (movement group)
        tick<QPartHintedWorld, false, PlannedRunner<fz::NeverFuse>>("QPH+NeverFuse", upt);
        tick<QPartHintedWorld, false, PlannedRunner<fz::ShareColumns, fz::Parallel>>("QPH+Parallel", upt);
        tick<QPartHintedWorld, false, OffloadRunner<>>("QPH+Offload", upt);
        tick<QPartHintedWorld, false, AutoTunedRunner>("QPH+AutoTuned", upt);
        if (upt <= 250) tick<StaticWorld<(1 << 13)>>("StaticBitmask", upt);
        else if (upt <= 2500) tick<StaticWorld<(1 << 16)>>("StaticBitmask", upt);
        else if (upt <= 12500) tick<StaticWorld<(1 << 18)>>("StaticBitmask", upt);
        else if (upt <= 50000) tick<StaticWorld<(1 << 20)>>("StaticBitmask", upt);
        if (upt <= 250) tick<SortedWorld>("SortedSoA", upt);   // O(n) flushes per structural batch: small N only
    }
    // SKIRMISH_THREADS: thread counts; default 1, 2, 4, ... up to hardware concurrency.
    for (std::int64_t upt : bench::env::list("SKIRMISH_THREADS_UPT", { 2500, 12500 }))
        for (bool fused : { false, true })
            for (std::int64_t t : bench::env::list("SKIRMISH_THREADS", bench::env::threadLadder()))
                for (bool affinity : { false, true })
                {
                    if (affinity && (t == 1 || !fused)) continue;
                    threads(static_cast<int>(upt), static_cast<unsigned>(t), fused, affinity);
                }
    return bench::harness::benchMain(argc, argv, registry, "sub0ecs_skirmish_bench");
}
