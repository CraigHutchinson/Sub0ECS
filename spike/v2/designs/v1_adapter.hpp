#pragma once
/** Design 0 — SubzeroECS v1 (baseline), used unmodified via its public API.
 *
 * Storage: per-component sorted EntityId vector + parallel component vector.
 * Query:   View<> N-way sorted intersection (galloping), .at() on access.
 * Limits:  no component removal, no entity destruction, ids never recycled,
 *          max 32 live worlds (static per-type registry table).
 */

#include "SubzeroECS/Collection.hpp"
#include "SubzeroECS/View.hpp"
#include "SubzeroECS/World.hpp"

#include "../common/components.hpp"

namespace spike::v1
{
    class World
    {
    public:
        using Entity = SubzeroECS::EntityId;
        static constexpr const char* kName = "V1";
        static constexpr bool kSupportsRemove = false;
        static constexpr bool kSupportsDestroy = false;

        void reserve(std::size_t) {}

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            return world_.create(std::move(cs)...).id();
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            SubzeroECS::View<Cs...> view(world_);
            const auto end = view.end();
            for (auto it = view.begin(); it != end; ++it)
            {
                f(it.template get<Cs>()...);
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            return world_.find<C>(e);
        }

        template <typename C>
        void add(Entity e, C c)
        {
            world_.add(e, std::move(c));
        }

        template <typename C>
        void remove(Entity) {}   // unsupported in v1

        void destroy(Entity) {}  // unsupported in v1

        void commit() {}

    private:
        SubzeroECS::World world_;
        SubzeroECS::Collection<Position, Velocity, Health, Rotation, Scale, Color, Team, Flags, Tag, Frozen>
            collections_{ world_ };
    };

} // namespace spike::v1
