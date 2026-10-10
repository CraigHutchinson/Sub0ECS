#pragma once
#include <tuple>
#include "nbody/body.hpp"
#include "sub0ecs/store/world.hpp"
namespace bench::nbody
{
/** ECS application with a stable-ID snapshot and joined authoritative row writes.
 * No structural mutation takes place during a tick. Pool failures are fail-stop:
 * rows may have been modified, so a failed tick must not be published/retried as
 * though the state were transactional. This benchmark uses nonthrowing kernels.
 */
class Ecs
{
public:
    explicit Ecs(std::size_t size) : input_(size)
    {
        world_.reserve(size);
        for (std::size_t i = 0; i < size; ++i)
        {
            const auto body = initial(i);
            world_.create(body.position, body.velocity, body.mass, Id{i});
        }
    }
    void gather()
    {
        world_.each<Position, Velocity, Mass, Id>([&](const Position& p, const Velocity& v, const Mass& m, const Id& id) {
            input_[id.value] = {p, v, m};
        });
    }
    void tick()
    {
        gather();
        world_.each<Position, Velocity, Mass, Id>([&](Position& p, Velocity& v, const Mass&, const Id& id) {
            const auto body = advanceBody(input_, id.value);
            p = body.position; v = body.velocity;
        });
        ++ticks_;
    }
    template <std::size_t Rows = 1024, typename Pool>
    void tick(Pool& pool)
    {
        gather();
        world_.eachParallel<Position, Velocity, Mass, Id>(pool, [&](unsigned, Position& p, Velocity& v, const Mass&, const Id& id) {
            const auto body = advanceBody(input_, id.value);
            p = body.position; v = body.velocity;
        }, sub0ecs::store::RowGrain<Rows>{});
        ++ticks_;
    }
    const std::vector<Body>& state() { gather(); return input_; }
    std::size_t ticks() const { return ticks_; }
private:
    using Query = sub0ecs::Query<Position, Velocity, Mass, Id>;
    using World = sub0ecs::store::World<std::tuple<Query>>;
    World world_;
    std::vector<Body> input_;
    std::size_t ticks_{};
};
}
