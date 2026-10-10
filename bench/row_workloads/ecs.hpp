#pragma once
#include <algorithm>
#include <tuple>
#include "row_workloads/state.hpp"
#include "sub0ecs/store/world.hpp"
namespace bench::rows
{
/** Joined row updates followed by structural work; output is gathered by stable ID. */
class Ecs
{
public:
    Ecs(std::size_t size, Scenario scenario) : states_(size), tags_(size), snapshot_(size), scenario_(scenario)
    {
        world_.reserve(size);
        entities_.reserve(size);
        for (std::size_t i = 0; i < size; ++i)
        {
            auto entity = world_.create(initial(i));
            entities_.push_back(entity);
            tags_[i] = scenario == Scenario::FragmentedChurn ? unsigned(i & 7) : 0;
            if (scenario_ == Scenario::FragmentedChurn)
            {
                if (tags_[i] & 1) world_.add<A>(entity, A{});
                if (tags_[i] & 2) world_.add<B>(entity, B{});
                if (tags_[i] & 4) world_.add<C>(entity, C{});
            }
        }
    }
    void tick()
    {
        prepare();
        world_.each<State>([&](State& row) { update(row, scenario_, snapshot_); });
        finish();
    }
    template <std::size_t Grain = 1024, typename Pool>
    void tick(Pool& pool)
    {
        prepare();
        world_.eachParallel<State>(pool, [&](unsigned, State& row) { update(row, scenario_, snapshot_); },
                                   sub0ecs::store::RowGrain<Grain>{});
        finish();
    }
    const auto& state()
    {
        world_.each<State>([&](const State& row) { states_[row.id] = row; });
        return states_;
    }
    const auto& tags()
    {
        std::fill(tags_.begin(), tags_.end(), 0u);
        world_.each<State, A>([&](const State& row, const A&) { tags_[row.id] |= 1; });
        world_.each<State, B>([&](const State& row, const B&) { tags_[row.id] |= 2; });
        world_.each<State, C>([&](const State& row, const C&) { tags_[row.id] |= 4; });
        return tags_;
    }
    auto ticks() const { return ticks_; }
private:
    struct A {};
    struct B {};
    struct C {};
    using Queries = std::tuple<sub0ecs::Query<State>, sub0ecs::Query<State, A>,
                               sub0ecs::Query<State, B>, sub0ecs::Query<State, C>>;
    void prepare()
    {
        if (scenario_ == Scenario::StagedNeighbors)
            world_.each<State>([&](const State& row) { snapshot_[row.id] = {row.x, row.y}; });
    }
    void finish()
    {
        // All pool work is joined before migrations can invalidate borrowed rows.
        if (scenario_ == Scenario::FragmentedChurn)
        {
            for (std::size_t i = ticks_ % 16; i < entities_.size(); i += 16)
            {
                tags_[i] ^= 1;
                if (tags_[i] & 1) world_.add<A>(entities_[i], A{});
                else world_.remove<A>(entities_[i]);
            }
            world_.each<State, A>([](State& row, const A&) { row.vx += 1.0 / 1048576.0; });
            world_.each<State, B>([](State& row, const B&) { row.vy -= 1.0 / 1048576.0; });
            world_.each<State, C>([](State& row, const C&) { row.x += 1.0 / 1048576.0; });
        }
        ++ticks_;
    }
    sub0ecs::store::World<Queries> world_;
    std::vector<sub0ecs::Entity> entities_;
    std::vector<State> states_;
    std::vector<unsigned> tags_;
    std::vector<Point> snapshot_;
    Scenario scenario_;
    std::size_t ticks_{};
};
}
