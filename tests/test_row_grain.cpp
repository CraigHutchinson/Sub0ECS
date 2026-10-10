#include <cstddef>
#include <tuple>
#include <type_traits>
#include <vector>
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

TEST_CASE_TEMPLATE("single partition arithmetic chunks preserve lanes tails and structural changes", Mode,
                   std::true_type, std::false_type)
{
    using Queries = std::tuple<sub0ecs::Query<GrainValue>, sub0ecs::Query<GrainValue, GrainTag>>;
    sub0ecs::store::BasicWorld<Mode::value, Queries> world;
    CountingPool pool;
    std::vector<sub0ecs::Entity> entities;
    auto visit = [](unsigned lane, GrainValue& value) { value.visits += lane + 1; };
    for (const unsigned n : {1u, 12u, 13u, 16u, 17u})
    {
        while (entities.size() < n) entities.push_back(world.create(GrainValue{}));
        world.template each<GrainValue>([](GrainValue& value) { value.visits = 0; });
        pool.items = 0;
        world.template eachParallel<GrainValue>(pool, visit, sub0ecs::store::RowGrain<4>{});
        const auto count = (n + 3u) / 4u;
        CHECK(pool.items == (count < 4 ? 0 : count));
        for (unsigned i = 0; i < n; ++i)
            CHECK(world.template find<GrainValue>(entities[i])->visits == (count < 4 ? 1u : (i / 4u) % 2u + 1u));
    }
    // A new matching partition must use fresh ranges; then retain an empty one.
    world.template add<GrainTag>(entities.back(), {});
    world.template each<GrainValue>([](GrainValue& value) { value.visits = 0; });
    pool.items = 0;
    world.template eachParallel<GrainValue>(pool, visit, sub0ecs::store::RowGrain<4>{});
    CHECK(pool.items == 5);
    world.template each<GrainValue>([](const GrainValue& value) { CHECK(value.visits >= 1); CHECK(value.visits <= 2); });
    world.destroy(entities.back());
    pool.items = 0;
    world.template eachParallel<GrainValue>(pool, visit, sub0ecs::store::RowGrain<4>{});
    CHECK(pool.items == 4);
}
