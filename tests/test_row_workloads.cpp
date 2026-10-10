#include <bit>
#include <cstdint>
#include <doctest/doctest.h>
#include "row_workloads/plain.hpp"
#include "row_workloads/ecs.hpp"
#include "sub0ecs/fusion/executors/parallel.hpp"
#if defined(SUB0ECS_TEST_PIPELINE)
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0pipeline/executor/priority_executor.hpp"
#endif
namespace
{
void sameRows(bench::rows::Plain& reference, bench::rows::Ecs& candidate)
{
    const auto& a = reference.state();
    const auto& b = candidate.state();
    REQUIRE(a.size() == b.size());
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        CHECK(a[i].id == b[i].id);
        const double left[]{a[i].x, a[i].y, a[i].vx, a[i].vy};
        const double right[]{b[i].x, b[i].y, b[i].vx, b[i].vy};
        for (unsigned field = 0; field < 4; ++field)
            CHECK(std::bit_cast<std::uint64_t>(left[field]) == std::bit_cast<std::uint64_t>(right[field]));
    }
    CHECK(reference.tags() == candidate.tags());
    CHECK(reference.ticks() == candidate.ticks());
}
}
TEST_CASE("representative row workloads preserve evolving state and actual tag membership")
{
    using bench::rows::Scenario;
    for (auto scenario : {Scenario::Streaming, Scenario::FragmentedChurn, Scenario::StagedNeighbors})
    for (auto count : {0u, 1u, 17u, 257u})
    for (auto width : {1u, 2u, 4u})
    {
        bench::rows::Plain reference(count, scenario);
        bench::rows::Ecs sequential(count, scenario), native(count, scenario);
        sub0ecs::fusion::Parallel pool(width);
#if defined(SUB0ECS_TEST_PIPELINE)
        bench::rows::Ecs bridged(count, scenario);
        sub0pipeline::PriorityExecutor executor({.threadCount = width, .queueCapacity = width});
        sub0ecs::adapters::PipelinePool bridge(executor);
#endif
        // Two complete sixteen-tick churn cycles plus a tail.
        for (unsigned tick = 0; tick < 33; ++tick)
        {
            reference.tick(); sequential.tick();
            if (tick & 1) native.tick<64>(pool);
            else native.tick<1024>(pool);
            sameRows(reference, sequential); sameRows(reference, native);
#if defined(SUB0ECS_TEST_PIPELINE)
            if (tick & 1) bridged.tick<64>(bridge);
            else bridged.tick<1024>(bridge);
            sameRows(reference, bridged);
#endif
        }
    }
}

TEST_CASE("row workload arithmetic has independent analytic controls")
{
    bench::rows::Plain streaming(4, bench::rows::Scenario::Streaming);
    streaming.tick();
    CHECK(streaming.state()[3].x == 3.0 / 16.0 + 3.0 / 65536.0);
    CHECK(streaming.state()[3].y == 3.0 / 16.0 - 3.0 / 65536.0);
    bench::rows::Plain neighbors(2, bench::rows::Scenario::StagedNeighbors);
    neighbors.tick();
    CHECK(neighbors.state()[0].vx == 1.0 / 262144.0);
    CHECK(neighbors.state()[0].vy == 1.0 / 262144.0);
    CHECK(neighbors.state()[0].x == 1.0 / 16777216.0);
}
