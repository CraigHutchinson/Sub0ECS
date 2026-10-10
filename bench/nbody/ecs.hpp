#pragma once
#include <tuple>
#include "nbody/body.hpp"
#include "nbody/compact_sources.hpp"
#include "sub0ecs/store/world.hpp"
namespace bench::nbody
{
/** ECS application with a stable-ID snapshot and joined authoritative row writes.
 * No structural mutation takes place during a tick. Pool failures are fail-stop:
 * rows may have been modified, so a failed tick must not be published/retried as
 * though the state were transactional. This benchmark uses nonthrowing kernels.
 */
template <bool Compact = false>
class BasicEcs
{
public:
    explicit BasicEcs(std::size_t size) : sources_(Compact ? size : 0), input_(size)
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
        world_.template each<Position, Velocity, Mass, Id>([&](const Position& p, const Velocity& v, const Mass& m, const Id& id) {
            input_[id.value] = {p, v, m};
        });
    }
    void tick()
    {
        prepareSources();
        world_.template each<Position, Velocity, Mass, Id>([&](Position& p, Velocity& v, const Mass& m, const Id& id) {
            const auto body = advance(id.value, p, v, m);
            p = body.position; v = body.velocity;
        });
        ++ticks_;
    }
    template <std::size_t Rows = 1024, typename Pool>
    void tick(Pool& pool)
    {
        prepareSources();
        world_.template eachParallel<Position, Velocity, Mass, Id>(pool, [&](unsigned, Position& p, Velocity& v, const Mass& m, const Id& id) {
            const auto body = advance(id.value, p, v, m);
            p = body.position; v = body.velocity;
        }, sub0ecs::store::RowGrain<Rows>{});
        ++ticks_;
    }
    const std::vector<Body>& state() { gather(); return input_; }
    std::size_t ticks() const { return ticks_; }
private:
    void prepareSources()
    {
        if constexpr (Compact)
        {
            world_.template each<Position, Velocity, Mass, Id>([&](const Position& p, const Velocity&, const Mass& m, const Id& id) {
                sources_.set(id.value, p, m);
            });
        }
        else gather();
    }
    Body advance(std::size_t row, const Position& p, const Velocity& v, const Mass& m) const
    {
        if constexpr (Compact) return sources_.advance({p, v, m}, row);
        else return advanceBody(input_, row);
    }
    using Query = sub0ecs::Query<Position, Velocity, Mass, Id>;
    using World = sub0ecs::store::World<std::tuple<Query>>;
    World world_;
    CompactSources sources_;
    std::vector<Body> input_;
    std::size_t ticks_{};
};
using Ecs = BasicEcs<false>;
using CompactEcs = BasicEcs<true>;
}
