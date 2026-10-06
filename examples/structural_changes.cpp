/** Use when: entities gain and lose components, or are destroyed, during play.
 *  Demonstrates: add (and overwrite), remove, destroy, and what a stale handle does.
 *  Story: a unit picks up a weapon, upgrades it, drops it, and dies; its slot is
 *  then reused by a new unit while the old handle stays safely dead.
 *  Keep in mind: handles are generational. Every operation on a destroyed entity's
 *  handle is a no-op and find() returns nullptr, so a stale handle does not reach
 *  the entity that reused its slot (until that slot has been reused 256 times: see
 *  sub0ecs/entity.hpp). Pointers from find() are invalidated by the next
 *  structural change. Changes apply immediately; there is no commit step.
 *  Run: ctest --preset default -R Sub0ECS_Example_structural_changes
 */
#include <cstddef>
#include <cstdio>
#include <tuple>

#include <sub0ecs/sub0ecs.hpp>

struct Position { float x = 0.0f, y = 0.0f; };
struct Weapon { int damage = 0; };

using Queries = std::tuple<sub0ecs::Query<Position>, sub0ecs::Query<Position, Weapon>>;
using World = sub0ecs::store::World<Queries>;

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

    std::size_t armed(World& world)
    {
        std::size_t n = 0;
        world.each<Position, Weapon>([&](Position&, Weapon&) { ++n; });
        return n;
    }
} // namespace

int main()
{
    World world;
    const sub0ecs::Entity unit = world.create(Position{ 1.0f, 2.0f });
    const sub0ecs::Entity bystander = world.create(Position{ 9.0f, 9.0f });
    expect(world.size() == 2 && armed(world) == 0, "two unarmed units");

    // add: the unit now matches the armed query.
    world.add(unit, Weapon{ 5 });
    expect(world.has<Weapon>(unit) && armed(world) == 1, "picking up a weapon joins the armed query");
    expect(world.find<Position>(unit)->x == 1.0f, "other components keep their values");

    // add again: overwrites, never duplicates.
    world.add(unit, Weapon{ 9 });
    expect(world.find<Weapon>(unit)->damage == 9 && armed(world) == 1, "adding again overwrites the value");

    // remove: leaves the query; removing what is not there is a no-op.
    world.remove<Weapon>(unit);
    world.remove<Weapon>(unit);
    world.remove<Weapon>(bystander);
    expect(!world.has<Weapon>(unit) && armed(world) == 0, "dropping the weapon leaves the armed query");

    // destroy: the handle goes stale, and every use of it is harmless.
    world.destroy(unit);
    expect(!world.alive(unit) && world.size() == 1, "destroyed");
    expect(world.find<Position>(unit) == nullptr, "find on a stale handle returns nullptr");
    world.destroy(unit);            // no-op: the slot is not released twice
    world.add(unit, Weapon{ 1 });   // no-op
    world.remove<Position>(unit);   // no-op
    expect(world.size() == 1 && armed(world) == 0, "operations on a stale handle change nothing");

    // The slot is reused under a new version, so the old handle still refers to nothing.
    const sub0ecs::Entity recruit = world.create(Position{ 4.0f, 4.0f });
    expect(recruit.index() == unit.index() && recruit.version() != unit.version(), "slot reused with a new version");
    expect(world.alive(recruit) && !world.alive(unit), "only the new handle is alive");
    expect(world.find<Position>(unit) == nullptr, "the stale handle cannot see the recruit");
    expect(world.find<Position>(bystander)->x == 9.0f, "the bystander was never disturbed");

    std::printf("%zu live entities; stale handle index %u version %u, recruit version %u\n", world.size(),
                static_cast<unsigned>(unit.index()), static_cast<unsigned>(unit.version()),
                static_cast<unsigned>(recruit.version()));
    return failures == 0 ? 0 : 1;
}
