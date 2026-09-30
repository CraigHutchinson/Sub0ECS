/** SubzeroECS v2 design spike — performance baseline.
 *
 * Benchmark names: <Scenario>/<Pattern>/<Design>/<N>
 * items_per_second is always "world entities per second" (N per iteration) so
 * rows are comparable across designs within a scenario.
 */
#include <benchmark/benchmark.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <memory>
#include <new>
#include <string>

#include "../common/scenarios.hpp"
#include "../designs/archetype.hpp"
#include "../designs/query_partition.hpp"
#include "../designs/sorted_soa.hpp"
#include "../designs/sparse_set.hpp"
#include "../designs/static_bitmask.hpp"
#include "../designs/v1_adapter.hpp"
#include "../common/systems.hpp"
#include "../fusion/executors.hpp"
#include "../fusion/planner.hpp"

#if defined(__SSE__) || defined(_M_X64)
#    include <xmmintrin.h>
#endif

// ---- Heap accounting (glibc) ----------------------------------------------
#if defined(__GLIBC__)
#    include <malloc.h>
namespace heap
{
    std::atomic<std::int64_t> liveBytes{ 0 };
    std::atomic<std::int64_t> allocations{ 0 };
} // namespace heap

void* operator new(std::size_t n)
{
    void* p = std::malloc(n ? n : 1);
    if (!p) throw std::bad_alloc();
    heap::liveBytes += static_cast<std::int64_t>(malloc_usable_size(p));
    ++heap::allocations;
    return p;
}
void* operator new(std::size_t n, std::align_val_t a)
{
    void* p = std::aligned_alloc(static_cast<std::size_t>(a), (n + static_cast<std::size_t>(a) - 1) & ~(static_cast<std::size_t>(a) - 1));
    if (!p) throw std::bad_alloc();
    heap::liveBytes += static_cast<std::int64_t>(malloc_usable_size(p));
    ++heap::allocations;
    return p;
}
void operator delete(void* p) noexcept
{
    if (!p) return;
    heap::liveBytes -= static_cast<std::int64_t>(malloc_usable_size(p));
    std::free(p);
}
void operator delete(void* p, std::size_t) noexcept { operator delete(p); }
void operator delete(void* p, std::align_val_t) noexcept { operator delete(p); }
void operator delete(void* p, std::size_t, std::align_val_t) noexcept { operator delete(p); }
#    define SPIKE_HEAP_TRACKING 1
#else
#    define SPIKE_HEAP_TRACKING 0
#endif

namespace
{
    using namespace spike;
    using Clock = std::chrono::steady_clock;

    template <typename W>
    bool skipUnsupported(benchmark::State& state, bool needRemove, bool needDestroy)
    {
        if ((needRemove && !W::kSupportsRemove) || (needDestroy && !W::kSupportsDestroy))
        {
            state.SkipWithError("unsupported by design");
            return true;
        }
        return false;
    }

    /** Physical table count, for designs that have one (fragmentation evidence). */
    template <typename W>
    void reportTables(benchmark::State& state, W& w)
    {
        if constexpr (requires { w.partitionCount(); }) state.counters["tables"] = static_cast<double>(w.partitionCount());
        else if constexpr (requires { w.archetypeCount(); }) state.counters["tables"] = static_cast<double>(w.archetypeCount());
    }

    // ---- Scenarios ----------------------------------------------------------

    template <typename W>
    void BM_Create(benchmark::State& state, Pattern pattern)
    {
        const std::int64_t n = state.range(0);
        double bytesPerEntity = 0.0, allocsPerEntity = 0.0;
        for (auto _ : state)
        {
#if SPIKE_HEAP_TRACKING
            const std::int64_t bytes0 = heap::liveBytes.load();
#endif
            auto w = std::make_unique<W>();
#if SPIKE_HEAP_TRACKING
            const std::int64_t allocs0 = heap::allocations.load();
#endif
            const auto t0 = Clock::now();
            auto es = populate(*w, n, pattern);
            const auto t1 = Clock::now();
            benchmark::DoNotOptimize(es.data());
#if SPIKE_HEAP_TRACKING
            const auto handleBytes = static_cast<std::int64_t>(malloc_usable_size(es.data()));
            bytesPerEntity = static_cast<double>(heap::liveBytes.load() - bytes0 - handleBytes) / static_cast<double>(n);
            allocsPerEntity = static_cast<double>(heap::allocations.load() - allocs0 - 1) / static_cast<double>(n);
#endif
            state.SetIterationTime(std::chrono::duration<double>(t1 - t0).count());
        }
        state.SetItemsProcessed(state.iterations() * n);
        state.counters["bytes_per_entity"] = bytesPerEntity;
        state.counters["allocs_per_entity"] = allocsPerEntity;
    }

    template <typename W, void (*System)(W&)>
    void BM_System(benchmark::State& state, Pattern pattern, bool tagged)
    {
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<W>();
        auto es = populate(*w, n, pattern);
        if (tagged) tagEveryHundredth(*w, es);
        for (auto _ : state)
        {
            System(*w);
            benchmark::ClobberMemory();
        }
        state.SetItemsProcessed(state.iterations() * n);
        reportTables(state, *w);
    }

    template <typename W>
    void BM_RandomGet(benchmark::State& state, Pattern pattern)
    {
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<W>();
        auto es = populate(*w, n, pattern);
        std::shuffle(es.begin(), es.end(), std::mt19937(123));
        for (auto _ : state)
        {
            float sum = 0.0f;
            for (const auto& e : es) sum += w->template find<Velocity>(e)->dx;
            benchmark::DoNotOptimize(sum);
        }
        state.SetItemsProcessed(state.iterations() * n);
    }

    template <typename W>
    void BM_AddRemove(benchmark::State& state, Pattern pattern)
    {
        if (skipUnsupported<W>(state, true, false)) return;
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<W>();
        auto es = populate(*w, n, pattern);
        for (auto _ : state) churnAddRemove(*w, es);
        reportTables(state, *w);
        state.SetItemsProcessed(state.iterations() * n);
        state.counters["churned"] = static_cast<double>((n + 9) / 10);
    }

    template <typename W>
    void BM_TagChurn(benchmark::State& state, Pattern pattern)
    {
        if (skipUnsupported<W>(state, true, false)) return;
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<W>();
        auto es = populate(*w, n, pattern);
        for (auto _ : state) churnTagAddRemove(*w, es);
        reportTables(state, *w);
        state.SetItemsProcessed(state.iterations() * n);
        state.counters["churned"] = static_cast<double>((n + 9) / 10);
    }

    template <typename W>
    void BM_DestroyCreate(benchmark::State& state, Pattern pattern)
    {
        if (skipUnsupported<W>(state, false, true)) return;
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<W>();
        auto es = populate(*w, n, pattern);
        Rng rng(7);
        std::size_t round = 0;
        for (auto _ : state) churnDestroyCreate(*w, es, round++, rng);
        state.SetItemsProcessed(state.iterations() * n);
    }

    /** Hand-written SoA upper bound for Update2 (every entity has Position+Velocity). */
    void BM_Update2_Reference(benchmark::State& state, Pattern)
    {
        const std::int64_t n = state.range(0);
        std::vector<Position> pos(static_cast<std::size_t>(n));
        std::vector<Velocity> vel(static_cast<std::size_t>(n));
        Rng rng;
        for (std::int64_t i = 0; i < n; ++i)
        {
            pos[i] = { rng.next(), rng.next() };
            vel[i] = { rng.next(), rng.next() };
        }
        for (auto _ : state)
        {
            for (std::size_t i = 0; i < pos.size(); ++i) kernel::updatePosition(pos[i], vel[i], kDeltaTime);
            benchmark::ClobberMemory();
        }
        state.SetItemsProcessed(state.iterations() * n);
    }

    // ---- Registration -------------------------------------------------------

    template <typename F>
    void reg(const std::string& scenario, Pattern p, const char* design, std::int64_t n, F fn, bool manualTime = false)
    {
        auto* b = benchmark::RegisterBenchmark((scenario + "/" + toString(p) + "/" + design).c_str(), fn, p);
        b->Arg(n)->Unit(benchmark::kMicrosecond);
        if (manualTime) b->UseManualTime();
    }

    template <typename W>
    void registerDesign(std::int64_t n)
    {
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            reg("Create", p, W::kName, n, BM_Create<W>, true);
            reg("Iter1", p, W::kName, n, [](benchmark::State& s, Pattern pp) { BM_System<W, systemIter1<W>>(s, pp, false); });
            reg("Update2", p, W::kName, n, [](benchmark::State& s, Pattern pp) { BM_System<W, systemPhysics<W>>(s, pp, false); });
        }
        const Pattern f = Pattern::Fragmented;
        reg("Frame3", f, W::kName, n, [](benchmark::State& s, Pattern pp) { BM_System<W, systemFrame3<W>>(s, pp, false); });
        reg("SparseQuery", f, W::kName, n, [](benchmark::State& s, Pattern pp) { BM_System<W, systemSparse<W>>(s, pp, true); });
        reg("RandomGet", f, W::kName, n, BM_RandomGet<W>);
        reg("AddRemove", f, W::kName, n, BM_AddRemove<W>);
        reg("TagChurn", f, W::kName, n, BM_TagChurn<W>);
        reg("DestroyCreate", f, W::kName, n, BM_DestroyCreate<W>);
    }

    /** H7 fusion. Same world, same systems; only the execution strategy differs.
     *  FusionFrame = Integrate, Forces, Wrap (share Position/Velocity) + RotHealth.
     *  Frame3Sys   = Physics, RotHealth, Pulse (disjoint columns).
     *  "HandFused" = the single hand-written Update2 kernel + RotHealth pass:
     *  the upper bound automatic fusion should approach for FusionFrame. */
    template <typename W>
    void registerFusionFor(std::int64_t n, const std::string& name)
    {
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            reg("FusionFrame", p, (name + "Seq").c_str(), n,
                [](benchmark::State& s, Pattern pp) { BM_System<W, systemFusionFrame<W>>(s, pp, false); });
            reg("FusionFrame", p, (name + "Fused").c_str(), n,
                [](benchmark::State& s, Pattern pp) { BM_System<W, systemFusionFrameFused<W>>(s, pp, false); });
            reg("FusionFrame", p, (name + "FusedGrouped").c_str(), n,
                [](benchmark::State& s, Pattern pp) { BM_System<W, systemFusionFrameGrouped<W>>(s, pp, false); });
            reg("FusionFrame", p, (name + "HandFused").c_str(), n,
                [](benchmark::State& s, Pattern pp) { BM_System<W, systemFusionFrameHand<W>>(s, pp, false); });
        }
        reg("Frame3Sys", Pattern::Fragmented, (name + "Seq").c_str(), n,
            [](benchmark::State& s, Pattern pp) { BM_System<W, systemFrame3Seq<W>>(s, pp, false); });
        reg("Frame3Sys", Pattern::Fragmented, (name + "Fused").c_str(), n,
            [](benchmark::State& s, Pattern pp) { BM_System<W, systemFrame3Fused<W>>(s, pp, false); });
    }

    // ---- Fusion extension points: planner x executor (QPartHinted) ----------
    template <int Frame, typename Planner, typename Exec, typename Host>
    void runFrame(qpart::HintedWorld& w, Exec& exec, Host& host)
    {
        if constexpr (Frame == 0)
            fusion::runPlanned<Planner>(w, exec, host, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
        else
            fusion::runPlanned<Planner>(w, exec, host, PhysicsSys{}, RotHealthSys{}, PulseSys{});
    }

    template <int Frame, typename Planner, typename Exec>
    void BM_Planned(benchmark::State& state, Pattern pattern)
    {
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<qpart::HintedWorld>();
        auto es = populate(*w, n, pattern);
        static Exec exec{};   // persistent (thread pool)
        fusion::Inline host;
        for (auto _ : state)
        {
            runFrame<Frame, Planner>(*w, exec, host);
            benchmark::ClobberMemory();
        }
        state.SetItemsProcessed(state.iterations() * n);
    }

    template <int Frame>
    void BM_AutoTuned(benchmark::State& state, Pattern pattern)
    {
        const std::int64_t n = state.range(0);
        auto w = std::make_unique<qpart::HintedWorld>();
        auto es = populate(*w, n, pattern);
        fusion::AutoTuner<fusion::NeverFuse, fusion::ShareColumns, fusion::AlwaysFuse> tuner;
        fusion::Inline e;
        auto frame = [&] {
            if constexpr (Frame == 0) tuner.run(*w, e, e, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
            else tuner.run(*w, e, e, PhysicsSys{}, RotHealthSys{}, PulseSys{});
        };
        while (!tuner.decided()) frame();   // "measure" phase before timing
        for (auto _ : state)
        {
            frame();
            benchmark::ClobberMemory();
        }
        state.SetItemsProcessed(state.iterations() * n);
        state.SetLabel(std::string("chose ") + tuner.names()[tuner.chosen()]);
    }

    template <int Frame>
    void registerFusionExec(std::int64_t n, const char* scenario)
    {
        const Pattern f = Pattern::Fragmented;
        using fusion::AlwaysFuse, fusion::NeverFuse, fusion::ShareColumns;
        reg(scenario, f, "NeverFuse", n, BM_Planned<Frame, NeverFuse, fusion::Inline>);
        reg(scenario, f, "AlwaysFuse", n, BM_Planned<Frame, AlwaysFuse, fusion::Inline>);
        reg(scenario, f, "ShareColumns", n, BM_Planned<Frame, ShareColumns, fusion::Inline>);
        reg(scenario, f, "AutoTuned", n, BM_AutoTuned<Frame>);
        reg(scenario, f, "ShareColumns+Tiled4K", n, BM_Planned<Frame, ShareColumns, fusion::Tiled<4096>>);
        reg(scenario, f, "ShareColumns+Parallel", n, BM_Planned<Frame, ShareColumns, fusion::Parallel>);
        reg(scenario, f, "DeviceAware+Offload1K", n,
            BM_Planned<Frame, fusion::DeviceAware<ShareColumns>, fusion::Offload<fusion::EmulatedDevice, 1024>>);
    }

    void registerFusion(std::int64_t n)
    {
        registerFusionExec<0>(n, "FusionExec");
        registerFusionExec<1>(n, "Frame3Exec");
        registerFusionFor<qpart::HintedWorld>(n, "QPartHinted");
        // Sequential on the other designs for context
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            reg("FusionFrame", p, "ArchetypeSeq", n, [](benchmark::State& s, Pattern pp) {
                BM_System<archetype::World, systemFusionFrame<archetype::World>>(s, pp, false);
            });
            reg("FusionFrame", p, "SparseSetSeq", n, [](benchmark::State& s, Pattern pp) {
                BM_System<sparse::World, systemFusionFrame<sparse::World>>(s, pp, false);
            });
        }
    }

    void registerAll(std::int64_t n)
    {
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented }) reg("Update2", p, "RawSoA", n, BM_Update2_Reference);
        registerDesign<v1::World>(n);
        registerDesign<sorted::World>(n);
        registerDesign<sparse::World>(n);
        registerDesign<archetype::World>(n);
        registerDesign<qpart::World>(n);
        registerDesign<qpart::HintedWorld>(n);
        registerFusion(n);
        // Static design must be sized at compile time for each N.
        if (n <= 1024) registerDesign<fixed::World<1024>>(n);
        else if (n <= (1 << 17)) registerDesign<fixed::World<(1 << 17)>>(n);
        else registerDesign<fixed::World<(1 << 20)>>(n);
    }
} // namespace

int main(int argc, char** argv)
{
    // Flush denormals to zero (as the v1 benchmark gets implicitly via -ffast-math).
    // The physics kernel damps velocity every step, so without FTZ/DAZ long-running
    // benchmarks drift into denormal arithmetic and measure the FPU, not the ECS.
#if defined(__SSE__) || defined(_M_X64)
    _mm_setcsr(_mm_getcsr() | 0x8040u);
#endif
    std::int64_t sizes[] = { 1'000, 100'000, 1'000'000 };
    if (const char* env = std::getenv("SPIKE_SIZES"); env && std::string(env) == "small")
    {
        sizes[1] = sizes[2] = 0;
    }
    for (std::int64_t n : sizes)
    {
        if (n) registerAll(n);
    }
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
