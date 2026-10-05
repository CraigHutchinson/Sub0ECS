/** Use when: results must not depend on how the frame was scheduled: lockstep
 *  multiplayer, replays, golden-file tests, or simply switching plans at runtime.
 *  Demonstrates: every legal (planner, executor) pair produces bit-identical state.
 *  Story: the same four frames run under three planners on four executors, and
 *  every result is compared, byte for byte, with plain sequential execution.
 *  Keep in mind: this holds because systems are row-local and the executors here
 *  share the host's floating-point behaviour. A real accelerator may not (fused
 *  multiply-add, different rounding), which is what the planned kBitExact executor
 *  capability will declare; a world that needs determinism would then reject
 *  executors without it. The emulated Offload device is bit-exact by construction.
 *  Run: ctest --preset default -R Sub0ECS_Example_determinism
 */
#include <cstddef>
#include <cstdio>
#include <cstring>
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
    void operator()(Position& p, Velocity& v) const { p.x += v.dx * 0.016f; p.y += v.dy * 0.016f; }
};
struct Gravity
{
    using Query = sub0ecs::Query<Position, Velocity>;
    static constexpr bool kDeviceSafe = true;
    void operator()(Position&, Velocity& v) const { v.dy -= 9.81f * 0.016f; v.dx *= 0.99f; }
};
struct Spin
{
    using Query = sub0ecs::Query<Rotation>;
    static constexpr bool kDeviceSafe = true;
    void operator()(Rotation& r) const { r.angle += 0.1f; }
};

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Rotation>>;
using World = sub0ecs::store::World<Queries>;

namespace
{
    constexpr int kEntities = 20'000;   // enough rows for Parallel to really split
    constexpr int kFrames = 4;

    std::vector<sub0ecs::Entity> populate(World& world)
    {
        std::vector<sub0ecs::Entity> es;
        for (int i = 0; i < kEntities; ++i)
        {
            const Position p{ float(i) * 0.37f, float(i % 91) };
            const Velocity v{ float(i % 13) - 6.0f, float(i % 7) };
            if (i % 2) es.push_back(world.create(p, v, Rotation{ float(i) * 0.01f }));
            else es.push_back(world.create(p, v));
        }
        return es;
    }

    /** The raw bytes of every component, in handle order: equality here is bit-identity. */
    std::vector<unsigned char> bytes(World& world, const std::vector<sub0ecs::Entity>& es)
    {
        std::vector<unsigned char> out;
        const auto append = [&](const void* data, std::size_t size) {
            const auto* first = static_cast<const unsigned char*>(data);
            out.insert(out.end(), first, first + size);
        };
        for (const sub0ecs::Entity e : es)
        {
            append(world.find<Position>(e), sizeof(Position));
            append(world.find<Velocity>(e), sizeof(Velocity));
            if (const Rotation* r = world.find<Rotation>(e)) append(r, sizeof(Rotation));
        }
        return out;
    }

    std::vector<unsigned char> sequential()
    {
        World world;
        const auto es = populate(world);
        for (int frame = 0; frame < kFrames; ++frame)
        {
            world.each<Position, Velocity>([](Position& p, Velocity& v) { Integrate{}(p, v); });
            world.each<Position, Velocity>([](Position& p, Velocity& v) { Gravity{}(p, v); });
            world.each<Rotation>([](Rotation& r) { Spin{}(r); });
        }
        return bytes(world, es);
    }

    template <typename Planner, typename Exec>
    std::vector<unsigned char> planned(Exec& exec)
    {
        World world;
        const auto es = populate(world);
        fz::Inline host;   // runs any group the executor may not take
        for (int frame = 0; frame < kFrames; ++frame) fz::runPlanned<Planner>(world, exec, host, Integrate{}, Gravity{}, Spin{});
        return bytes(world, es);
    }

    int failures = 0;
    int combinations = 0;

    template <typename Exec>
    void onEveryPlanner(const std::vector<unsigned char>& reference, Exec& exec, const char* name)
    {
        const bool same = planned<fz::NeverFuse, Exec>(exec) == reference &&
                          planned<fz::ShareColumns, Exec>(exec) == reference &&
                          planned<fz::AlwaysFuse, Exec>(exec) == reference;
        combinations += 3;
        std::printf("  %-8s x {NeverFuse, ShareColumns, AlwaysFuse}: %s\n", name, same ? "bit-identical" : "DIFFERS");
        if (!same) ++failures;
    }
} // namespace

int main()
{
    const std::vector<unsigned char> reference = sequential();

    fz::Inline inlineExec;
    fz::Tiled<1000> tiled;
    fz::Parallel pool(4);
    fz::Offload<fz::EmulatedDevice, 1024> offload;

    std::printf("%d entities, %d frames, compared with sequential execution:\n", kEntities, kFrames);
    onEveryPlanner(reference, inlineExec, "Inline");
    onEveryPlanner(reference, tiled, "Tiled");
    onEveryPlanner(reference, pool, "Parallel");
    onEveryPlanner(reference, offload, "Offload");

    std::printf("%d (planner, executor) pairs checked\n", combinations);
    return failures == 0 ? 0 : 1;
}
