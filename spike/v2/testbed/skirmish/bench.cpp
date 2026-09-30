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
#if defined(__SSE__) || defined(_M_X64)
#    include <xmmintrin.h>
#endif

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
#if defined(__SSE__) || defined(_M_X64)
    _mm_setcsr(_mm_getcsr() | 0x8040u);
#endif
    for (int upt : { 250, 2500, 12500 })   // 1K, 10K, 50K units (+ projectiles, buildings)
    {
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
        else reg<StaticWorld<(1 << 18)>>("StaticBitmask", upt);
        if (upt <= 250) reg<SortedWorld>("SortedSoA", upt);   // O(n) flushes per structural batch: small N only
    }
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
