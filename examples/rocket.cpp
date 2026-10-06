/** Use when: you want the classic "first game loop": a few entities, two systems,
 *  something drawn each frame.
 *  Demonstrates: a frame loop with a movement system and a render system that
 *  reads a different pair of components, drawing to a text grid.
 *  Story: three rockets fly across a 40x8 terminal at different speeds; one of them
 *  has no Velocity and stays parked.
 *  Keep in mind: a system only sees entities that have every component its query
 *  names, so the parked rocket is drawn but never moved. The render system writes
 *  to its own buffer, not to the world, so it could run alongside other readers.
 *  Run: ctest --preset default -R Sub0ECS_Example_rocket
 */
#include <array>
#include <cstdio>
#include <string>
#include <tuple>

#include <sub0ecs/sub0ecs.hpp>

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Sprite { char symbol = '?'; };

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>,   // movement
                           sub0ecs::Query<Position, Sprite>>;    // rendering
using World = sub0ecs::store::World<Queries>;

namespace
{
    constexpr int kWidth = 40;
    constexpr int kHeight = 8;
    using Screen = std::array<std::string, kHeight>;

    void move(World& world, float dt)
    {
        world.each<Position, Velocity>([dt](Position& p, Velocity& v) {
            p.x += v.dx * dt;
            p.y += v.dy * dt;
        });
    }

    Screen render(World& world)
    {
        Screen screen;
        screen.fill(std::string(kWidth, '.'));
        world.each<Position, Sprite>([&](Position& p, Sprite& s) {
            const int column = static_cast<int>(p.x);
            const int row = static_cast<int>(p.y);
            if (column >= 0 && column < kWidth && row >= 0 && row < kHeight)
                screen[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)] = s.symbol;
        });
        return screen;
    }
} // namespace

int main()
{
    World world;
    world.create(Position{ 0.0f, 1.0f }, Velocity{ 2.0f, 0.0f }, Sprite{ '>' });   // fast
    world.create(Position{ 0.0f, 3.0f }, Velocity{ 1.0f, 0.0f }, Sprite{ '=' });   // slow
    world.create(Position{ 0.0f, 5.0f }, Velocity{ 1.0f, 0.25f }, Sprite{ '/' });  // climbing
    world.create(Position{ 5.0f, 7.0f }, Sprite{ '#' });                           // parked: no Velocity

    Screen screen = render(world);
    for (int frame = 0; frame < 8; ++frame)
    {
        move(world, 1.0f);
        screen = render(world);
    }
    for (const std::string& line : screen) std::printf("%s\n", line.c_str());

    // After 8 frames: fast at x=16, slow at x=8, climbing at (8, 7), parked unmoved at (5, 7).
    int failures = 0;
    const auto expect = [&](bool ok, const char* what) {
        if (!ok)
        {
            ++failures;
            std::printf("FAILED: %s\n", what);
        }
    };
    expect(screen[1][16] == '>', "the fast rocket reached column 16");
    expect(screen[3][8] == '=', "the slow rocket reached column 8");
    expect(screen[7][8] == '/', "the climbing rocket reached row 7");
    expect(screen[7][5] == '#', "the parked rocket never moved");
    return failures == 0 ? 0 : 1;
}
