/** Use when: starting a project with the smallest useful world.
 *  Demonstrates: declaring a query, creating an entity, and running its system.
 *  Story: integrate one position by its velocity.
 *  Keep in mind: declare every query before constructing the World type.
 *  Run: ctest --preset default -R Sub0ECS_Example_minimal_world
 */
#include <tuple>

#include <sub0ecs/sub0ecs.hpp>

struct Position
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity
{
    float dx = 0.0f;
    float dy = 0.0f;
};

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>>;
using World = sub0ecs::store::World<Queries>;

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;

    void operator()(Position& position, Velocity& velocity) const
    {
        position.x += velocity.dx;
        position.y += velocity.dy;
    }
};

int main()
{
    World world;
    const sub0ecs::Entity entity = world.create(Position{}, Velocity{ 1.0f, 2.0f });
    world.runFused(Integrate{});

    const Position* position = world.find<Position>(entity);
    if (position == nullptr || position->x != 1.0f || position->y != 2.0f)
        return 1;

    return 0;
}
