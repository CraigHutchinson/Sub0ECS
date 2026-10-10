/** Evolving ordered n-body ticks through the common paired benchmark harness. */
#include <memory>
#include <stdexcept>
#include <string>

#include "common/env.hpp"
#include "harness/harness.hpp"
#include "nbody/simulation.hpp"
#include "common/native_simulation.hpp"
#if defined(SUB0ECS_TEST_PIPELINE)
#include "common/pipeline_simulation.hpp"
#endif

namespace
{
using bench::harness::Case;
using bench::harness::Prepared;
using bench::harness::Record;
template <std::size_t Rows = 1024, typename Simulation = bench::nbody::Ecs>
using Native = bench::NativeSimulation<Simulation, Rows>;
#if defined(SUB0ECS_TEST_PIPELINE)
template <std::size_t Rows = 1024, typename Simulation = bench::nbody::Ecs>
using Bridged = bench::PipelineSimulation<Simulation, Rows>;
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
    const auto widths = bench::env::list("NBODY_THREADS", {1, 2, 4});
    // The paired harness holds 16 alternatives. Chunking larger evolving groups
    // would advance the shared baseline more ticks than subsequent alternatives.
    if (widths.empty() || widths.size() > 3)
        throw std::invalid_argument("Use one to three worker widths per evolving comparison");
    bench::harness::Registry registry;
    for (const auto n : bench::env::list("NBODY_SIZES", {64, 1024, 4096}))
    {
        if (n < 1 || n > 16384) throw std::invalid_argument("NBODY_SIZES must be within 1..16384 (quadratic work)");
        registry.add(Case{"NBody", "Ordered", "HandWritten", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Plain>(n)); }});
        registry.add(Case{"NBody", "Ordered", "ECS", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Ecs>(n)); }});
        registry.add(Case{"NBody", "Compact", "HandWritten", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Plain>(n)); }});
        registry.add(Case{"NBody", "Compact", "ECS", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::Ecs>(n)); }});
        registry.add(Case{"NBody", "Compact", "CompactECS", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::CompactEcs>(n)); }});
        registry.add(Case{"NBody", "Compact", "CompactHandWritten", n, 1, "tick", false, 1,
            [n] { return prepare(std::make_shared<bench::nbody::CompactPlain>(n)); }});
        for (const auto workers : widths)
        {
            registry.add(Case{"NBody", "Compact", "NativeG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", "Compact", "CompactNativeG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64, bench::nbody::CompactEcs>>(n, static_cast<unsigned>(workers))); }});
#if defined(SUB0ECS_TEST_PIPELINE)
            registry.add(Case{"NBody", "Compact", "PipelineG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", "Compact", "CompactPipelineG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64, bench::nbody::CompactEcs>>(n, static_cast<unsigned>(workers))); }});
#endif
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
            const auto compactNativePattern = "CompactMatchedNative" + std::to_string(workers);
            registry.add(Case{"NBody", compactNativePattern, "HandWritten", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64, bench::nbody::CompactPlain>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", compactNativePattern, "ECS", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64, bench::nbody::CompactEcs>>(n, static_cast<unsigned>(workers))); }});
            const auto nativePattern = "MatchedNative" + std::to_string(workers);
            registry.add(Case{"NBody", nativePattern, "HandWritten", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64, bench::nbody::Plain>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", nativePattern, "ECS", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Native<64>>(n, static_cast<unsigned>(workers))); }});
#if defined(SUB0ECS_TEST_PIPELINE)
            const auto compactPipelinePattern = "CompactMatchedPipeline" + std::to_string(workers);
            registry.add(Case{"NBody", compactPipelinePattern, "HandWritten", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64, bench::nbody::CompactPlain>>(n, static_cast<unsigned>(workers))); }});
            registry.add(Case{"NBody", compactPipelinePattern, "ECS", n, 1, "tick", false, 1,
                [n, workers] { return prepare(std::make_shared<Bridged<64, bench::nbody::CompactEcs>>(n, static_cast<unsigned>(workers))); }});
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
