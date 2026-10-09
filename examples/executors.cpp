/** Use when: deciding where a fused group runs: on the calling thread, in cache-
 *  sized tiles, across a thread pool, or staged to a device.
 *  Demonstrates: the four executors. Inline, Tiled<N>, Parallel and
 *  Offload<Device, N> (here the emulated device).
 *  Story: one Integrate system over 20,000 entities, run on each executor.
 *  Keep in mind: an executor may split rows any way it likes, as long as each row is
 *  processed exactly once; systems must therefore be row-local. Offload stages
 *  only the group's columns and copies back only those the systems declare as
 *  written (Access), and it only accepts device-safe systems. Parallel splits a
 *  partition only when it has enough rows to be worth a fork-join.
 *  Run: ctest --preset default -R Sub0ECS_Example_executors
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include "sub0ecs/sub0ecs.hpp"

namespace fz = sub0ecs::fusion;

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;
    using Access = fz::Access<fz::Write<Position>, fz::Read<Velocity>>;   // Velocity is never copied back
    static constexpr bool kDeviceSafe = true;                             // no host pointers or host state
    void operator()(Position& p, Velocity& v) const { p.x += v.dx; p.y += v.dy; }
};

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>>;
using World = sub0ecs::store::World<Queries>;

namespace
{
    constexpr std::size_t kEntities = 20'000;

    int failures = 0;
    void expect(bool ok, const char* what)
    {
        if (!ok)
        {
            ++failures;
            std::printf("FAILED: %s\n", what);
        }
    }

    /** Runs Integrate once on `exec` over a fresh world; true if every row was processed exactly once. */
    template <typename Exec>
    bool exactlyOnce(Exec& exec)
    {
        World world;
        std::vector<sub0ecs::Entity> es;
        for (std::size_t i = 0; i < kEntities; ++i)
            es.push_back(world.create(Position{ float(i), 0.0f }, Velocity{ 1.0f, 2.0f }));

        world.runFusedOn(exec, Integrate{});

        for (std::size_t i = 0; i < kEntities; ++i)
        {
            const Position* p = world.find<Position>(es[i]);
            if (p->x != float(i) + 1.0f || p->y != 2.0f) return false;   // skipped, or processed twice
        }
        return true;
    }
} // namespace

int main()
{
    fz::Inline inlineExec;
    expect(exactlyOnce(inlineExec), "Inline: one loop over each partition");

    fz::Tiled<1000> tiled;
    expect(exactlyOnce(tiled), "Tiled<1000>: fixed-size tiles, including the partial last one");

    fz::Parallel pool(4);
    expect(exactlyOnce(pool), "Parallel: contiguous row chunks across 4 threads");

    fz::Offload<fz::EmulatedDevice, 1024> offload;
    expect(exactlyOnce(offload), "Offload: staged tile by tile to the device");
    expect(offload.bytesIn() == kEntities * (sizeof(Position) + sizeof(Velocity)), "both columns are staged in");
    expect(offload.bytesOut() == kEntities * sizeof(Position), "only the written column is copied back");

    std::printf("offload moved %zu bytes in, %zu bytes out for %zu entities\n", offload.bytesIn(), offload.bytesOut(),
                kEntities);
    return failures == 0 ? 0 : 1;
}
