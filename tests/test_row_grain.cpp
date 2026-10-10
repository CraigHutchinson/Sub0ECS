#include <cstddef>
#include <tuple>
#include <type_traits>
#include <doctest/doctest.h>
#include "sub0ecs/store/world.hpp"
namespace
{
struct GrainValue { unsigned visits{}; };
struct GrainTag {};
struct CountingPool
{
    std::size_t items{};
    unsigned concurrency() const { return 2; }
    template <typename F>
    void parallelFor(std::size_t count, F& fn)
    {
        items += count;
        for (std::size_t i = 0; i < count; ++i) fn(i, unsigned(i % 2));
    }
};
}
TEST_CASE_TEMPLATE("explicit row grain preserves fragmented rows and inline threshold", Mode, std::true_type, std::false_type)
{
    using Queries = std::tuple<sub0ecs::Query<GrainValue>, sub0ecs::Query<GrainValue, GrainTag>>;
    sub0ecs::store::BasicWorld<Mode::value, Queries> world;
    CountingPool pool;
    auto visit = [](unsigned lane, GrainValue& value) { CHECK(lane < 2); ++value.visits; };
    world.template eachParallel<GrainValue>(pool, visit, sub0ecs::store::RowGrain<4>{});
    CHECK(pool.items == 0);
    for (unsigned i = 0; i < 9; ++i) world.create(GrainValue{});
    for (unsigned i = 0; i < 5; ++i) world.create(GrainValue{}, GrainTag{});
    world.template eachParallel<GrainValue>(pool, visit);
    CHECK(pool.items == 0);
    world.template eachParallel<GrainValue>(pool, visit, sub0ecs::store::RowGrain<4>{});
    CHECK(pool.items == 5);
    world.template each<GrainValue>([](const GrainValue& value) { CHECK(value.visits == 2); });
}
