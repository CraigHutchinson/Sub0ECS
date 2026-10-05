#pragma once
/** SidePool<T>: sparse-set side storage for components that are not columns. */

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

#include "../entity.hpp"

namespace sub0ecs::store
{
    /** Side storage: sparse set for components that are not columns.
     *  Typed (hot add/remove/find paths are inlined); the virtual interface
     *  is only used when values migrate between side storage and columns. */
    class SidePoolBase
    {
    public:
        static constexpr std::uint32_t kNull = ~0u;
        virtual ~SidePoolBase() = default;
        virtual const void* findRaw(Entity e) const = 0;
        virtual void emplaceRaw(Entity e, const void* src) = 0;
        virtual void remove(Entity e) = 0;
        virtual void removeIfPresent(Entity e) = 0;
        virtual const void* tryFindRaw(Entity e) const = 0;
        virtual std::size_t size() const = 0;
        virtual Entity entityAt(std::size_t i) const = 0;
    };

    template <typename T>
    class SidePool final : public SidePoolBase
    {
    public:
        T* find(Entity e)
        {
            const std::uint32_t i = e.index();
            return (i < sparse_.size() && sparse_[i] != kNull) ? &data_[sparse_[i]] : nullptr;
        }

        /** Insert, or overwrite when e already holds a value (never a second dense entry). */
        void emplace(Entity e, const T& value)
        {
            const std::uint32_t i = e.index();
            if (i >= sparse_.size()) sparse_.resize(i + 1u, kNull);
            if (sparse_[i] != kNull)
            {
                data_[sparse_[i]] = value;
                return;
            }
            sparse_[i] = static_cast<std::uint32_t>(dense_.size());
            dense_.push_back(e);
            data_.push_back(value);
        }

        /** Precondition: e is present (removeIfPresent otherwise). */
        void remove(Entity e) override
        {
            const std::uint32_t i = e.index();
            assert(i < sparse_.size() && sparse_[i] != kNull);
            const std::uint32_t row = sparse_[i];
            const Entity last = dense_.back();
            dense_[row] = last;
            data_[row] = data_.back();
            sparse_[last.index()] = row;
            sparse_[i] = kNull;
            dense_.pop_back();
            data_.pop_back();
        }

        void removeIfPresent(Entity e) override
        {
            if (find(e)) remove(e);
        }

        const void* findRaw(Entity e) const override { return &data_[sparse_[e.index()]]; }
        const void* tryFindRaw(Entity e) const override
        {
            const std::uint32_t i = e.index();
            return (i < sparse_.size() && sparse_[i] != kNull) ? &data_[sparse_[i]] : nullptr;
        }
        std::size_t size() const override { return dense_.size(); }
        Entity entityAt(std::size_t i) const override { return dense_[i]; }

        void emplaceRaw(Entity e, const void* src) override
        {
            T value;
            std::memcpy(&value, src, sizeof(T));
            emplace(e, value);
        }

    private:
        std::vector<std::uint32_t> sparse_;
        std::vector<Entity> dense_;
        std::vector<T> data_;
    };

} // namespace sub0ecs::store
