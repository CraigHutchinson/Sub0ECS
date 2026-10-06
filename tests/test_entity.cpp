/** Entity handles and the generational allocator. */
#include <cstdint>
#include <set>
#include <vector>

#include <sub0ecs/entity.hpp>

#include <doctest/doctest.h>

using sub0ecs::Entity;
using sub0ecs::EntityAllocator;
using sub0ecs::kNullEntity;

TEST_CASE("Entity: a default handle is the null handle")
{
    constexpr Entity e{};
    static_assert(e == kNullEntity);
    CHECK(e.value == ~0u);
    CHECK(e.index() == Entity::kIndexMask);
    CHECK(e.version() == 0xFFu);
}

TEST_CASE("Entity: index and version round-trip through the 24/8-bit packing")
{
    constexpr Entity e = Entity::make(12345, 7);
    static_assert(e.index() == 12345 && e.version() == 7);

    const Entity maxIndex = Entity::make(Entity::kIndexMask, 0);
    CHECK(maxIndex.index() == (1u << 24) - 1u);
    CHECK(maxIndex.version() == 0);

    // index bits beyond 24 are masked off rather than bleeding into the version
    CHECK(Entity::make(Entity::kIndexMask + 5u, 3).index() == 4u);
    CHECK(Entity::make(Entity::kIndexMask + 5u, 3).version() == 3u);
}

TEST_CASE("Entity: equality compares index and version")
{
    const Entity a = Entity::make(1, 0);
    const Entity copy = a;
    Entity assigned;
    assigned = a;
    CHECK(copy == a);
    CHECK(assigned == a);
    CHECK_FALSE(Entity::make(1, 1) == a);
    CHECK_FALSE(Entity::make(2, 0) == a);
}

TEST_CASE("EntityAllocator: starts empty")
{
    EntityAllocator ids;
    CHECK(ids.slots() == 0);
    CHECK(ids.liveCount() == 0);
    CHECK_FALSE(ids.alive(kNullEntity));
    CHECK_FALSE(ids.alive(Entity::make(0, 0)));
}

TEST_CASE("EntityAllocator: fresh slots are sequential from zero at version zero")
{
    EntityAllocator ids;
    for (std::uint32_t i = 0; i < 100; ++i)
    {
        const Entity e = ids.create();
        CHECK(e.index() == i);
        CHECK(e.version() == 0);
        CHECK(ids.alive(e));
    }
    CHECK(ids.slots() == 100);
    CHECK(ids.liveCount() == 100);
}

TEST_CASE("EntityAllocator: release makes the handle stale and recycles the slot with a new version")
{
    EntityAllocator ids;
    const Entity a = ids.create();
    const Entity b = ids.create();
    ids.release(a);
    CHECK_FALSE(ids.alive(a));
    CHECK(ids.alive(b));
    CHECK(ids.liveCount() == 1);
    CHECK(ids.slots() == 2);

    const Entity reused = ids.create();
    CHECK(reused.index() == a.index());
    CHECK(reused.version() == a.version() + 1);
    CHECK(ids.alive(reused));
    CHECK_FALSE(ids.alive(a));   // the old handle stays stale after reuse
    CHECK(ids.slots() == 2);
}

TEST_CASE("EntityAllocator: freed slots are reused (LIFO) before new slots are opened")
{
    EntityAllocator ids;
    std::vector<Entity> es;
    for (int i = 0; i < 10; ++i) es.push_back(ids.create());
    ids.release(es[2]);
    ids.release(es[7]);
    ids.release(es[4]);
    CHECK(ids.liveCount() == 7);
    CHECK(ids.create().index() == 4u);
    CHECK(ids.create().index() == 7u);
    CHECK(ids.create().index() == 2u);
    CHECK(ids.create().index() == 10u);   // free list exhausted: a new slot
    CHECK(ids.liveCount() == 11);
}

TEST_CASE("EntityAllocator: alloc/free/alloc keeps every live handle unique")
{
    EntityAllocator ids;
    std::vector<Entity> live;
    std::set<std::uint32_t> seen;
    for (int round = 0; round < 50; ++round)
    {
        for (int i = 0; i < 20; ++i) live.push_back(ids.create());
        for (std::size_t i = round % 3; i < live.size(); i += 3)
        {
            ids.release(live[i]);
            live[i] = kNullEntity;
        }
        std::erase(live, kNullEntity);
    }
    for (const Entity& e : live)
    {
        CHECK(ids.alive(e));
        CHECK(seen.insert(e.index()).second);   // no two live handles share a slot
    }
    CHECK(ids.liveCount() == live.size());
}

TEST_CASE("EntityAllocator: the 8-bit version wraps after 256 reuses of one slot")
{
    // Documents the handle's ABA horizon: a handle kept across 256 reuses of
    // its slot compares alive again. 24/8 bits trades this for 16M slots.
    EntityAllocator ids;
    const Entity first = ids.create();
    Entity e = first;
    for (int i = 0; i < 255; ++i)
    {
        ids.release(e);
        e = ids.create();
        CHECK_FALSE(ids.alive(first));
    }
    CHECK(e.version() == 255u);
    ids.release(e);
    CHECK(ids.create() == first);
}

TEST_SUITE("exhaustive")
{
    TEST_CASE("EntityAllocator: hands out kMaxEntities slots, then the null handle, never a duplicate index")
    {
        // Past 2^24 - 1 slots an index would wrap and alias a live entity.
        EntityAllocator ids;
        ids.reserve(Entity::kMaxEntities);
        Entity last = kNullEntity;
        bool allValid = true;
        for (std::uint32_t i = 0; i < Entity::kMaxEntities; ++i)
        {
            last = ids.create();
            allValid = allValid && last.index() == i && !(last == kNullEntity);
        }
        CHECK(allValid);
        CHECK(ids.liveCount() == Entity::kMaxEntities);
        CHECK(ids.create() == kNullEntity);            // full: no wrapped index
        CHECK(ids.liveCount() == Entity::kMaxEntities);

        ids.release(last);                             // a freed slot can be handed out again
        const Entity again = ids.create();
        CHECK(again.index() == last.index());
        CHECK(ids.alive(again));
        CHECK_FALSE(ids.alive(last));
        CHECK(ids.create() == kNullEntity);
    }
}
