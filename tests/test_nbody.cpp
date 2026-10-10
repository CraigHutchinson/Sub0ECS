#include <bit>
#include <cstdint>
#include <doctest/doctest.h>
#include "sub0ecs/fusion/executors/parallel.hpp"
#include "nbody/simulation.hpp"
#if defined(SUB0ECS_TEST_PIPELINE)
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0pipeline/executor/priority_executor.hpp"
#endif
namespace
{
void same(const std::vector<bench::nbody::Body>& a, const std::vector<bench::nbody::Body>& b)
{
    REQUIRE(a.size() == b.size());
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        const double left[]{a[i].position.x, a[i].position.y, a[i].position.z, a[i].velocity.x, a[i].velocity.y, a[i].velocity.z, a[i].mass.value};
        const double right[]{b[i].position.x, b[i].position.y, b[i].position.z, b[i].velocity.x, b[i].velocity.y, b[i].velocity.z, b[i].mass.value};
        for (unsigned field = 0; field < 7; ++field)
            CHECK(std::bit_cast<std::uint64_t>(left[field]) == std::bit_cast<std::uint64_t>(right[field]));
    }
}
}
TEST_CASE("nbody softened two-body force and empty single worlds")
{
    using namespace bench::nbody;
    std::vector<Body> pair{{{0, 0, 0}, {}, {1}}, {{1, 0, 0}, {}, {1}}};
    const auto a = advanceBody(pair, 0), b = advanceBody(pair, 1);
    const double expected = 0.01 / (1.0625 * std::sqrt(1.0625)) / 1024.0;
    CHECK(a.velocity.x == expected);
    CHECK(b.velocity.x == -expected);
    CHECK(a.velocity.y == 0);
    CHECK(a.position.x == expected / 1024.0);
    for (auto count : {0u, 1u, 17u})
    {
        Plain reference(count); Ecs world(count);
        for (int tick = 0; tick < 4; ++tick) { reference.tick(); world.tick(); same(reference.state(), world.state()); }
    }
}
TEST_CASE("nbody pools preserve ordered evolving state including ECS chunk tail" * doctest::test_suite("exhaustive"))
{
    bench::nbody::Plain reference(4097);
    bench::nbody::Ecs native(4097);
    bench::nbody::CompactEcs compact(4097);
    bench::nbody::CompactPlain compactHand(4097);
    sub0ecs::fusion::Parallel pool(2);
#if defined(SUB0ECS_TEST_PIPELINE)
    bench::nbody::Ecs bridged(4097);
    bench::nbody::CompactEcs compactBridged(4097);
    sub0pipeline::PriorityExecutor executor({.threadCount = 2, .queueCapacity = 2});
    sub0ecs::adapters::PipelinePool bridge(executor);
#endif
    for (int tick = 0; tick < 2; ++tick)
    {
        reference.tick(); native.tick(pool); same(reference.state(), native.state());
        compact.tick<64>(pool); compactHand.tick<64>(pool);
        same(reference.state(), compact.state()); same(reference.state(), compactHand.state());
#if defined(SUB0ECS_TEST_PIPELINE)
        bridged.tick(bridge); same(reference.state(), bridged.state());
        compactBridged.tick<64>(bridge); same(reference.state(), compactBridged.state());
#endif
    }
}

TEST_CASE("nbody explicit grain exposes expensive small worlds to both pools")
{
    for (const unsigned workers : {1u, 2u, 4u})
    {
        bench::nbody::Plain reference(257);
        bench::nbody::Ecs native(257);
        bench::nbody::Plain handNative(257);
        sub0ecs::fusion::Parallel pool(workers);
#if defined(SUB0ECS_TEST_PIPELINE)
        bench::nbody::Ecs bridged(257);
        bench::nbody::Plain handPipeline(257);
        sub0pipeline::PriorityExecutor executor({.threadCount = workers, .queueCapacity = workers});
        sub0ecs::adapters::PipelinePool bridge(executor);
#endif
        for (int tick = 0; tick < 3; ++tick)
        {
            reference.tick();
            native.tick<64>(pool);
            handNative.tick<64>(pool);
            same(reference.state(), handNative.state());
            same(reference.state(), native.state());
#if defined(SUB0ECS_TEST_PIPELINE)
            bridged.tick<64>(bridge);
            handPipeline.tick<64>(bridge);
            same(reference.state(), handPipeline.state());
            same(reference.state(), bridged.state());
#endif
        }
    }
}

TEST_CASE("compact force snapshot preserves ordered state across pool widths")
{
    for (auto count : {0u, 1u, 17u, 257u})
    {
        for (auto width : {1u, 2u, 4u})
        {
            bench::nbody::Plain reference(count);
            bench::nbody::CompactEcs sequential(count), native(count);
            bench::nbody::CompactPlain hand(count), handNative(count);
            sub0ecs::fusion::Parallel pool(width);
#if defined(SUB0ECS_TEST_PIPELINE)
            bench::nbody::CompactEcs bridged(count);
            bench::nbody::CompactPlain handPipeline(count);
            sub0pipeline::PriorityExecutor executor({.threadCount = width, .queueCapacity = width});
            sub0ecs::adapters::PipelinePool bridge(executor);
#endif
            for (unsigned tick = 0; tick < 4; ++tick)
            {
                reference.tick(); sequential.tick(); native.tick<64>(pool);
                hand.tick(); handNative.tick<64>(pool);
                same(reference.state(), hand.state());
                same(reference.state(), handNative.state());
                same(reference.state(), sequential.state());
                same(reference.state(), native.state());
#if defined(SUB0ECS_TEST_PIPELINE)
                bridged.tick<64>(bridge);
                handPipeline.tick<64>(bridge);
                same(reference.state(), handPipeline.state());
                same(reference.state(), bridged.state());
#endif
            }
        }
    }
}
