#pragma once
/** Classical inheritance: a class hierarchy with virtual updates.
 *
 * One heap object per entity, its data as plain members, and a virtual call per
 * object per update: the design an ECS is usually measured against. The
 * hierarchy is Small <- Medium <- Large, each level adding members and
 * overriding the updates to include them. Entities are created through the
 * shared populate() (scenarios.hpp), so the mix and values match every other
 * design, and the updates run the shared kernels with the same constant timestep.
 *
 * Scope: Create, Iter1, Update2, Frame3 and RandomGet. An object's class is fixed
 * when it is created, so there are no queries and no adding or removing of
 * components. find<C>() exists so conformance can compare state.
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
        /** Iter1: nudge the position. */
        virtual void nudge() = 0;
        /** Update2: one physics step. The timestep is the workload's compile-time constant,
         *  exactly as in every other design: passed at runtime instead, `9.8f * dt` is fused
         *  into the add on FMA targets (ARM64, native x86) and rounds differently from the
         *  folded constant, so the designs would no longer be bit-identical. */
        virtual void update() = 0;
        /** Frame3: everything this class does in a frame, in one call. */
        virtual void frame() = 0;
        /** Address of the member for kSlot<C>, or nullptr if this class has none (conformance only). */
        virtual void* raw(int slot) = 0;
    };

    class Small : public Object
    {
    public:
        Small(Position p, Velocity v) : pos_(p), vel_(v) {}
        void nudge() override { pos_.x += 1.0f; }
        void update() override { kernel::updatePosition(pos_, vel_, kDeltaTime); }
        void frame() override { kernel::updatePosition(pos_, vel_, kDeltaTime); }
        const Velocity& velocity() const { return vel_; }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 0: return &pos_;
            case 1: return &vel_;
            default: return nullptr;
            }
        }

    protected:
        Position pos_;
        Velocity vel_;
    };

    class Medium : public Small
    {
    public:
        Medium(Position p, Velocity v, Health h, Rotation r, Scale s) : Small(p, v), health_(h), rotation_(r), scale_(s) {}
        void frame() override
        {
            Small::frame();
            kernel::updateRotationHealth(health_, rotation_, kDeltaTime);
        }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 2: return &health_;
            case 3: return &rotation_;
            case 4: return &scale_;
            default: return Small::raw(slot);
            }
        }

    protected:
        Health health_;
        Rotation rotation_;
        Scale scale_;
    };

    class Large : public Medium
    {
    public:
        Large(Position p, Velocity v, Health h, Rotation r, Scale s, Color c, Team t, Flags f)
            : Medium(p, v, h, r, s), color_(c), team_(t), flags_(f)
        {
        }
        void frame() override
        {
            Medium::frame();
            kernel::pulseScale(scale_, color_, kDeltaTime);
        }
        void* raw(int slot) override
        {
            switch (slot)
            {
            case 5: return &color_;
            case 6: return &team_;
            case 7: return &flags_;
            default: return Medium::raw(slot);
            }
        }

    private:
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

        /** One virtual call per entity for each scenario. */
        void nudgeAll()
        {
            for (auto& o : objects_) o->nudge();
        }
        void updateAll()
        {
            for (auto& o : objects_) o->update();
        }
        void frameAll()
        {
            for (auto& o : objects_) o->frame();
        }

        /** RandomGet: every class has a velocity, so no virtual call is needed. */
        const Velocity& velocity(Entity e) const { return static_cast<const Small&>(*objects_[e.index()]).velocity(); }

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
