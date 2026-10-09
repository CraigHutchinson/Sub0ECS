/** Use when: a component is added and removed often (a status flag, a selection
 *  marker) and no system filters on it.
 *  Demonstrates: the Volatile<T> hint. A carried component is a dense column, so
 *  adding it moves the entity to another partition; a Volatile one lives in side
 *  storage, so churning it moves nothing.
 *  Story: 1,000 moving units. Stunning and un-stunning them leaves the layout
 *  untouched; giving one unit a Shield (carried) creates a new partition.
 *  Keep in mind: Volatile only changes where the data lives. find(), add() and
 *  remove() behave the same either way, and a component a query requires is
 *  always a column, hint or not.
 *  Run: ctest --preset default -R Sub0ECS_Example_hinted_partitions
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include "sub0ecs/sub0ecs.hpp"

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Armor { int value = 0; };     // unqueried, stable: carried as a dense column
struct Shield { int value = 0; };    // unqueried, stable: carried as a dense column
struct Stunned { int ticks = 0; };   // unqueried, churns: declared Volatile

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>>;
using World = sub0ecs::store::World<Queries, sub0ecs::store::Volatile<Stunned>>;

namespace
{
    int failures = 0;
    void expect(bool ok, const char* what)
    {
        if (!ok)
        {
            ++failures;
            std::printf("FAILED: %s\n", what);
        }
    }

    std::size_t moving(World& world)
    {
        std::size_t n = 0;
        world.each<Position, Velocity>([&](Position&, Velocity&) { ++n; });
        return n;
    }
} // namespace

int main()
{
    World world;
    std::vector<sub0ecs::Entity> units;
    for (int i = 0; i < 1000; ++i)
        units.push_back(world.create(Position{ float(i), 0.0f }, Velocity{ 1.0f, 0.0f }, Armor{ i }));

    const std::size_t partitions = world.partitionCount();

    // Volatile churn: side storage only. No entity changes partition.
    for (int round = 0; round < 3; ++round)
    {
        for (std::size_t i = 0; i < units.size(); i += 3) world.add(units[i], Stunned{ round });
        expect(world.partitionCount() == partitions, "stunning units creates no partition");
        expect(world.find<Stunned>(units[0]) && world.find<Stunned>(units[0])->ticks == round, "stunned value is stored");
        for (std::size_t i = 0; i < units.size(); i += 3) world.remove<Stunned>(units[i]);
    }
    expect(world.partitionCount() == partitions, "un-stunning units creates no partition");
    expect(world.find<Stunned>(units[0]) == nullptr, "stun removed");

    // A carried component is part of the layout: adding it moves the entity.
    world.add(units[0], Shield{ 7 });
    expect(world.partitionCount() == partitions + 1, "a carried component creates a partition for its new shape");
    expect(world.find<Shield>(units[0])->value == 7, "shield stored");
    expect(world.find<Armor>(units[0])->value == 0, "other components survive the move");
    expect(world.find<Position>(units[0])->x == 0.0f, "position survives the move");

    // Either way the query sees every unit, in whole partitions of dense columns.
    expect(moving(world) == 1000, "the query still matches all 1000 units");

    std::printf("%zu units, %zu partitions after stun churn and one shield\n", world.size(), world.partitionCount());
    return failures == 0 ? 0 : 1;
}
