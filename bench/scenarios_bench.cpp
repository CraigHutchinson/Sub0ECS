/** Storage designs on micro scenarios, fusion, and planners x executors.
 *
 * Case names: <Scenario>/<Pattern>/<Design>/<N>. One operation is one pass over
 * the whole world, so items_per_second is world entities per second and rows are
 * comparable across designs within a scenario. Designs of a group are measured
 * with a paired comparison against the first registered: the hand-written loop
 * where one exists (iteration and lookup), the sparse set elsewhere (see
 * bench/harness/runner.hpp).
 */
#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <memory>
#include <new>
#include <random>
#include <string>
#include <vector>

#include <nanobench.h>
#include "sub0ecs/fusion/executors.hpp"
#include "sub0ecs/fusion/planner.hpp"

#include "common/env.hpp"
#include "common/scenarios.hpp"
#include "common/systems.hpp"
#include "designs/archetype.hpp"
#include "designs/handwritten.hpp"
#include "designs/naive_components.hpp"
#include "designs/oop.hpp"
#include "designs/query_partition.hpp"
#include "designs/sorted_soa.hpp"
#include "designs/sparse_set.hpp"
#include "designs/static_bitmask.hpp"
#include "harness/harness.hpp"

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
#    define BENCH_HEAP_TRACKING 1
#else
#    define BENCH_HEAP_TRACKING 0
#endif

namespace
{
    using namespace bench;
    using harness::Case;
    using harness::Prepared;
    using harness::Record;

    harness::Registry registry;

    /** A world populated with n entities of the pattern, plus their handles. */
    template <typename W>
    struct WorldFixture
    {
        std::unique_ptr<W> world = std::make_unique<W>();
        std::vector<typename W::Entity> entities;
    };

    template <typename W>
    std::shared_ptr<WorldFixture<W>> populated(std::int64_t n, Pattern p, bool tagged = false)
    {
        auto fx = std::make_shared<WorldFixture<W>>();
        fx->entities = populate(*fx->world, n, p);
        if (tagged) tagEveryHundredth(*fx->world, fx->entities);
        return fx;
    }

    /** Physical table count, for designs that have one (fragmentation evidence). */
    template <typename W>
    void reportTables(W& w, Record& r)
    {
        if constexpr (requires { w.partitionCount(); }) r.counter("tables", static_cast<double>(w.partitionCount()));
        else if constexpr (requires { w.archetypeCount(); }) r.counter("tables", static_cast<double>(w.archetypeCount()));
    }

    /** Registers a steady-state case: `make()` builds the fixture once, `op(fixture)`
     *  is one timed pass, `finish(fixture, record)` adds counters afterwards. */
    template <typename Make, typename Op, typename Finish>
    void add(std::string scenario, Pattern p, std::string design, std::int64_t n, Make make, Op op, Finish finish)
    {
        registry.add(Case{ std::move(scenario), toString(p), std::move(design), n, static_cast<double>(n), "pass", false, 0,
                           [=]() {
                               auto fx = make();
                               Prepared prep;
                               prep.op = [fx, op] { op(*fx); };
                               prep.finish = [fx, finish](Record& r) { finish(*fx, r); };
                               prep.fixture = fx;
                               return prep;
                           } });
    }

    template <typename Make, typename Op>
    void add(std::string scenario, Pattern p, std::string design, std::int64_t n, Make make, Op op)
    {
        add(std::move(scenario), p, std::move(design), n, make, op, [](auto&, Record&) {});
    }

    // ---- Scenarios ----------------------------------------------------------

    /** Create: populate a fresh world. The world's construction is untimed (setup);
     *  heap bytes and allocations per entity are measured once, untimed (glibc only). */
    template <typename W>
    void addCreate(Pattern p, std::int64_t n)
    {
        struct Fixture
        {
            std::unique_ptr<W> world;
            std::vector<typename W::Entity> entities;
        };
        registry.add(Case{ "Create", toString(p), W::kName, n, static_cast<double>(n), "pass", true, 0, [p, n]() {
                              double bytesPerEntity = 0.0, allocsPerEntity = 0.0;
#if BENCH_HEAP_TRACKING
                              {
                                  const std::int64_t bytes0 = heap::liveBytes.load();
                                  auto w = std::make_unique<W>();
                                  const std::int64_t allocs0 = heap::allocations.load();
                                  auto es = populate(*w, n, p);
                                  const auto handleBytes = static_cast<std::int64_t>(malloc_usable_size(es.data()));
                                  bytesPerEntity = static_cast<double>(heap::liveBytes.load() - bytes0 - handleBytes) / static_cast<double>(n);
                                  allocsPerEntity = static_cast<double>(heap::allocations.load() - allocs0 - 1) / static_cast<double>(n);
                              }
#endif
                              auto fx = std::make_shared<Fixture>();
                              Prepared prep;
                              // Untimed: drop the previous world and handles, build an empty world.
                              prep.setup = [fx] {
                                  fx->entities = {};
                                  fx->world = std::make_unique<W>();
                              };
                              prep.op = [fx, p, n] {
                                  fx->entities = populate(*fx->world, n, p);
                                  ankerl::nanobench::doNotOptimizeAway(fx->entities.data());
                              };
                              prep.finish = [bytesPerEntity, allocsPerEntity](Record& r) {
                                  if (BENCH_HEAP_TRACKING)
                                  {
                                      r.counter("bytes_per_entity", bytesPerEntity);
                                      r.counter("allocs_per_entity", allocsPerEntity);
                                  }
                              };
                              prep.fixture = fx;
                              return prep;
                          } });
    }

    template <typename W, void (*System)(W&)>
    void addSystem(const char* scenario, Pattern p, std::string design, std::int64_t n, bool tagged = false)
    {
        add(scenario, p, std::move(design), n, [=] { return populated<W>(n, p, tagged); },
            [](WorldFixture<W>& fx) { System(*fx.world); },
            [](WorldFixture<W>& fx, Record& r) { reportTables(*fx.world, r); });
    }

    template <typename W>
    void registerDesign(std::int64_t n)
    {
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            addCreate<W>(p, n);
            addSystem<W, systemIter1<W>>("Iter1", p, W::kName, n);
            addSystem<W, systemPhysics<W>>("Update2", p, W::kName, n);
        }
        const Pattern f = Pattern::Fragmented;
        addSystem<W, systemFrame3<W>>("Frame3", f, W::kName, n);
        addSystem<W, systemSparse<W>>("SparseQuery", f, W::kName, n, true);

        add("RandomGet", f, W::kName, n,
            [=] {
                auto fx = populated<W>(n, f);
                std::shuffle(fx->entities.begin(), fx->entities.end(), std::mt19937(123));
                return fx;
            },
            [](WorldFixture<W>& fx) {
                float sum = 0.0f;
                for (const auto& e : fx.entities) sum += fx.world->template find<Velocity>(e)->dx;
                ankerl::nanobench::doNotOptimizeAway(sum);
            });

        if constexpr (W::kSupportsRemove)
        {
            auto churned = [n](auto&, Record& r) { r.counter("churned", static_cast<double>((n + 9) / 10)); };
            add("AddRemove", f, W::kName, n, [=] { return populated<W>(n, f); },
                [](WorldFixture<W>& fx) { churnAddRemove(*fx.world, fx.entities); },
                [churned](WorldFixture<W>& fx, Record& r) { reportTables(*fx.world, r); churned(fx, r); });
            add("TagChurn", f, W::kName, n, [=] { return populated<W>(n, f); },
                [](WorldFixture<W>& fx) { churnTagAddRemove(*fx.world, fx.entities); },
                [churned](WorldFixture<W>& fx, Record& r) { reportTables(*fx.world, r); churned(fx, r); });
        }
        if constexpr (W::kSupportsDestroy)
        {
            struct Churn : WorldFixture<W>
            {
                Rng rng{ 7 };
                std::size_t round = 0;
            };
            add("DestroyCreate", f, W::kName, n,
                [=] {
                    auto fx = std::make_shared<Churn>();
                    fx->entities = populate(*fx->world, n, f);
                    return fx;
                },
                [](Churn& fx) { churnDestroyCreate(*fx.world, fx.entities, fx.round++, fx.rng); });
        }
    }

    /** Classical inheritance: one heap object per entity and one virtual call per
     *  object per scenario. No queries or structural change (see designs/oop.hpp). */
    void registerOop(std::int64_t n)
    {
        using Fixture = WorldFixture<oop::World>;
        // Not populated<>(): the hierarchy has no add() to tag with.
        const auto make = [](std::int64_t count, Pattern p) {
            auto fx = std::make_shared<Fixture>();
            fx->entities = populate(*fx->world, count, p);
            return fx;
        };
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            addCreate<oop::World>(p, n);
            add("Iter1", p, oop::World::kName, n, [=] { return make(n, p); }, [](Fixture& fx) { fx.world->nudgeAll(); });
            add("Update2", p, oop::World::kName, n, [=] { return make(n, p); }, [](Fixture& fx) { fx.world->updateAll(); });
        }
        const Pattern f = Pattern::Fragmented;
        add("Frame3", f, oop::World::kName, n, [=] { return make(n, f); }, [](Fixture& fx) { fx.world->frameAll(); });
        add("RandomGet", f, oop::World::kName, n,
            [=] {
                auto fx = make(n, f);
                std::shuffle(fx->entities.begin(), fx->entities.end(), std::mt19937(123));
                return fx;
            },
            [](Fixture& fx) {
                float sum = 0.0f;
                for (const auto& e : fx.entities) sum += fx.world->velocity(e).dx;
                ankerl::nanobench::doNotOptimizeAway(sum);
            });
    }

    /** The bars: the workload written by hand with no ECS (designs/handwritten.hpp).
     *  Registered first, so HandWritten is the paired baseline of these scenarios. */
    template <typename H>
    void registerHand(std::int64_t n)
    {
        struct Fixture
        {
            H world;
            std::vector<std::uint32_t> handles;   // RandomGet: every entity, shuffled
            Fixture(std::int64_t count, Pattern p) : world(count, p) {}
        };
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            add("Iter1", p, H::kName, n, [=] { return std::make_shared<Fixture>(n, p); }, [](Fixture& fx) { fx.world.iter1(); });
            add("Update2", p, H::kName, n, [=] { return std::make_shared<Fixture>(n, p); }, [](Fixture& fx) { fx.world.update2(); });
        }
        const Pattern f = Pattern::Fragmented;
        add("Frame3", f, H::kName, n, [=] { return std::make_shared<Fixture>(n, f); }, [](Fixture& fx) { fx.world.frame3(); });
        add("RandomGet", f, H::kName, n,
            [=] {
                auto fx = std::make_shared<Fixture>(n, f);
                for (std::int64_t i = 0; i < n; ++i) fx->handles.push_back(hand::handleOf(hand::placeOf(static_cast<std::size_t>(i), f)));
                std::shuffle(fx->handles.begin(), fx->handles.end(), std::mt19937(123));
                return fx;
            },
            [](Fixture& fx) { ankerl::nanobench::doNotOptimizeAway(fx.world.randomGet(fx.handles)); });
    }

    /** The store's own methodology beside the bars: the same work written as several
     *  small single-purpose systems, run as separate passes and then fused.
     *
     *  Update2 becomes Integrate, Forces and Wrap (applied in order per row they equal
     *  kernel::updatePosition, bit for bit); Frame3 becomes those three plus RotHealth
     *  and Pulse. "Seq" is one pass per system: what small systems cost with no fusion.
     *  "Fused" is what the library does with them: for Update2 one fused pass, for
     *  Frame3 the ShareColumns plan (fuse what shares columns, leave the rest apart).
     *  They join the Update2 and Frame3 groups, so each is paired with HandWritten. */
    void registerSmallSystems(std::int64_t n)
    {
        using W = qpart::HintedWorld;
        using Fixture = WorldFixture<W>;
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            add("Update2", p, "QPartHinted3Seq", n, [=] { return populated<W>(n, p); },
                [](Fixture& fx) { runSequential(*fx.world, Integrate{}, Forces{}, Wrap{}); });
            add("Update2", p, "QPartHinted3Fused", n, [=] { return populated<W>(n, p); },
                [](Fixture& fx) { fx.world->runFused(Integrate{}, Forces{}, Wrap{}); });
        }
        const Pattern f = Pattern::Fragmented;
        add("Frame3", f, "QPartHinted5Seq", n, [=] { return populated<W>(n, f); },
            [](Fixture& fx) { runSequential(*fx.world, Integrate{}, Forces{}, Wrap{}, RotHealthSys{}, PulseSys{}); });
        add("Frame3", f, "QPartHinted5Fused", n, [=] { return populated<W>(n, f); },
            [](Fixture& fx) {
                fusion::Inline exec, host;
                fusion::runPlanned<fusion::ShareColumns>(*fx.world, exec, host, Integrate{}, Forces{}, Wrap{}, RotHealthSys{}, PulseSys{});
            });
    }

    /** Fusion. Same world, same systems; only the execution strategy differs.
     *  FusionFrame = Integrate, Forces, Wrap (share Position/Velocity) + RotHealth.
     *  Frame3Sys   = Physics, RotHealth, Pulse (disjoint columns).
     *  "HandFused" = the single hand-written Update2 kernel + RotHealth pass:
     *  the upper bound automatic fusion should approach for FusionFrame. */
    template <typename W>
    void registerFusionFor(std::int64_t n, const std::string& name)
    {
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            addSystem<W, systemFusionFrame<W>>("FusionFrame", p, name + "Seq", n);
            addSystem<W, systemFusionFrameFused<W>>("FusionFrame", p, name + "Fused", n);
            addSystem<W, systemFusionFrameGrouped<W>>("FusionFrame", p, name + "FusedGrouped", n);
            addSystem<W, systemFusionFrameHand<W>>("FusionFrame", p, name + "HandFused", n);
        }
        addSystem<W, systemFrame3Seq<W>>("Frame3Sys", Pattern::Fragmented, name + "Seq", n);
        addSystem<W, systemFrame3Fused<W>>("Frame3Sys", Pattern::Fragmented, name + "Fused", n);
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
    void addPlanned(const char* scenario, const char* design, std::int64_t n)
    {
        struct Fixture : WorldFixture<qpart::HintedWorld>
        {
            Exec exec{};   // owns its thread pool, if any
            fusion::Inline host;
        };
        const Pattern f = Pattern::Fragmented;
        add(scenario, f, design, n,
            [=] {
                auto fx = std::make_shared<Fixture>();
                fx->entities = populate(*fx->world, n, f);
                return fx;
            },
            [](Fixture& fx) { runFrame<Frame, Planner>(*fx.world, fx.exec, fx.host); });
    }

    template <int Frame>
    void addAutoTuned(const char* scenario, std::int64_t n)
    {
        using Tuner = fusion::AutoTuner<fusion::NeverFuse, fusion::ShareColumns, fusion::AlwaysFuse>;
        struct Fixture : WorldFixture<qpart::HintedWorld>
        {
            Tuner tuner;
            fusion::Inline exec;
            void frame()
            {
                if constexpr (Frame == 0) tuner.run(*world, exec, exec, Integrate{}, Forces{}, Wrap{}, RotHealthSys{});
                else tuner.run(*world, exec, exec, PhysicsSys{}, RotHealthSys{}, PulseSys{});
            }
        };
        const Pattern f = Pattern::Fragmented;
        add(scenario, f, "AutoTuned", n,
            [=] {
                auto fx = std::make_shared<Fixture>();
                fx->entities = populate(*fx->world, n, f);
                while (!fx->tuner.decided()) fx->frame();   // the "measure" phase happens before timing
                return fx;
            },
            [](Fixture& fx) { fx.frame(); },
            [](Fixture& fx, Record& r) { r.note("chose", Tuner::names()[fx.tuner.chosen()]); });
    }

    template <int Frame>
    void registerFusionExec(std::int64_t n, const char* scenario)
    {
        using fusion::AlwaysFuse, fusion::NeverFuse, fusion::ShareColumns;
        addPlanned<Frame, NeverFuse, fusion::Inline>(scenario, "NeverFuse", n);
        addPlanned<Frame, AlwaysFuse, fusion::Inline>(scenario, "AlwaysFuse", n);
        addPlanned<Frame, ShareColumns, fusion::Inline>(scenario, "ShareColumns", n);
        addAutoTuned<Frame>(scenario, n);
        addPlanned<Frame, ShareColumns, fusion::Tiled<4096>>(scenario, "ShareColumns+Tiled4K", n);
        addPlanned<Frame, ShareColumns, fusion::Parallel>(scenario, "ShareColumns+Parallel", n);
        addPlanned<Frame, fusion::DeviceAware<ShareColumns>, fusion::Offload<fusion::EmulatedDevice, 1024>>(
            scenario, "DeviceAware+Offload1K", n);
    }

    void registerFusion(std::int64_t n)
    {
        registerFusionExec<0>(n, "FusionExec");
        registerFusionExec<1>(n, "Frame3Exec");
        registerFusionFor<qpart::HintedWorld>(n, "QPartHinted");
        // Sequential on the other designs for context
        for (Pattern p : { Pattern::Coherent, Pattern::Fragmented })
        {
            addSystem<archetype::World, systemFusionFrame<archetype::World>>("FusionFrame", p, "ArchetypeSeq", n);
            addSystem<sparse::World, systemFusionFrame<sparse::World>>("FusionFrame", p, "SparseSetSeq", n);
        }
    }

    void registerAll(std::int64_t n)
    {
        registerHand<hand::Plain>(n);   // first: the paired baseline of the iteration and lookup scenarios
        registerHand<hand::Tuned>(n);
        registerDesign<sparse::World>(n);   // the paired baseline of the remaining scenarios
        registerDesign<sorted::World>(n);
        registerDesign<archetype::World>(n);
        registerDesign<qpart::World>(n);
        registerDesign<qpart::HintedWorld>(n);
        registerSmallSystems(n);
        // Static design must be sized at compile time for each N; larger N has no instantiation.
        if (n <= 1024) registerDesign<fixed::World<1024>>(n);
        else if (n <= (1 << 17)) registerDesign<fixed::World<(1 << 17)>>(n);
        else if (n <= (1 << 20)) registerDesign<fixed::World<(1 << 20)>>(n);
        registerOop(n);
        // One allocation per component: ~300 bytes per entity, so not beyond 1M entities.
        if (n <= (1 << 20)) registerDesign<naive::World>(n);
        registerFusion(n);
    }
} // namespace

int main(int argc, char** argv)
{
    // BENCH_SIZES: comma list of entity counts ("small" = 1000 only); default 1K, 100K, 1M.
    std::vector<std::int64_t> sizes = env::list("BENCH_SIZES", { 1'000, 100'000, 1'000'000 });
    if (const char* e = std::getenv("BENCH_SIZES"); e && std::string(e) == "small") sizes = { 1'000 };
    for (std::int64_t n : sizes)
        if (n > 0) registerAll(n);
    return harness::benchMain(argc, argv, registry, "sub0ecs_bench");
}
