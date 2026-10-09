#pragma once
/** Partition: the rows of every entity matching the same set of declared queries. */

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <vector>

#include "sub0ecs/entity.hpp"
#include "sub0ecs/store/column.hpp"
#include "sub0ecs/store/mask.hpp"

namespace sub0ecs::store
{
    struct Partition
    {
        static constexpr std::size_t kAlign = 64;

        Mask columnsMask = 0;
        std::uint32_t signature = 0;   // bit i set = matches declared query i
        std::vector<Column> columns;
        std::array<std::int8_t, kMaxTypes> columnOf{};
        /** Column base pointer per component type (nullptr = not a column):
         *  find()/each() resolve a column with one load instead of
         *  columnOf -> columns[] -> data. Kept in sync on growth. */
        std::array<std::byte*, kMaxTypes> base{};
        std::vector<Entity> entities;
        std::size_t capacity = 0;      // rows allocated in every column
        /** Transition cache: last destination seen when adding/removing a
         *  fragmenting type from this partition. Keyed by the destination
         *  column mask too, so it is exact even when two entities in this
         *  partition carry different non-column fragmenting components. */
        // Empty = index kNoEdge. (Not a sentinel mask: every 64-bit value, all-ones
        // included, is the valid column mask of some entity.)
        static constexpr std::uint32_t kNoEdge = ~0u;
        struct Edge { Mask cols = 0; std::uint32_t index = kNoEdge; };
        std::array<Edge, kMaxTypes> addEdge{}, removeEdge{};

        Partition() = default;
        Partition(const Partition&) = delete;
        Partition& operator=(const Partition&) = delete;
        ~Partition()
        {
            for (auto& c : columns) ::operator delete(c.data, std::align_val_t{ kAlign });
        }

        std::size_t size() const { return entities.size(); }

        void addColumn(std::uint32_t type, std::size_t stride)
        {
            columnOf[type] = static_cast<std::int8_t>(columns.size());
            columns.push_back(Column{ type, stride, nullptr });
        }

        std::uint32_t pushRow(Entity e)
        {
            if (entities.size() == capacity) grow(capacity ? capacity * 2u : 64u);
            entities.push_back(e);
            return static_cast<std::uint32_t>(entities.size() - 1u);
        }

        /** Swap-remove; returns the entity moved into `row` (or null). */
        Entity swapRemove(std::uint32_t row)
        {
            const std::uint32_t last = static_cast<std::uint32_t>(entities.size() - 1u);
            Entity moved = kNullEntity;
            if (row != last)
            {
                for (auto& c : columns) copyRow(c.at(row), c.at(last), c.stride);
                entities[row] = entities[last];
                moved = entities[row];
            }
            entities.pop_back();
            return moved;
        }

    private:
        void grow(std::size_t rows)
        {
            for (auto& c : columns)
            {
                auto* fresh = static_cast<std::byte*>(::operator new(rows * c.stride, std::align_val_t{ kAlign }));
                if (c.data)
                {
                    std::memcpy(fresh, c.data, entities.size() * c.stride);
                    ::operator delete(c.data, std::align_val_t{ kAlign });
                }
                c.data = fresh;
                base[c.type] = fresh;
            }
            capacity = rows;
        }
    };

} // namespace sub0ecs::store
