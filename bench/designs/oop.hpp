#pragma once
/** OOP comparator: v1's update_patterns "OOP" design (see the master branch), rebuilt on
 * the v2 benchmark workload.
 *
 * One heap object per entity, the component data as plain members, and a virtual
 * update() per object: the class-hierarchy design an ECS is usually measured
 * against. Entities are created through the shared populate() (scenarios.hpp), so the
 * entity mix and values are identical to every other design; Update2's work is
 * updateAll(), one virtual call per entity running the same kernel::updatePosition
 * with the same constant timestep.
 *
 * Scope, as in v1: Create and Update2 only (no queries, structural change or
 * destroy). find<C>() exists so conformance can compare state; it is not timed.
 */

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

#include "../common/components.hpp"
#include "../common/scenarios.hpp"

namespace bench::oop
{
    /** Slot of a component type in Object::raw(): the components a hierarchy class holds. */
    template <typename C>
    constexpr int kSlot = std::is_same_v<C, Position> ? 0
                        : std::is_same_v<C, Velocity> ? 1
                        : std::is_same_v<C, Health>   ? 2
                        : std::is_same_v<C, Rotation> ? 3
                        : std::is_same_v<C, Scale>    ? 4
                        : std::is_same_v<C, Color>    ? 5
                        : std::is_same_v<C, Team>     ? 6
                        : std::is_same_v<C, Flags>    ? 7
                                                      : -1;

    class Object
    {
    public:
        virtual ~Object() = default;
        /** One Update2 step. The timestep is the workload's compile-time constant, exactly as
         *  in every other design: passed at runtime instead, `9.8f * dt` is fused into the
         *  add on FMA targets (ARM64, native x86) and rounds differently from the folded
         *  constant, so the designs would no longer be bit-identical. */
        virtual void update() = 0;
        /** Address of the member for kSlot<C>, or nullptr if this class has none (conformance only). */
        virtual void* raw(int slot) = 0;
    };

    class Small : public Object
    {
    public:
        Small(Position p, Velocity v) : pos_(p), vel_(v) {}
        void update() override { kernel::updatePosition(pos_, vel_, kDeltaTime); }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 0: return &pos_;
            case 1: return &vel_;
            default: return nullptr;
            }
        }

    private:
        Position pos_;
        Velocity vel_;
    };

    class Medium : public Object
    {
    public:
        Medium(Position p, Velocity v, Health h, Rotation r, Scale s) : pos_(p), vel_(v), health_(h), rotation_(r), scale_(s) {}
        void update() override { kernel::updatePosition(pos_, vel_, kDeltaTime); }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 0: return &pos_;
            case 1: return &vel_;
            case 2: return &health_;
            case 3: return &rotation_;
            case 4: return &scale_;
            default: return nullptr;
            }
        }

    private:
        Position pos_;
        Velocity vel_;
        Health health_;
        Rotation rotation_;
        Scale scale_;
    };

    class Large : public Object
    {
    public:
        Large(Position p, Velocity v, Health h, Rotation r, Scale s, Color c, Team t, Flags f)
            : pos_(p), vel_(v), health_(h), rotation_(r), scale_(s), color_(c), team_(t), flags_(f)
        {
        }
        void update() override { kernel::updatePosition(pos_, vel_, kDeltaTime); }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 0: return &pos_;
            case 1: return &vel_;
            case 2: return &health_;
            case 3: return &rotation_;
            case 4: return &scale_;
            case 5: return &color_;
            case 6: return &team_;
            case 7: return &flags_;
            default: return nullptr;
            }
        }

    private:
        Position pos_;
        Velocity vel_;
        Health health_;
        Rotation rotation_;
        Scale scale_;
        Color color_;
        Team team_;
        Flags flags_;
    };

    class World
    {
    public:
        using Entity = bench::Entity;
        static constexpr const char* kName = "OOP";
        static constexpr bool kSupportsRemove = false;
        static constexpr bool kSupportsDestroy = false;

        void reserve(std::size_t n) { objects_.reserve(n); }

        /** The three entity shapes createOne() makes; any other combination does not compile. */
        template <typename... Cs>
        Entity create(Cs... cs)
        {
            objects_.push_back(make(cs...));
            return Entity::make(static_cast<std::uint32_t>(objects_.size() - 1u), 0);
        }

        /** Update2: one virtual call per entity. */
        void updateAll()
        {
            for (auto& o : objects_) o->update();
        }

        template <typename C>
        C* find(Entity e)
        {
            if (e.index() >= objects_.size()) return nullptr;
            if constexpr (kSlot<C> < 0) return nullptr;   // no class in the hierarchy holds C
            else return static_cast<C*>(objects_[e.index()]->raw(kSlot<C>));
        }

        void commit() {}

    private:
        static std::unique_ptr<Object> make(Position p, Velocity v) { return std::make_unique<Small>(p, v); }
        static std::unique_ptr<Object> make(Position p, Velocity v, Health h, Rotation r, Scale s)
        {
            return std::make_unique<Medium>(p, v, h, r, s);
        }
        static std::unique_ptr<Object> make(Position p, Velocity v, Health h, Rotation r, Scale s, Color c, Team t, Flags f)
        {
            return std::make_unique<Large>(p, v, h, r, s, c, t, f);
        }

        std::vector<std::unique_ptr<Object>> objects_;
    };
} // namespace bench::oop
