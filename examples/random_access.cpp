/** Use when: you hold an entity handle and need one of its components (a target, a
 *  parent, a UI selection), outside any system's iteration.
 *  Demonstrates: find<T>(entity) for column components, side-stored components and
 *  absent ones, has<T>(), and what a structural change does to a pointer.
 *  Story: a turret looks up its target's position, then the target changes shape.
 *  Keep in mind: find() is one indexed load for a column component (the entity's
 *  record gives partition and row), or a sparse-set lookup for a side-stored one.
 *  The pointer is valid until the next structural change to that entity's
 *  partition, so re-find after add/remove/destroy rather than keeping it.
 *  Run: ctest --preset default -R Sub0ECS_Example_random_access
 */
#include <cstdio>
#include <tuple>

#include <sub0ecs/sub0ecs.hpp>

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Target { sub0ecs::Entity entity; };   // a handle stored in a component
struct Marked { int by = 0; };               // Volatile: side storage

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Position, Target>>;
using World = sub0ecs::store::World<Queries, sub0ecs::store::Volatile<Marked>>;

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
} // namespace

int main()
{
    World world;
    const sub0ecs::Entity drone = world.create(Position{ 3.0f, 4.0f }, Velocity{ 1.0f, 0.0f });
    const sub0ecs::Entity turret = world.create(Position{ 0.0f, 0.0f }, Target{ drone });

    // Column component: present and absent.
    expect(world.find<Position>(drone) && world.find<Position>(drone)->x == 3.0f, "find a column component");
    expect(world.find<Target>(drone) == nullptr && !world.has<Target>(drone), "absent component: nullptr");

    // Follow a handle stored in a component.
    // Copy the handle out: it stays meaningful across structural changes, a pointer does not.
    const sub0ecs::Entity target = world.find<Target>(turret)->entity;
    expect(world.alive(target), "the turret has a live target");
    expect(world.find<Position>(target)->y == 4.0f, "look the target's position up through the handle");

    // Side-stored component: same call, different storage.
    world.add(drone, Marked{ 42 });
    expect(world.find<Marked>(drone) && world.find<Marked>(drone)->by == 42, "find a side-stored component");
    expect(world.find<Marked>(turret) == nullptr, "side-stored component absent on another entity");

    // A structural change moves the entity: re-find, do not keep the pointer.
    world.remove<Velocity>(drone);
    const Position* moved = world.find<Position>(drone);
    expect(moved && moved->x == 3.0f && moved->y == 4.0f, "the value survives the move; find it again");
    expect(world.find<Marked>(drone)->by == 42, "side storage is unaffected by the move");

    // A destroyed target: the stored handle goes stale instead of dangling.
    world.destroy(drone);
    expect(!world.alive(target) && world.find<Position>(target) == nullptr, "a stale target handle finds nothing");

    std::printf("turret target alive: %s\n", world.alive(target) ? "yes" : "no");
    return failures == 0 ? 0 : 1;
}
