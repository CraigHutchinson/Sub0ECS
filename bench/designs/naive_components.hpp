#pragma once
/** Naive component architecture: every entity is an object that owns a list of
 * separately allocated components.
 *
 * The "game object with components" design many engines started from, and
 * what an ECS replaces: flexible (any object can gain or lose any component at
 * any time) and simple, at the price of one allocation per component and a
 * lookup per component per object in every system.
 *
 * Storage:  one heap GameObject per entity; each component its own heap block.
 * Query:    visit every live object, look each component up in its list.
 * Mutation: add = allocate and append; remove = erase from the list.
 */

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "../common/components.hpp"
#include <sub0ecs/detail/hints.hpp>
#include <sub0ecs/entity.hpp>

namespace bench::naive
{
    struct Component
    {
        virtual ~Component() = default;
    };

    template <typename T>
    struct Holder final : Component
    {
        explicit Holder(T v) : value(std::move(v)) {}
        T value;
    };

    class GameObject
    {
    public:
        template <typename T>
        T* get()
        {
            const std::uint32_t id = typeId<T>();
            for (auto& part : parts_)
                if (part.first == id) return &static_cast<Holder<T>&>(*part.second).value;
            return nullptr;
        }

        template <typename T>
        void add(T value)
        {
            if (T* existing = get<T>()) *existing = std::move(value);
            else parts_.emplace_back(typeId<T>(), std::make_unique<Holder<T>>(std::move(value)));
        }

        template <typename T>
        void remove()
        {
            const std::uint32_t id = typeId<T>();
            for (std::size_t i = 0; i < parts_.size(); ++i)
            {
                if (parts_[i].first != id) continue;
                parts_.erase(parts_.begin() + static_cast<std::ptrdiff_t>(i));
                return;
            }
        }

    private:
        std::vector<std::pair<std::uint32_t, std::unique_ptr<Component>>> parts_;
    };

    class World
    {
    public:
        using Entity = bench::Entity;
        static constexpr const char* kName = "NaiveObjects";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;

        void reserve(std::size_t n) { objects_.reserve(n); }

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            const Entity e = entities_.create();
            if (e.index() >= objects_.size()) objects_.resize(e.index() + 1u);
            auto object = std::make_unique<GameObject>();
            (object->add(std::move(cs)), ...);
            objects_[e.index()] = std::move(object);
            return e;
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            for (auto& object : objects_)
            {
                if (!object) continue;
                visit(f, object->template get<Cs>()...);
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            return entities_.alive(e) ? objects_[e.index()]->template get<C>() : nullptr;
        }

        template <typename C>
        void add(Entity e, C c)
        {
            if (entities_.alive(e)) objects_[e.index()]->add(std::move(c));
        }

        template <typename C>
        void remove(Entity e)
        {
            if (entities_.alive(e)) objects_[e.index()]->template remove<C>();
        }

        void destroy(Entity e)
        {
            if (!entities_.alive(e)) return;
            objects_[e.index()].reset();
            entities_.release(e);
        }

        void commit() {}

    private:
        template <typename F, typename... Cs>
        static void visit(F& f, Cs*... parts)
        {
            if ((parts && ...)) SUB0ECS_FLATTEN_CALLS f(*parts...);
        }

        EntityAllocator entities_;
        std::vector<std::unique_ptr<GameObject>> objects_;   // by entity slot; null once destroyed
    };
} // namespace bench::naive
