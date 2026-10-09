/** Use when: a system arrives at runtime (a new world region, a loaded mod, a debug
 *  tool) and needs a component the layout keeps in side storage.
 *  Demonstrates: addQuery (register a query at runtime), eachDyn (iterate it), and
 *  migrateStep (move a bounded number of entities per frame into the new layout).
 *  Story: fire spreads. Burning starts as a Volatile side-stored flag; then a
 *  fire system is paged in that needs Position and Burning together.
 *  Keep in mind: the new query works immediately, in a degraded mode (a join over
 *  the side pool) that visits every holder exactly once but is slower than dense
 *  columns. Each migrateStep(budget) call moves at most `budget` entities into
 *  columns, so the relayout never stalls a frame; migrateStep(SIZE_MAX) does it
 *  all at once when a stall is acceptable (a loading screen). Dynamic queries
 *  need the carry-mode World.
 *  Run: ctest --preset default -R Sub0ECS_Example_dynamic_systems
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include "sub0ecs/sub0ecs.hpp"

struct Position { float x = 0.0f, y = 0.0f; };
struct Burning { int ticks = 0; };

using Queries = std::tuple<sub0ecs::Query<Position>>;                              // what is known up front
using World = sub0ecs::store::World<Queries, sub0ecs::store::Volatile<Burning>>;   // Burning: side storage

namespace
{
    constexpr int kTrees = 1000;
    constexpr std::size_t kBurning = kTrees / 2;
    constexpr std::size_t kBudgetPerFrame = 100;

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
    std::vector<sub0ecs::Entity> trees;
    for (int i = 0; i < kTrees; ++i)
    {
        trees.push_back(world.create(Position{ float(i), 0.0f }));
        if (i % 2 == 0) world.add(trees.back(), Burning{});
    }

    // The fire system arrives: it needs Position and Burning in the same row.
    const auto fire = world.addQuery<Position, Burning>();   // a typed handle: DynamicQuery<Position, Burning>
    expect(world.queryDegraded(fire), "the new query starts in degraded mode");
    expect(world.pendingMigration() == kBurning, "every burning tree is waiting to migrate");

    int frames = 0;
    int degradedFrames = 0;
    bool everyHolderOncePerFrame = true;
    const auto frame = [&] {
        std::size_t visited = 0;
        world.eachDyn(fire, [&](Position& p, Burning& b) {
            ++b.ticks;
            p.y += 1.0f;
            ++visited;
        });
        everyHolderOncePerFrame = everyHolderOncePerFrame && visited == kBurning;
        ++frames;
    };

    // Keep simulating while the relayout proceeds, a bounded slice per frame.
    while (world.queryDegraded(fire))
    {
        frame();
        ++degradedFrames;
        world.migrateStep(kBudgetPerFrame);
    }
    expect(degradedFrames >= static_cast<int>(kBurning / kBudgetPerFrame), "migration was spread over several frames");
    expect(world.pendingMigration() == 0, "nothing is left in side storage");

    // Fully migrated: the same call now runs over dense columns.
    for (int i = 0; i < 3; ++i) frame();

    expect(everyHolderOncePerFrame, "every burning tree was visited exactly once in every frame");
    bool consistent = true;
    for (int i = 0; i < kTrees; ++i)
    {
        const Burning* b = world.find<Burning>(trees[i]);
        const Position* p = world.find<Position>(trees[i]);
        if (i % 2 == 0) consistent = consistent && b != nullptr && b->ticks == frames && p->y == float(frames);
        else consistent = consistent && b == nullptr && p->y == 0.0f;
    }
    expect(consistent, "state is the same as if the layout had never changed");

    std::printf("%zu burning trees migrated over %d frames (%zu per frame), %d frames simulated\n", kBurning, degradedFrames,
                kBudgetPerFrame, frames);
    return failures == 0 ? 0 : 1;
}
