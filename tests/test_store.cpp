/** The store (sub0ecs::store::BasicWorld): per-operation semantics in both
 * storage modes, handle rules, and a model-based churn test that checks every
 * observable against a trivially correct reference after random operations.
 */
#include <array>
#include <cstring>
#include <cstdint>
#include <optional>
#include <random>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include <sub0ecs/fusion/executors.hpp>
#include <sub0ecs/store.hpp>

#include "../bench/common/components.hpp"
#include <doctest/doctest.h>

using namespace bench;   // components: Position, Velocity, Health, Rotation, Scale, Frozen
using sub0ecs::Entity;
using sub0ecs::kNullEntity;
using sub0ecs::Query;
using sub0ecs::store::BasicWorld;
using sub0ecs::store::Volatile;

namespace
{
    // Queried: Position, Velocity, Health, Rotation. Unqueried: Scale (a dense
    // column in carry mode, side storage in pure mode), Frozen (Volatile in carry mode).
    using Queries = std::tuple<Query<Position>, Query<Position, Velocity>, Query<Health, Rotation>>;
    using PureWorld = BasicWorld<false, Queries>;
    using HintedWorld = sub0ecs::store::World<Queries, Volatile<Frozen>>;   // the recommended model

    template <typename W, typename... Cs>
    std::size_t count(W& w)
    {
        std::size_t n = 0;
        w.template each<Cs...>([&](Cs&...) { ++n; });
        return n;
    }
} // namespace

#define WORLDS PureWorld, HintedWorld

TEST_CASE_TEMPLATE("store: an empty world", W, WORLDS)
{
    W w;
    CHECK(w.size() == 0);
    CHECK(count<W, Position>(w) == 0);
    CHECK_FALSE(w.alive(kNullEntity));
    CHECK(w.template find<Position>(kNullEntity) == nullptr);
    CHECK(w.template find<Position>(Entity::make(0, 0)) == nullptr);
}

TEST_CASE_TEMPLATE("store: create stores every component and reports membership", W, WORLDS)
{
    W w;
    const Entity e = w.create(Position{ 1, 2 }, Velocity{ 3, 4 }, Scale{ 5 });
    CHECK(w.alive(e));
    CHECK(w.size() == 1);
    REQUIRE(w.template find<Position>(e));
    CHECK(w.template find<Position>(e)->x == 1);
    CHECK(w.template find<Position>(e)->y == 2);
    CHECK(w.template find<Velocity>(e)->dy == 4);
    CHECK(w.template find<Scale>(e)->value == 5);
    CHECK(w.template has<Position>(e));
    CHECK_FALSE(w.template has<Health>(e));
    CHECK_FALSE(w.template has<Frozen>(e));

    const Entity bare = w.create();
    CHECK(w.alive(bare));
    CHECK_FALSE(w.template has<Position>(bare));
    CHECK(w.size() == 2);
}

TEST_CASE_TEMPLATE("store: each visits exactly the entities matching the declared query", W, WORLDS)
{
    W w;
    for (int i = 0; i < 10; ++i) w.create(Position{ float(i), 0 });                        // Position only
    for (int i = 0; i < 20; ++i) w.create(Position{ float(i), 0 }, Velocity{ 1, 0 });      // both
    for (int i = 0; i < 5; ++i) w.create(Velocity{ 1, 0 });                                // Velocity only: matches nothing
    for (int i = 0; i < 7; ++i) w.create(Health{ 1 }, Rotation{ 0 }, Scale{ 2 });           // other query
    for (int i = 0; i < 3; ++i) w.create(Health{ 1 });                                      // partial match: excluded

    CHECK(count<W, Position>(w) == 30);
    CHECK(count<W, Position, Velocity>(w) == 20);
    CHECK(count<W, Health, Rotation>(w) == 7);
}

TEST_CASE_TEMPLATE("store: writes through each are visible to find", W, WORLDS)
{
    W w;
    std::vector<Entity> es;
    for (int i = 0; i < 100; ++i) es.push_back(w.create(Position{ float(i), 0 }, Velocity{ 2, 3 }));
    w.template each<Position, Velocity>([](Position& p, Velocity& v) { p.x += v.dx; p.y += v.dy; });
    for (int i = 0; i < 100; ++i)
    {
        CHECK(w.template find<Position>(es[i])->x == float(i) + 2);
        CHECK(w.template find<Position>(es[i])->y == 3);
    }
}

TEST_CASE_TEMPLATE("store: add makes an entity match a query; remove makes it stop", W, WORLDS)
{
    W w;
    const Entity e = w.create(Position{ 1, 1 }, Scale{ 9 });
    CHECK(count<W, Position, Velocity>(w) == 0);

    w.add(e, Velocity{ 5, 6 });
    CHECK(count<W, Position, Velocity>(w) == 1);
    CHECK(w.template find<Velocity>(e)->dx == 5);
    CHECK(w.template find<Position>(e)->x == 1);   // values survive the partition move
    CHECK(w.template find<Scale>(e)->value == 9);

    w.template remove<Velocity>(e);
    CHECK(count<W, Position, Velocity>(w) == 0);
    CHECK(count<W, Position>(w) == 1);
    CHECK(w.template find<Velocity>(e) == nullptr);
    CHECK(w.template find<Position>(e)->x == 1);
    CHECK(w.template find<Scale>(e)->value == 9);
}

TEST_CASE_TEMPLATE("store: add of a component the entity already has overwrites it", W, WORLDS)
{
    // One case per storage kind: queried column, unqueried (column or side
    // storage by mode), Volatile side storage, and a never-queried pool.
    W w;
    const Entity e = w.create(Position{ 1, 1 }, Velocity{ 1, 1 }, Scale{ 1 }, Frozen{ 1 }, Health{ 1 });
    const Entity other = w.create(Position{ 7, 7 }, Scale{ 7 }, Frozen{ 7 }, Health{ 7 });
    for (int round = 2; round < 5; ++round)
    {
        w.add(e, Position{ float(round), 0 });
        w.add(e, Scale{ float(round) });
        w.add(e, Frozen{ round });
        w.add(e, Health{ float(round) });
    }
    CHECK(w.template find<Position>(e)->x == 4);
    CHECK(w.template find<Scale>(e)->value == 4);
    CHECK(w.template find<Frozen>(e)->ticks == 4);
    CHECK(w.template find<Health>(e)->value == 4);
    CHECK(count<W, Position>(w) == 2);
    CHECK(count<W, Position, Velocity>(w) == 1);

    // A single remove must remove it: overwriting never left a second entry behind.
    w.template remove<Frozen>(e);
    w.template remove<Scale>(e);
    w.template remove<Health>(e);
    CHECK(w.template find<Frozen>(e) == nullptr);
    CHECK(w.template find<Scale>(e) == nullptr);
    CHECK(w.template find<Health>(e) == nullptr);
    CHECK(w.template find<Frozen>(other)->ticks == 7);
    CHECK(w.template find<Scale>(other)->value == 7);
    CHECK(w.template find<Health>(other)->value == 7);
}

TEST_CASE_TEMPLATE("store: remove of a component the entity lacks is a no-op", W, WORLDS)
{
    W w;
    const Entity e = w.create(Position{ 1, 2 });
    const Entity f = w.create(Position{ 3, 4 }, Frozen{ 1 }, Scale{ 1 });
    w.template remove<Velocity>(e);   // queried, absent
    w.template remove<Frozen>(e);     // Volatile/unqueried, absent, pool exists
    w.template remove<Scale>(e);      // unqueried, absent
    w.template remove<Rotation>(e);   // queried type this world has never stored
    CHECK(w.template find<Position>(e)->y == 2);
    CHECK(w.template find<Frozen>(f)->ticks == 1);
    CHECK(w.template find<Scale>(f)->value == 1);
    CHECK(count<W, Position>(w) == 2);
}

TEST_CASE_TEMPLATE("store: destroy removes the entity and keeps the others' values", W, WORLDS)
{
    W w;
    std::vector<Entity> es;
    for (int i = 0; i < 200; ++i) es.push_back(w.create(Position{ float(i), 0 }, Velocity{ 0, float(i) }, Frozen{ i }));
    w.destroy(es[0]);    // swap-remove moves the last row into row 0
    w.destroy(es[57]);
    CHECK_FALSE(w.alive(es[0]));
    CHECK(w.template find<Position>(es[0]) == nullptr);
    CHECK(w.template find<Frozen>(es[0]) == nullptr);
    CHECK(w.size() == 198);
    CHECK(count<W, Position, Velocity>(w) == 198);
    for (int i = 1; i < 200; ++i)
    {
        if (i == 57) continue;
        CHECK(w.template find<Position>(es[i])->x == float(i));
        CHECK(w.template find<Velocity>(es[i])->dy == float(i));
        CHECK(w.template find<Frozen>(es[i])->ticks == i);
    }
}

TEST_CASE_TEMPLATE("store: operations on a stale handle are no-ops", W, WORLDS)
{
    W w;
    const Entity e = w.create(Position{ 1, 1 });
    w.destroy(e);
    w.destroy(e);                        // a second destroy must not release the slot twice
    w.add(e, Velocity{});
    w.template remove<Position>(e);
    CHECK(w.size() == 0);
    CHECK_FALSE(w.alive(e));

    const Entity a = w.create(Position{ 2, 2 });
    const Entity b = w.create(Position{ 3, 3 });
    CHECK(a.index() != b.index());       // a double release would hand out the slot twice
    CHECK(a.index() == e.index());
    CHECK(w.template find<Position>(e) == nullptr);   // the recycled slot is not visible via the old handle
    CHECK(w.template find<Position>(a)->x == 2);
    CHECK(w.size() == 2);
}

TEST_CASE_TEMPLATE("store: worlds are independent", W, WORLDS)
{
    W a, b;
    const Entity ea = a.create(Position{ 1, 0 }, Frozen{ 1 });
    const Entity eb = b.create(Position{ 2, 0 }, Frozen{ 2 });
    CHECK(ea == eb);   // same slot in each world's own allocator
    b.add(eb, Velocity{});
    a.destroy(ea);
    CHECK_FALSE(a.alive(ea));
    CHECK(b.alive(eb));
    CHECK(b.template find<Position>(eb)->x == 2);
    CHECK(b.template find<Frozen>(eb)->ticks == 2);
    CHECK(count<W, Position, Velocity>(a) == 0);
    CHECK(count<W, Position, Velocity>(b) == 1);
}

TEST_CASE("store: churn of a non-queried component moves no data")
{
    SUBCASE("carry mode: a Volatile component lives in side storage")
    {
        HintedWorld w;
        std::vector<Entity> es;
        for (int i = 0; i < 1000; ++i) es.push_back(w.create(Position{}, Velocity{}));
        const std::size_t partitions = w.partitionCount();
        for (int round = 0; round < 3; ++round)
        {
            for (std::size_t i = 0; i < es.size(); i += 3) w.add(es[i], Frozen{ round });
            for (std::size_t i = 0; i < es.size(); i += 3) w.template remove<Frozen>(es[i]);
        }
        CHECK(w.partitionCount() == partitions);
    }
    SUBCASE("pure mode: every unqueried component lives in side storage")
    {
        PureWorld w;
        std::vector<Entity> es;
        for (int i = 0; i < 1000; ++i) es.push_back(w.create(Position{}, Velocity{}));
        const std::size_t partitions = w.partitionCount();
        for (std::size_t i = 0; i < es.size(); i += 2) w.add(es[i], Scale{ 2 });
        for (std::size_t i = 0; i < es.size(); i += 4) w.template remove<Scale>(es[i]);
        CHECK(w.partitionCount() == partitions);
        CHECK(w.template find<Scale>(es[2])->value == 2);
    }
}

TEST_CASE_TEMPLATE("store: eachParallel visits every matching entity exactly once", W, WORLDS)
{
    W w;
    constexpr int kN = 50'000;   // enough chunks for a real fork-join
    std::vector<Entity> es;
    for (int i = 0; i < kN; ++i) es.push_back(w.create(Position{ 0, 0 }, Velocity{ 1, 0 }));
    for (int i = 0; i < 1000; ++i) w.create(Position{ 0, 0 });   // not matching
    sub0ecs::fusion::Parallel pool(4);
    std::array<std::size_t, 4> perWorker{};
    w.template eachParallel<Position, Velocity>(pool, [&](unsigned worker, Position& p, Velocity& v) {
        p.x += v.dx;
        ++perWorker[worker];   // each worker writes only its own slot
    });
    std::size_t total = 0;
    for (std::size_t n : perWorker) total += n;
    CHECK(total == kN);
    for (const Entity& e : es) CHECK(w.template find<Position>(e)->x == 1);
}

namespace
{
    struct Move
    {
        using Query = sub0ecs::Query<Position, Velocity>;
        void operator()(Position& p, Velocity& v) const { p.x += v.dx; }
    };
    struct Spin
    {
        using Query = sub0ecs::Query<Health, Rotation>;
        void operator()(Health& h, Rotation& r) const { r.angle += h.value; }
    };
} // namespace

TEST_CASE_TEMPLATE("store: runFused applies each system to exactly the entities it matches", W, WORLDS)
{
    W w;
    const Entity moving = w.create(Position{ 0, 0 }, Velocity{ 2, 0 });
    const Entity spinning = w.create(Health{ 3 }, Rotation{ 0 });
    const Entity both = w.create(Position{ 0, 0 }, Velocity{ 5, 0 }, Health{ 1 }, Rotation{ 0 });
    const Entity still = w.create(Position{ 0, 0 });
    w.runFused(Move{}, Spin{});
    w.runFused(Move{}, Spin{});
    CHECK(w.template find<Position>(moving)->x == 4);
    CHECK(w.template find<Rotation>(spinning)->angle == 6);
    CHECK(w.template find<Position>(both)->x == 10);
    CHECK(w.template find<Rotation>(both)->angle == 2);
    CHECK(w.template find<Position>(still)->x == 0);
}

// ---- model-based churn -------------------------------------------------------

namespace
{
    /** The reference: per-handle optional values, nothing clever. */
    struct Expected
    {
        std::optional<Position> p;
        std::optional<Velocity> v;
        std::optional<Health> h;
        std::optional<Rotation> r;
        std::optional<Scale> s;
        std::optional<Frozen> f;
    };

    template <typename W>
    struct Harness
    {
        W w;
        std::vector<Entity> handles;            // every handle ever created (live or stale)
        std::vector<std::optional<Expected>> model;   // parallel to handles; nullopt = destroyed
        std::mt19937 rng;

        explicit Harness(std::uint32_t seed = 12345) : rng(seed) {}

        std::uint32_t pick(std::uint32_t n) { return static_cast<std::uint32_t>(rng() % n); }
        float value() { return static_cast<float>(pick(1000)); }   // exact in float

        std::size_t liveIndex()
        {
            for (;;)
            {
                const std::size_t i = pick(static_cast<std::uint32_t>(handles.size()));
                if (model[i]) return i;
            }
        }

        void create()
        {
            Expected x;
            const std::uint32_t mask = pick(64);
            if (mask & 1) x.p = Position{ value(), value() };
            if (mask & 2) x.v = Velocity{ value(), value() };
            if (mask & 4) x.h = Health{ value() };
            if (mask & 8) x.r = Rotation{ value() };
            if (mask & 16) x.s = Scale{ value() };
            if (mask & 32) x.f = Frozen{ static_cast<std::int32_t>(pick(1000)) };
            // create takes the components by value; add the optional ones after
            const Entity e = w.create();
            if (x.p) w.add(e, *x.p);
            if (x.v) w.add(e, *x.v);
            if (x.h) w.add(e, *x.h);
            if (x.r) w.add(e, *x.r);
            if (x.s) w.add(e, *x.s);
            if (x.f) w.add(e, *x.f);
            handles.push_back(e);
            model.push_back(x);
        }

        template <typename C>
        void toggle(std::size_t i, std::optional<C>& slot, C value)
        {
            if (slot && pick(2)) { w.template remove<C>(handles[i]); slot.reset(); }
            else { w.add(handles[i], value); slot = value; }   // also exercises overwrite
        }

        void step()
        {
            const std::uint32_t op = pick(100);
            const bool any = w.size() > 0;
            if (op < 25 || !any) { create(); return; }
            const std::size_t i = liveIndex();
            Expected& x = *model[i];
            if (op < 35) { w.destroy(handles[i]); model[i].reset(); }
            else if (op < 40) { w.destroy(handles[pick(static_cast<std::uint32_t>(handles.size()))]); }   // maybe stale
            else if (op < 50) toggle(i, x.p, Position{ value(), value() });
            else if (op < 60) toggle(i, x.v, Velocity{ value(), value() });
            else if (op < 67) toggle(i, x.h, Health{ value() });
            else if (op < 74) toggle(i, x.r, Rotation{ value() });
            else if (op < 85) toggle(i, x.s, Scale{ value() });
            else if (op < 97) toggle(i, x.f, Frozen{ static_cast<std::int32_t>(pick(1000)) });
            else
            {
                w.template each<Position, Velocity>([](Position& p, Velocity& v) { p.x += v.dx; });
                for (auto& m : model)
                    if (m && m->p && m->v) m->p->x += m->v->dx;
            }
            // destroy() above may have hit a handle whose model entry we must retire
            for (std::size_t k = 0; k < handles.size(); ++k)
                if (model[k] && !w.alive(handles[k])) model[k].reset();
        }

        template <typename C>
        void expectComponent(Entity e, const std::optional<C>& want)
        {
            const C* got = w.template find<C>(e);
            REQUIRE((got != nullptr) == want.has_value());
            if (got) CHECK(std::memcmp(got, &*want, sizeof(C)) == 0);
        }

        void verify()
        {
            std::size_t live = 0, pos = 0, posVel = 0, healthRot = 0;
            for (std::size_t k = 0; k < handles.size(); ++k)
            {
                const Entity e = handles[k];
                REQUIRE(w.alive(e) == model[k].has_value());
                if (!model[k])
                {
                    CHECK(w.template find<Position>(e) == nullptr);
                    continue;
                }
                const Expected& x = *model[k];
                ++live;
                pos += x.p.has_value();
                posVel += x.p && x.v;
                healthRot += x.h && x.r;
                expectComponent(e, x.p);
                expectComponent(e, x.v);
                expectComponent(e, x.h);
                expectComponent(e, x.r);
                expectComponent(e, x.s);
                expectComponent(e, x.f);
            }
            CHECK(w.size() == live);
            CHECK(count<W, Position>(w) == pos);
            CHECK(count<W, Position, Velocity>(w) == posVel);
            CHECK(count<W, Health, Rotation>(w) == healthRot);
        }
    };
} // namespace

namespace
{
    /** `batches` x 250 random operations from `seed`, every observable checked after each batch. */
    template <typename W>
    void randomWalk(std::uint32_t seed, int batches)
    {
        CAPTURE(seed);
        Harness<W> h(seed);
        for (int batch = 0; batch < batches; ++batch)
        {
            for (int i = 0; i < 250; ++i) h.step();
            h.verify();
        }
        CHECK(h.w.size() > 100);   // the walk really built up a population (growth beyond first capacity)
    }
} // namespace

// Gated sample: one seed, 10,000 operations per storage mode.
TEST_CASE_TEMPLATE("store: random structural churn matches a reference model", W, WORLDS)
{
    randomWalk<W>(12345, 40);
}

// On demand (ctest -L exhaustive): many seeds, longer walks.
TEST_SUITE("exhaustive")
{
    TEST_CASE_TEMPLATE("store: random structural churn, many seeds and long walks", W, WORLDS)
    {
        for (std::uint32_t seed = 1; seed <= 24; ++seed) randomWalk<W>(seed, 200);
    }
}

// ---- per-World-type component indices ------------------------------------------

namespace
{
    template <int N, int Family>
    struct Many { std::int32_t value = 0; };

    template <int Family, int... N>
    void addAll(auto& w, Entity e, std::integer_sequence<int, N...>)
    {
        (w.add(e, Many<N, Family>{ N }), ...);
    }

    template <int Family, int... N>
    bool allPresent(auto& w, Entity e, std::integer_sequence<int, N...>)
    {
        return ((w.template find<Many<N, Family>>(e) && w.template find<Many<N, Family>>(e)->value == N) && ...);
    }

    /** Adds Many<Offset..Offset+count-1, Family> in that order, each holding its own number. */
    template <int Family, int Offset, int... N>
    void addFrom(auto& w, Entity e, std::integer_sequence<int, N...>)
    {
        (w.add(e, Many<N + Offset, Family>{ N + Offset }), ...);
    }

    // Each world type numbers its own component types. A family is used by one world
    // type only, so its runtime numbering is exactly the order the test adds in.
    using WorldA = BasicWorld<true, std::tuple<Query<Many<0, 1>>>>;
    using WorldB = BasicWorld<true, std::tuple<Query<Many<0, 2>>>>;
    using CarryManyWorld = BasicWorld<true, std::tuple<Query<Many<0, 3>>>, Volatile<Many<1, 3>>>;
    using PureManyWorld = BasicWorld<false, std::tuple<Query<Many<0, 4>>>>;
} // namespace

TEST_CASE("store: each World type has its own 64 layout bits, independent of the rest of the process")
{
    // 128 component types in total, 64 per world type and every one a column, on
    // top of every type the other tests and the benchmark designs already use. A
    // process-wide id would have exhausted a 64-bit mask long before this.
    constexpr auto kSeq = std::make_integer_sequence<int, 64>{};
    WorldA a;
    WorldB b;
    const Entity ea = a.create();
    const Entity eb = b.create();
    addAll<1>(a, ea, kSeq);
    addAll<2>(b, eb, kSeq);
    CHECK(allPresent<1>(a, ea, kSeq));
    CHECK(allPresent<2>(b, eb, kSeq));

    std::size_t n = 0;
    a.template each<Many<0, 1>>([&](Many<0, 1>&) { ++n; });
    CHECK(n == 1);

    WorldA second;   // instances of one World type share its numbering
    const Entity e2 = second.create(Many<5, 1>{ 5 }, Many<0, 1>{ 0 });
    CHECK(second.template find<Many<5, 1>>(e2)->value == 5);
    CHECK(second.template find<Many<7, 1>>(e2) == nullptr);
}

TEST_CASE("store: component types beyond the 64 layout bits are side-stored, not refused")
{
    CarryManyWorld w;
    const Entity e = w.create();
    const Entity other = w.create(Many<0, 3>{ 0 });
    const std::size_t before = w.partitionCount();

    // Many<0> is queried and Many<1> Volatile: both numbered at compile time (0 and 1).
    // Many<2>..Many<63> are first seen here and take the remaining 62 layout bits, so
    // each is carried as a column and moves e to a partition of its own shape.
    addFrom<3, 0>(w, e, std::make_integer_sequence<int, 64>{});
    CHECK(w.partitionCount() == before + 62);

    // 136 more types: no layout bit is left, so they go to side storage and move nothing.
    const std::size_t full = w.partitionCount();
    addFrom<3, 64>(w, e, std::make_integer_sequence<int, 136>{});
    CHECK(w.partitionCount() == full);
    CHECK(allPresent<3>(w, e, std::make_integer_sequence<int, 200>{}));

    std::size_t matched = 0;
    w.template each<Many<0, 3>>([&](Many<0, 3>&) { ++matched; });
    CHECK(matched == 2);

    // A side-stored overflow type behaves like any other component.
    w.add(e, Many<150, 3>{ -1 });
    CHECK(w.template find<Many<150, 3>>(e)->value == -1);
    w.template remove<Many<150, 3>>(e);
    CHECK(w.template find<Many<150, 3>>(e) == nullptr);
    CHECK(w.template find<Many<151, 3>>(e)->value == 151);
    CHECK(w.template find<Many<151, 3>>(other) == nullptr);

    // Destroy scrubs every pool, so the entity that reuses the slot starts clean.
    w.destroy(e);
    CHECK(w.size() == 1);
    const Entity again = w.create(Many<199, 3>{ 7 });
    CHECK(again.index() == e.index());
    CHECK(w.template find<Many<199, 3>>(again)->value == 7);
    CHECK(w.template find<Many<198, 3>>(again) == nullptr);
    CHECK(w.template find<Many<5, 3>>(again) == nullptr);
}

TEST_CASE("store: in pure mode unqueried types need no layout bit, so their number is unlimited")
{
    PureManyWorld w;
    const Entity e = w.create(Many<0, 4>{ 0 });
    const std::size_t partitions = w.partitionCount();
    addFrom<4, 1>(w, e, std::make_integer_sequence<int, 199>{});
    CHECK(w.partitionCount() == partitions);   // nothing but the queried component shapes the layout
    CHECK(allPresent<4>(w, e, std::make_integer_sequence<int, 200>{}));

    PureManyWorld second;   // another instance of the type shares its numbering, not its data
    const Entity e2 = second.create(Many<0, 4>{ 0 }, Many<120, 4>{ 120 });
    CHECK(second.template find<Many<120, 4>>(e2)->value == 120);
    CHECK(second.template find<Many<121, 4>>(e2) == nullptr);
}
