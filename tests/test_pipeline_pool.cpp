#include <atomic>
#include <functional>
#include <stdexcept>
#include <vector>

#include <doctest/doctest.h>
#include "sub0pipeline/executor/priority_executor.hpp"
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0ecs/store/world.hpp"

namespace
{
class Deferred final : public sub0pipeline::IExecutor
{
public:
    int rejectAt{-1};
    int submitted{};
    int completions{};
    std::vector<std::function<void()>> pending;
    void dispatch(std::string_view, std::function<void()> body, std::function<void()> done, int, uint8_t, uint32_t) override
    {
        if (submitted++ == rejectAt) throw std::runtime_error("injected rejection");
        pending.push_back([this, body = std::move(body), done = std::move(done)] { body(); done(); ++completions; });
    }
    void waitAll() override
    {
        auto work = std::move(pending);
        pending.clear();
        for (auto& task : work) task();
    }
    int concurrency() const noexcept override { return 4; }
};
struct Value { std::size_t id; int visits{}; };
}

TEST_CASE("Pipeline adapter joins partial submission and is reusable")
{
    Deferred executor;
    sub0ecs::adapters::PipelinePool pool(executor);
    int calls{};
    auto body = [&](std::size_t, unsigned) { ++calls; };
    executor.rejectAt = 2;
    CHECK_THROWS_AS(pool.parallelFor(12, body), std::runtime_error);
    CHECK(calls == 6);
    CHECK(executor.completions == 2);
    CHECK(executor.pending.empty());
    executor.rejectAt = -1;
    pool.parallelFor(5, body);
    CHECK(calls == 11);
}

TEST_CASE("Pipeline adapter catches body failure and rejects reentry")
{
    Deferred executor;
    sub0ecs::adapters::PipelinePool pool(executor);
    auto failure = [](std::size_t, unsigned lane) { if (lane == 1) throw std::runtime_error("body"); };
    CHECK_THROWS_AS(pool.parallelFor(9, failure), std::runtime_error);
    CHECK(executor.completions == 4);
    auto noop = [](std::size_t, unsigned) {};
    auto nested = [&](std::size_t, unsigned) { pool.parallelFor(1, noop); };
    CHECK_THROWS_AS(pool.parallelFor(1, nested), std::logic_error);
    pool.parallelFor(1, noop);
    CHECK_THROWS_AS(sub0ecs::adapters::PipelinePool(executor, 5), std::invalid_argument);
}

TEST_CASE("Pipeline adapter supplies exclusive lanes and visits ECS tails exactly once")
{
    sub0pipeline::PriorityExecutor executor({.threadCount = 4, .queueCapacity = 4});
    sub0ecs::adapters::PipelinePool pool(executor);
    std::atomic<unsigned> overlap{};
    std::atomic<unsigned> active[4]{};
    std::vector<unsigned> visits(103);
    auto body = [&](std::size_t item, unsigned lane) {
        if (active[lane].fetch_add(1) != 0) ++overlap;
        ++visits[item];
        active[lane].fetch_sub(1);
    };
    for (auto size : {0u, 1u, 3u, 103u})
    {
        std::fill(visits.begin(), visits.end(), 0);
        pool.parallelFor(size, body);
        for (std::size_t i = 0; i < visits.size(); ++i) CHECK(visits[i] == (i < size ? 1u : 0u));
    }
    CHECK(overlap == 0);
    sub0ecs::store::World<std::tuple<sub0ecs::Query<Value>>> world;
    for (std::size_t i = 0; i < 4099; ++i) world.create(Value{i});
    world.eachParallel<Value>(pool, [](unsigned, Value& value) { ++value.visits; });
    world.each<Value>([](const Value& value) { CHECK(value.visits == 1); });
}
