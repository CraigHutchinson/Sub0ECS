#pragma once
/** Design D — Static fixed-capacity world with per-entity signature mask.
 *
 * Storage: compile-time component list; for each component a fixed array of
 *          Capacity elements indexed directly by entity slot; one 32-bit
 *          signature per slot (bit per component + alive bit).
 * Query:   linear scan of slots [0, highWater) testing the signature.
 * Mutation: add/remove = set/clear a bit, destroy = clear mask; all O(1).
 * Registry: compile-time (component index is a constant expression).
 *
 * Zero heap allocation after construction; memory = Capacity * sum(sizeof).
 * Target: embedded (ESP32-class) where N is bounded and known up front.
 */

#include <array>
#include <cstdint>
#include <cstdlib>
#include <tuple>
#include <type_traits>

#include "../common/components.hpp"
#include <sub0ecs/entity.hpp>

namespace bench::fixed
{
    template <typename T, typename... Ts>
    constexpr std::size_t indexOf()
    {
        std::size_t i = 0;
        const bool found = ((std::is_same_v<T, Ts> ? true : (++i, false)) || ...);
        return found ? i : ~std::size_t{ 0 };
    }

    template <std::size_t Capacity, typename... Components>
    class BasicWorld
    {
        static_assert(sizeof...(Components) < 32, "one bit per component + alive bit");

    public:
        using Entity = bench::Entity;
        using Signature = std::uint32_t;
        static constexpr const char* kName = "StaticBitmask";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;
        static constexpr Signature kAlive = Signature{ 1 } << 31;

        template <typename C>
        static constexpr Signature bit()
        {
            constexpr std::size_t i = indexOf<C, Components...>();
            static_assert(i < sizeof...(Components), "component not registered in this world");
            return Signature{ 1 } << i;
        }

        void reserve(std::size_t) {}

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            std::uint32_t slot;
            if (freeCount_ > 0) slot = free_[--freeCount_];
            else slot = highWater_++;
            if (slot >= Capacity) std::abort();   // fixed capacity exceeded: fail loudly, never corrupt
            mask_[slot] = kAlive | (bit<Cs>() | ... | Signature{ 0 });
            ((column<Cs>()[slot] = std::move(cs)), ...);
            return Entity::make(slot, version_[slot]);
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            constexpr Signature need = kAlive | (bit<Cs>() | ...);
            auto cols = std::tuple<Cs*...>{ column<Cs>().data()... };
            for (std::uint32_t i = 0; i < highWater_; ++i)
            {
                if ((mask_[i] & need) == need) f(std::get<Cs*>(cols)[i]...);
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            const std::uint32_t i = e.index();
            if (i >= highWater_ || version_[i] != e.version() || !(mask_[i] & bit<C>())) return nullptr;
            return &column<C>()[i];
        }

        template <typename C>
        void add(Entity e, C c)
        {
            mask_[e.index()] |= bit<C>();
            column<C>()[e.index()] = std::move(c);
        }

        template <typename C>
        void remove(Entity e) { mask_[e.index()] &= ~bit<C>(); }

        void destroy(Entity e)
        {
            const std::uint32_t i = e.index();
            mask_[i] = 0;
            version_[i] = static_cast<std::uint8_t>(version_[i] + 1u);
            free_[freeCount_++] = i;
        }

        void commit() {}

    private:
        template <typename C>
        std::array<C, Capacity>& column() { return std::get<indexOf<C, Components...>()>(columns_); }

        std::array<Signature, Capacity> mask_{};
        std::array<std::uint8_t, Capacity> version_{};
        std::array<std::uint32_t, Capacity> free_{};
        std::uint32_t freeCount_ = 0;
        std::uint32_t highWater_ = 0;
        std::tuple<std::array<Components, Capacity>...> columns_{};
    };

    template <std::size_t Capacity>
    using World = BasicWorld<Capacity, Position, Velocity, Health, Rotation, Scale, Color, Team, Flags, Tag, Frozen>;

} // namespace bench::fixed
