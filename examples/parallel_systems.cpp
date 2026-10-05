/** Use when: a system has enough entities to be worth several threads, and may
 *  need to make structural changes as a result.
 *  Demonstrates: eachParallel (one system, data-parallel), runFusedParallel (a fused
 *  group, data-parallel) and per-worker command buffers for the structural changes.
 *  Story: 20,000 units take damage in parallel; the ones that die are recorded per
 *  worker and destroyed at a commit point after the pass. Then movement runs fused
 *  and parallel.
 *  Keep in mind: a parallel pass must not change structure (add, remove, destroy):
 *  record the change and apply it serially afterwards. The worker index passed to
 *  the callback gives each thread its own buffer without locks. Order within a pass
 *  is unspecified, so anything order-sensitive (here: which units die) must depend
 *  only on the entity's own data.
 *  Run: ctest --preset default -R Sub0ECS_Example_parallel_systems
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include <sub0ecs/sub0ecs.hpp>

namespace fz = sub0ecs::fusion;

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Health { int value = 0; };
struct Self { sub0ecs::Entity entity; };   // the handle, so a parallel pass can name its entity

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position& p, Velocity& v) const { p.x += v.dx; p.y += v.dy; }
};
struct Drag
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position&, Velocity& v) const { v.dx *= 0.5f; }
};

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Self, Health>>;
using World = sub0ecs::store::World<Queries>;

namespace
{
    constexpr int kUnits = 20'000;

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
    std::vector<sub0ecs::Entity> units;
    for (int i = 0; i < kUnits; ++i)
    {
        // health 1..4: after one point of damage, every unit that started at 1 dies
        const sub0ecs::Entity e = world.create(Position{ float(i), 0.0f }, Velocity{ 2.0f, 1.0f }, Health{ 1 + i % 4 }, Self{});
        world.find<Self>(e)->entity = e;
        units.push_back(e);
    }

    fz::Parallel pool(4);

    // ---- one system, data-parallel, with per-worker command buffers ----------
    std::vector<std::vector<sub0ecs::Entity>> doomed(pool.concurrency());
    world.eachParallel<Self, Health>(pool, [&](unsigned worker, Self& self, Health& health) {
        health.value -= 1;
        if (health.value <= 0) doomed[worker].push_back(self.entity);   // record; never destroy inside the pass
    });

    // ---- the commit point: apply the recorded changes serially ---------------
    std::size_t destroyed = 0;
    for (const auto& buffer : doomed)
        for (const sub0ecs::Entity e : buffer)
        {
            world.destroy(e);
            ++destroyed;
        }

    expect(destroyed == static_cast<std::size_t>(kUnits / 4), "exactly the units that started with 1 health died");
    expect(world.size() == static_cast<std::size_t>(kUnits) - destroyed, "the survivors remain");
    bool damagedOnce = true;
    for (int i = 0; i < kUnits; ++i)
    {
        const Health* h = world.find<Health>(units[i]);
        if (i % 4 == 0) damagedOnce = damagedOnce && h == nullptr;                // died, handle stale
        else damagedOnce = damagedOnce && h != nullptr && h->value == i % 4;      // damaged exactly once
    }
    expect(damagedOnce, "every unit was damaged exactly once, whichever thread ran it");

    // ---- a fused group, data-parallel ---------------------------------------
    world.runFusedParallel(pool, Integrate{}, Drag{});
    bool movedOnce = true;
    for (int i = 0; i < kUnits; ++i)
    {
        if (i % 4 == 0) continue;
        const Position* p = world.find<Position>(units[i]);
        const Velocity* v = world.find<Velocity>(units[i]);
        movedOnce = movedOnce && p->x == float(i) + 2.0f && p->y == 1.0f && v->dx == 1.0f;
    }
    expect(movedOnce, "the fused parallel pass moved and damped every survivor exactly once");

    std::printf("%d units, %zu destroyed at the commit point, %u threads\n", kUnits, destroyed, pool.concurrency());
    return failures == 0 ? 0 : 1;
}
