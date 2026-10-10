/** Isolate row-dispatch bookkeeping from worker wakeups and kernel cost.
 * HandWritten streams equivalent columns directly. ECS each and eachParallel
 * apply identical unsigned arithmetic; InlinePool executes every chunk on the
 * coordinator so these numbers make no thread-scaling claim.
 */
#include <cstdint>
#include <memory>
#include <string>
#include <stdexcept>
#include <tuple>
#include <vector>
#include "harness/harness.hpp"
#include "sub0ecs/store/world.hpp"

namespace
{
struct Value { std::uint64_t value{}; };
struct Step { std::uint64_t value{3}; };
struct Tag {};
struct InlinePool
{
    unsigned concurrency() const { return 2; }
    template <typename F>
    void parallelFor(std::size_t count, F& fn)
    {
        for (std::size_t i = 0; i < count; ++i) fn(i, 0u);
    }
};
struct Plain
{
    std::vector<Value> values[2];
    std::vector<Step> steps[2];
    Plain(std::size_t n, bool fragmented)
    {
        for (std::size_t i = 0; i < n; ++i)
        {
            const auto part = fragmented ? i % 2 : 0;
            values[part].push_back({i});
            steps[part].push_back({3});
        }
    }
    void tick()
    {
        for (unsigned p = 0; p < 2; ++p)
            for (std::size_t i = 0; i < values[p].size(); ++i) values[p][i].value += steps[p][i].value;
    }
    double checksum()
    {
        std::uint64_t sum = 0;
        for (const auto& part : values) for (const auto& v : part) sum += v.value;
        return double(sum);
    }
};
template <std::size_t Rows>
struct Ecs
{
    sub0ecs::store::World<std::tuple<sub0ecs::Query<Value, Step>>> world;
    InlinePool pool;
    Ecs(std::size_t n, bool fragmented)
    {
        world.reserve(n);
        for (std::size_t i = 0; i < n; ++i)
        {
            if (fragmented && i % 2) world.create(Value{i}, Step{}, Tag{});
            else world.create(Value{i}, Step{});
        }
    }
    void tick()
    {
        if constexpr (Rows == 0)
            world.template each<Value, Step>([](Value& v, const Step& s) { v.value += s.value; });
        else
            world.template eachParallel<Value, Step>(pool,
                [](unsigned, Value& v, const Step& s) { v.value += s.value; }, sub0ecs::store::RowGrain<Rows>{});
    }
    double checksum()
    {
        std::uint64_t sum = 0;
        world.template each<Value, Step>([&](const Value& v, const Step&) { sum += v.value; });
        return double(sum);
    }
};
template <typename Simulation>
bench::harness::Prepared prepare(std::size_t n, bool fragmented)
{
    auto simulation = std::make_shared<Simulation>(n, fragmented);
    Plain reference(n, fragmented);
    for (unsigned tick = 0; tick < 3; ++tick)
    {
        reference.tick();
        simulation->tick();
        if (simulation->checksum() != reference.checksum()) throw std::runtime_error("row dispatch conformance");
    }
    bench::harness::Prepared result;
    result.fixture = simulation;
    result.op = [simulation] { simulation->tick(); };
    result.finish = [simulation](bench::harness::Record& r) { r.counter("checksum", simulation->checksum()); };
    return result;
}
template <typename Simulation>
void add(bench::harness::Registry& registry, const char* name, std::size_t n, bool fragmented)
{
    registry.add({"RowDispatch", fragmented ? "Fragmented" : "Coherent", name,
        static_cast<std::int64_t>(n), 1, "pass", false, 0,
        [=] { return prepare<Simulation>(n, fragmented); }});
}
}
int main(int argc, char** argv)
{
    bench::harness::Registry registry;
    for (const auto n : {64u, 4096u, 100000u})
        for (const bool fragmented : {false, true})
        {
            add<Plain>(registry, "HandWritten", n, fragmented);
            add<Ecs<0>>(registry, "Each", n, fragmented);
            add<Ecs<1>>(registry, "Grain1", n, fragmented);
            add<Ecs<64>>(registry, "Grain64", n, fragmented);
            add<Ecs<1024>>(registry, "Grain1024", n, fragmented);
        }
    return bench::harness::benchMain(argc, argv, registry, "sub0ecs_rows_bench");
}
