/** Use when: you have several small systems and want them to run as few passes
 *  over the data as is worthwhile.
 *  Demonstrates: the four planners. NeverFuse (one pass per system), AlwaysFuse
 *  (one pass for all), ShareColumns (fuse while systems share a column: the
 *  recommended default) and DeviceAware<Base> (never mix device-safe and host-only
 *  systems in one group).
 *  Story: Integrate and Drag both touch Position/Velocity; Spin touches Rotation.
 *  Keep in mind: a plan is a compile-time value (checked here with static_assert),
 *  groups are runs of consecutive systems in schedule order, and every legal plan
 *  produces the same state, so choosing one is purely a performance decision.
 *  Fusing systems that share nothing (AlwaysFuse here) can be slower than not
 *  fusing: ShareColumns declines to.
 *  Run: ctest --preset default -R Sub0ECS_Example_fusion_planners
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include <sub0ecs/sub0ecs.hpp>

namespace fz = sub0ecs::fusion;

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Rotation { float angle = 0.0f; };

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;
    using Access = fz::Access<fz::Write<Position>, fz::Read<Velocity>>;
    static constexpr bool kDeviceSafe = true;
    void operator()(Position& p, Velocity& v) const { p.x += v.dx; p.y += v.dy; }
};

struct Drag   // host-only: declares no kDeviceSafe
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position&, Velocity& v) const { v.dx *= 0.5f; v.dy *= 0.5f; }
};

struct Spin
{
    using Query = sub0ecs::Query<Rotation>;
    static constexpr bool kDeviceSafe = true;
    void operator()(Rotation& r) const { r.angle += 0.25f; }
};

// ---- the plans, decided at compile time -----------------------------------
template <typename Planner>
using Plan = fz::Plan<Planner, Integrate, Drag, Spin>;

static_assert(Plan<fz::NeverFuse>::kGroups == 3, "one group per system");
static_assert(Plan<fz::AlwaysFuse>::kGroups == 1, "everything in one group");
static_assert(Plan<fz::ShareColumns>::kGroups == 2 && Plan<fz::ShareColumns>::groupOf[1] == 0 &&
                  Plan<fz::ShareColumns>::groupOf[2] == 1,
              "{Integrate, Drag} share Position/Velocity; {Spin} shares nothing");
static_assert(Plan<fz::DeviceAware<fz::ShareColumns>>::kGroups == 3,
              "host-only Drag is split from device-safe Integrate and Spin");

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Rotation>>;
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

    constexpr int kEntities = 3000;
    constexpr int kFrames = 4;

    std::vector<sub0ecs::Entity> populate(World& world)
    {
        std::vector<sub0ecs::Entity> es;
        for (int i = 0; i < kEntities; ++i)
        {
            const Position p{ float(i), float(i % 7) };
            const Velocity v{ float(i % 5), 1.0f };
            switch (i % 3)   // three shapes, so groups run over several partitions
            {
            case 0: es.push_back(world.create(p, v)); break;
            case 1: es.push_back(world.create(p, v, Rotation{ float(i) })); break;
            default: es.push_back(world.create(Rotation{ float(i) })); break;
            }
        }
        return es;
    }

    /** Every component value, in handle order. */
    std::vector<float> state(World& world, const std::vector<sub0ecs::Entity>& es)
    {
        std::vector<float> out;
        for (const sub0ecs::Entity e : es)
        {
            if (const Position* p = world.find<Position>(e)) out.insert(out.end(), { p->x, p->y });
            if (const Velocity* v = world.find<Velocity>(e)) out.insert(out.end(), { v->dx, v->dy });
            if (const Rotation* r = world.find<Rotation>(e)) out.push_back(r->angle);
        }
        return out;
    }

    /** The reference: one pass per system, written by hand. */
    std::vector<float> sequential()
    {
        World world;
        const auto es = populate(world);
        for (int frame = 0; frame < kFrames; ++frame)
        {
            world.each<Position, Velocity>([](Position& p, Velocity& v) { Integrate{}(p, v); });
            world.each<Position, Velocity>([](Position& p, Velocity& v) { Drag{}(p, v); });
            world.each<Rotation>([](Rotation& r) { Spin{}(r); });
        }
        return state(world, es);
    }

    template <typename Planner>
    std::vector<float> planned()
    {
        World world;
        const auto es = populate(world);
        fz::Inline exec;
        for (int frame = 0; frame < kFrames; ++frame) fz::runPlanned<Planner>(world, exec, exec, Integrate{}, Drag{}, Spin{});
        std::printf("  %-12s %d group(s)\n", Planner::kName, Plan<Planner>::kGroups);
        return state(world, es);
    }
} // namespace

int main()
{
    const std::vector<float> reference = sequential();
    std::printf("plans for {Integrate, Drag, Spin}:\n");
    expect(planned<fz::NeverFuse>() == reference, "NeverFuse equals sequential");
    expect(planned<fz::AlwaysFuse>() == reference, "AlwaysFuse equals sequential");
    expect(planned<fz::ShareColumns>() == reference, "ShareColumns equals sequential");
    expect(planned<fz::DeviceAware<fz::ShareColumns>>() == reference, "DeviceAware equals sequential");
    return failures == 0 ? 0 : 1;
}
