/** Evolving ordered n-body ticks through the common paired benchmark harness. */
#include <memory>
#include <stdexcept>
#include <string>

#include "common/env.hpp"
#include "harness/harness.hpp"
#include "nbody/simulation.hpp"
#include "sub0ecs/fusion/executors/parallel.hpp"
#if defined(SUB0ECS_TEST_PIPELINE)
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0pipeline/executor/priority_executor.hpp"
#endif

namespace
{
using bench::harness::Case;
using bench::harness::Prepared;
using bench::harness::Record;
template <std::size_t Rows = 1024, typename Simulation = bench::nbody::Ecs>
struct Native
{
    Simulation simulation;
    sub0ecs::fusion::Parallel pool;
    Native(std::size_t n, unsigned workers) : simulation(n), pool(workers) {}
    void tick() { simulation.template tick<Rows>(pool); }
    const auto& state() { return simulation.state(); }
    auto ticks() const { return simulation.ticks(); }
};
#if defined(SUB0ECS_TEST_PIPELINE)
template <std::size_t Rows = 1024, typename Simulation = bench::nbody::Ecs>
struct Bridged
{
    Simulation simulation;
    sub0pipeline::PriorityExecutor executor;
    sub0ecs::adapters::PipelinePool pool;
    Bridged(std::size_t n, unsigned workers)
        : simulation(n), executor({.threadCount = workers, .queueCapacity = workers}), pool(executor) {}
    void tick() { simulation.template tick<Rows>(pool); }
    const auto& state() { return simulation.state(); }
    auto ticks() const { return simulation.ticks(); }
};
#endif

template <typename Simulation>
Prepared prepare(std::shared_ptr<Simulation> simulation)
{
    // Same warmup and fixed operations per epoch for every evolving alternative.
    for (int tick = 0; tick < 2; ++tick) simulation->tick();
    Prepared result;
    result.fixture = simulation;
    result.op = [simulation] { simulation->tick(); };
    result.finish = [simulation](Record& record) {
        double checksum = 0;
        const auto& state = simulation->state();
        for (const auto& body : state) checksum += body.position.x + body.velocity.y;
        record.counter("state_checksum", checksum);
        record.counter("completed_ticks", double(simulation->ticks()));
        record.counter("directed_interactions_per_tick", double(state.size()) * double(state.empty() ? 0 : state.size() - 1));
    };
    return result;
}
}

int main(int argc, char** argv)
{
    bench::harness::Registry registry;
    for (const auto n : bench::env::list("NBODY_SIZES", {64, 1024, 4096}))
    {
        if (n < 1 || n > 16384) throw std::invalid_argument("NBODY_SIZES must be within 1..16384 (quadratic work)");
        registry.add(Case{"NBody", "Ordered", "HandWritten", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Plain>(n)); }});
        registry.add(Case{"NBody", "Ordered", "ECS", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Ecs>(n)); }});
        for (const auto workers : bench::env::list("NBODY_THREADS", {1, 2, 4}))
        {
            if (workers < 1 || workers > 64) throw std::invalid_argument("NBODY_THREADS must be within 1..64");
            registry.add(Case{"NBody", "Ordered", "Native" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<>>(n, static_cast<unsigned>(workers))); }});
#if defined(SUB0ECS_TEST_PIPELINE)
            registry.add(Case{"NBody", "Ordered", "Pipeline" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<>>(n, static_cast<unsigned>(workers))); }});
#endif
            registry.add(Case{"NBody", "Ordered", "NativeG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64>>(n, static_cast<unsigned>(workers))); }});
#if defined(SUB0ECS_TEST_PIPELINE)
            registry.add(Case{"NBody", "Ordered", "PipelineG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64>>(n, static_cast<unsigned>(workers))); }});
#endif
            // Separate groups preserve a same-resource handwritten baseline.
            const auto nativePattern = "MatchedNative" + std::to_string(workers);
            registry.add(Case{"NBody", nativePattern, "HandWritten", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64, bench::nbody::Plain>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", nativePattern, "ECS", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64>>(n, static_cast<unsigned>(workers))); }});
#if defined(SUB0ECS_TEST_PIPELINE)
            const auto pipelinePattern = "MatchedPipeline" + std::to_string(workers);
            registry.add(Case{"NBody", pipelinePattern, "HandWritten", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64, bench::nbody::Plain>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", pipelinePattern, "ECS", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64>>(n, static_cast<unsigned>(workers))); }});
#endif
        }
    }
    return bench::harness::benchMain(argc, argv, registry, "sub0ecs_nbody_bench");
}
