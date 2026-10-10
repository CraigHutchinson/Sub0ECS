/** Representative evolving row workloads through the common paired benchmark harness. */
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

#include "common/env.hpp"
#include "harness/harness.hpp"
#include "row_workloads/plain.hpp"
#include "row_workloads/ecs.hpp"
#include "common/native_simulation.hpp"
#if defined(SUB0ECS_TEST_PIPELINE)
#include "common/pipeline_simulation.hpp"
#endif

namespace
{
using bench::harness::Case;
using bench::harness::Prepared;
using bench::harness::Record;
template <std::size_t Rows = 1024, typename Simulation = bench::rows::Ecs>
using Native = bench::NativeSimulation<Simulation, Rows>;
#if defined(SUB0ECS_TEST_PIPELINE)
template <std::size_t Rows = 1024, typename Simulation = bench::rows::Ecs>
using Bridged = bench::PipelineSimulation<Simulation, Rows>;
#endif

template <typename Simulation>
Prepared prepare(std::shared_ptr<Simulation> simulation, bench::rows::Scenario scenario)
{
    // Same warmup and fixed operations per epoch for every evolving alternative.
    for (int tick = 0; tick < 2; ++tick) simulation->tick();
    Prepared result;
    result.fixture = simulation;
    result.op = [simulation] { simulation->tick(); };
    result.finish = [simulation, scenario](Record& record) {
        double checksum = 0;
        const auto& state = simulation->state();
        for (const auto& body : state) checksum += body.x + body.vy;
        record.counter("state_checksum", checksum);
        record.counter("completed_ticks", double(simulation->ticks()));
        record.counter("entities_per_tick", double(state.size()));
        record.counter("neighbor_reads_per_tick", scenario == bench::rows::Scenario::StagedNeighbors ? 8.0 * state.size() : 0.0);
        const auto ticks = simulation->ticks();
        const auto toggles = (ticks / 16) * state.size() + (state.size() / 16) * (ticks % 16)
            + std::min(state.size() % 16, ticks % 16);
        record.counter("structural_toggles_completed", scenario == bench::rows::Scenario::FragmentedChurn ? double(toggles) : 0.0);
    };
    return result;
}
}

int main(int argc, char** argv)
{
    using bench::rows::Scenario;
    const auto widths = bench::env::list("ROW_THREADS", {1, 2, 4});
    // The paired harness holds 16 alternatives. Chunking larger evolving groups
    // would advance the shared baseline more ticks than subsequent alternatives.
    if (widths.empty() || widths.size() > 3)
        throw std::invalid_argument("Use one to three worker widths per evolving comparison");
    bench::harness::Registry registry;
    for (const auto n : bench::env::list("ROW_SIZES", {1024, 16384, 65536}))
    {
        if (n < 1 || n > 1000000) throw std::invalid_argument("ROW_SIZES must be within 1..1000000");
        for (const auto scenario : {Scenario::Streaming, Scenario::FragmentedChurn, Scenario::StagedNeighbors})
        {
            const std::string pattern = scenario == Scenario::Streaming ? "Streaming" :
                scenario == Scenario::FragmentedChurn ? "FragmentedChurn" : "StagedNeighbors";
            registry.add(Case{"RowWork", pattern, "HandWritten", n, 1, "tick", false, 1,
                [n, scenario] { return prepare(std::make_shared<bench::rows::Plain>(n, scenario), scenario); }});
            registry.add(Case{"RowWork", pattern, "ECS", n, 1, "tick", false, 1,
                [n, scenario] { return prepare(std::make_shared<bench::rows::Ecs>(n, scenario), scenario); }});
            for (const auto workers : widths)
            {
                if (workers < 1 || workers > 64) throw std::invalid_argument("ROW_THREADS must be within 1..64");
                registry.add(Case{"RowWork", pattern, "NativeG1024x" + std::to_string(workers), n, 1, "tick", false, 1,
                    [n, workers, scenario] { return prepare(std::make_shared<Native<1024>>(n, static_cast<unsigned>(workers), scenario), scenario); }});
                registry.add(Case{"RowWork", pattern, "NativeG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                    [n, workers, scenario] { return prepare(std::make_shared<Native<64>>(n, static_cast<unsigned>(workers), scenario), scenario); }});
#if defined(SUB0ECS_TEST_PIPELINE)
                registry.add(Case{"RowWork", pattern, "PipelineG1024x" + std::to_string(workers), n, 1, "tick", false, 1,
                    [n, workers, scenario] { return prepare(std::make_shared<Bridged<1024>>(n, static_cast<unsigned>(workers), scenario), scenario); }});
                registry.add(Case{"RowWork", pattern, "PipelineG64x" + std::to_string(workers), n, 1, "tick", false, 1,
                    [n, workers, scenario] { return prepare(std::make_shared<Bridged<64>>(n, static_cast<unsigned>(workers), scenario), scenario); }});
#endif
            }
        }
    }
    return bench::harness::benchMain(argc, argv, registry, "sub0ecs_row_workloads_bench");
}
