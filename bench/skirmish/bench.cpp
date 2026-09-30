/** Skirmish benchmark: time per simulation tick, per storage design, with a
 * per-system breakdown (us_<system> counters, microseconds per tick).
 *
 * Name: Tick/<Design>/<unitsPerTeam>. Each benchmark warms the game up for
 * kWarmup ticks (armies engaged, projectiles in flight) before timing; every
 * timed iteration is one tick of the evolving game.
 */
#include <benchmark/benchmark.h>

#include <memory>
#include <string>

#include "../common/denormals.hpp"
#include "../common/env.hpp"
#include "sim.hpp"
#include "runners.hpp"
#include "worlds.hpp"

using namespace skirmish;

namespace
{
    constexpr int kWarmup = 150;
    constexpr int kTimedTicks = 200;

    template <typename W, bool Fused, typename Runner = void>
    void BM_Tick(benchmark::State& state)
    {
        Config cfg;
        cfg.unitsPerTeam = static_cast<int>(state.range(0));
        auto world = std::make_unique<W>();
        auto simPtr = std::make_unique<Sim<W, Fused, Runner>>(*world, cfg);
        auto& sim = *simPtr;
        for (int i = 0; i < kWarmup; ++i) sim.tick();
        sim.enableTimings(true);
        const auto start = sim.stats();
        for (auto _ : state) sim.tick();
        const auto end = sim.stats();
        const double ticks = static_cast<double>(end.tick - start.tick);
        state.counters["entities"] = static_cast<double>(end.entities);
        state.counters["projectiles"] = static_cast<double>(end.projectiles);
        for (int i = 0; i < kSysCount; ++i)
            state.counters[std::string("us_") + kSystemNames[i]] = 1e6 * sim.timings()[i] / ticks;
    }

    /** H8 thread scaling: args (unitsPerTeam, threads, fuseMovement). threads=1 → no pool. */
    void BM_Threads(benchmark::State& state)
    {
        const auto threads = static_cast<unsigned>(state.range(1));
        std::unique_ptr<fz::Parallel> pool = threads > 1 ? std::make_unique<fz::Parallel>(threads, state.range(3) != 0) : nullptr;
        Config cfg;
        cfg.unitsPerTeam = static_cast<int>(state.range(0));
        cfg.pool = pool.get();
        cfg.fuseMovement = state.range(2) != 0;
        auto world = std::make_unique<QPartHintedWorld>();
        auto sim = std::make_unique<Sim<QPartHintedWorld, true>>(*world, cfg);
        for (int i = 0; i < kWarmup; ++i) sim->tick();
        sim->enableTimings(true);
        const auto start = sim->stats();
        for (auto _ : state) sim->tick();
        const double ticks = static_cast<double>(sim->stats().tick - start.tick);
        for (int i = 0; i < kSysCount; ++i)
            state.counters[std::string("us_") + kSystemNames[i]] = 1e6 * sim->timings()[i] / ticks;
    }

    template <typename W, bool Fused = false, typename Runner = void>
    void reg(const char* name, int unitsPerTeam)
    {
        // Fixed window (ticks kWarmup..kWarmup+kTimedTicks): every design times the
        // *same* game states (armies shrink as the battle goes on).
        benchmark::RegisterBenchmark((std::string("Tick/") + name).c_str(), BM_Tick<W, Fused, Runner>)
            ->Arg(unitsPerTeam)
            ->Iterations(kTimedTicks)
            ->Unit(benchmark::kMillisecond);
    }
} // namespace

int main(int argc, char** argv)
{
    bench::flushDenormals();
    // SKIRMISH_UPT: units per team (4 teams); default 1K / 10K / 50K units.
    for (std::int64_t uptL : bench::env::list("SKIRMISH_UPT", { 250, 2500, 12500 }))
    {
        const int upt = static_cast<int>(uptL);
        reg<SparseSetWorld>("SparseSet", upt);
        reg<ArchetypeWorld>("Archetype", upt);
        reg<QueryPartWorld>("QueryPart", upt);
        reg<QueryPartWorld, true>("QueryPartFused", upt);
        reg<QPartHintedWorld>("QPartHinted", upt);
        reg<QPartHintedWorld, true>("QPartHintedFused", upt);
        // fusion extension points on the real game (movement group)
        reg<QPartHintedWorld, false, PlannedRunner<fz::NeverFuse>>("QPH+NeverFuse", upt);
        reg<QPartHintedWorld, false, PlannedRunner<fz::ShareColumns, fz::Parallel>>("QPH+Parallel", upt);
        reg<QPartHintedWorld, false, OffloadRunner<>>("QPH+Offload", upt);
        reg<QPartHintedWorld, false, AutoTunedRunner>("QPH+AutoTuned", upt);
        if (upt <= 250) reg<StaticWorld<(1 << 13)>>("StaticBitmask", upt);
        else if (upt <= 2500) reg<StaticWorld<(1 << 16)>>("StaticBitmask", upt);
        else if (upt <= 12500) reg<StaticWorld<(1 << 18)>>("StaticBitmask", upt);
        else if (upt <= 50000) reg<StaticWorld<(1 << 20)>>("StaticBitmask", upt);
        if (upt <= 250) reg<SortedWorld>("SortedSoA", upt);   // O(n) flushes per structural batch: small N only
    }
    // SKIRMISH_THREADS: thread counts; default 1, 2, 4, ... up to hardware concurrency.
    for (std::int64_t upt : bench::env::list("SKIRMISH_THREADS_UPT", { 2500, 12500 }))
        for (std::int64_t threads : bench::env::list("SKIRMISH_THREADS", bench::env::threadLadder()))
            for (int fuse : { 0, 1 })
                for (int affinity : { 0, 1 })
                {
                    if (affinity && (threads == 1 || !fuse)) continue;
                benchmark::RegisterBenchmark("Threads/QPartHinted", BM_Threads)
                    ->Args({ upt, threads, fuse, affinity })
                    ->ArgNames({ "upt", "threads", "fused", "affinity" })
                    ->Iterations(kTimedTicks)
                    ->Unit(benchmark::kMillisecond);
                }
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
