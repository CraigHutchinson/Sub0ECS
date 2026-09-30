#pragma once
/** Design B — Archetype tables (flecs / Bevy / Unity DOTS style).
 *
 * Storage: entities grouped by exact component signature; each archetype is a
 *          table of SoA columns (one contiguous array per component type).
 * Query:   match archetypes by mask (cached per query mask), then a tight
 *          linear loop over typed column pointers — no per-entity lookups.
 * Mutation: add/remove moves the row to another archetype (memcpy per column,
 *          transition edges cached), destroy = swap-remove row.
 * Registry: runtime typeId<T>() -> bit in a 64-bit signature mask.
 *
 * Spike constraints: components must be trivially copyable (memcpy moves),
 * at most 64 component types per process.
 */

#include <array>
#include <cassert>
#include <cstring>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../common/components.hpp"
#include <sub0ecs/entity.hpp>

namespace bench::archetype
{
    using Mask = std::uint64_t;
    inline constexpr std::uint32_t kMaxTypes = 64;
    inline constexpr std::int8_t kNoColumn = -1;

    template <typename T>
    Mask bit() { return Mask{ 1 } << typeId<T>(); }

    struct Column
    {
        std::uint32_t type = 0;
        std::size_t stride = 0;
        std::vector<std::byte> bytes;

        std::byte* at(std::size_t row) { return bytes.data() + row * stride; }
    };

    struct Archetype
    {
        Mask mask = 0;
        std::vector<Column> columns;
        std::array<std::int8_t, kMaxTypes> columnOf{};
        std::vector<Entity> entities;
        std::array<Archetype*, kMaxTypes> addEdge{};     // cached transition graph
        std::array<Archetype*, kMaxTypes> removeEdge{};

        std::size_t size() const { return entities.size(); }

        template <typename T>
        T* column()
        {
            const std::int8_t c = columnOf[typeId<T>()];
            return c == kNoColumn ? nullptr : reinterpret_cast<T*>(columns[c].bytes.data());
        }

        /** Append an uninitialised row, returns row index. */
        std::uint32_t pushRow(Entity e)
        {
            entities.push_back(e);
            for (auto& col : columns) col.bytes.resize(col.bytes.size() + col.stride);
            return static_cast<std::uint32_t>(entities.size() - 1u);
        }

        /** Swap-remove a row, returns the entity that moved into it (or null). */
        Entity swapRemove(std::uint32_t row)
        {
            const std::uint32_t last = static_cast<std::uint32_t>(entities.size() - 1u);
            Entity moved = kNullEntity;
            if (row != last)
            {
                for (auto& col : columns) std::memcpy(col.at(row), col.at(last), col.stride);
                entities[row] = entities[last];
                moved = entities[row];
            }
            entities.pop_back();
            for (auto& col : columns) col.bytes.resize(col.bytes.size() - col.stride);
            return moved;
        }
    };

    class World
    {
    public:
        using Entity = bench::Entity;
        static constexpr const char* kName = "Archetype";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;

        World() { strides_.fill(0); }

        void reserve(std::size_t n)
        {
            entities_.reserve(n);
            records_.reserve(n);
        }

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            (registerType<Cs>(), ...);
            const Entity e = entities_.create();
            Archetype& a = archetypeFor((bit<Cs>() | ... | Mask{ 0 }));
            const std::uint32_t row = a.pushRow(e);
            (write(a, row, std::move(cs)), ...);
            setRecord(e, &a, row);
            return e;
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            const Mask need = (bit<Cs>() | ...);
            for (Archetype* a : matching(need))
            {
                const std::size_t n = a->size();
                if (n == 0) continue;
                std::tuple<Cs*...> cols{ a->column<Cs>()... };
                for (std::size_t i = 0; i < n; ++i)
                {
                    f(std::get<Cs*>(cols)[i]...);
                }
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            if (!entities_.alive(e)) return nullptr;
            const Record& r = records_[e.index()];
            C* col = r.archetype->column<C>();
            return col ? col + r.row : nullptr;
        }

        template <typename C>
        void add(Entity e, C c)
        {
            registerType<C>();
            const std::uint32_t t = typeId<C>();
            Record& r = records_[e.index()];
            Archetype* src = r.archetype;
            Archetype*& edge = src->addEdge[t];
            if (!edge) edge = &archetypeFor(src->mask | bit<C>());
            const std::uint32_t row = moveRow(e, *src, r.row, *edge);
            write(*edge, row, std::move(c));
        }

        template <typename C>
        void remove(Entity e)
        {
            const std::uint32_t t = typeId<C>();
            Record& r = records_[e.index()];
            Archetype* src = r.archetype;
            Archetype*& edge = src->removeEdge[t];
            if (!edge) edge = &archetypeFor(src->mask & ~bit<C>());
            moveRow(e, *src, r.row, *edge);
        }

        void destroy(Entity e)
        {
            Record& r = records_[e.index()];
            const Entity moved = r.archetype->swapRemove(r.row);
            if (!(moved == kNullEntity)) records_[moved.index()].row = r.row;
            r = Record{};
            entities_.release(e);
        }

        void commit() {}

        std::size_t archetypeCount() const { return archetypes_.size(); }

    private:
        struct Record
        {
            Archetype* archetype = nullptr;
            std::uint32_t row = 0;
        };

        template <typename T>
        void registerType()
        {
            static_assert(std::is_trivially_copyable_v<T>, "archetype spike requires trivially copyable components");
            const std::uint32_t t = typeId<T>();
            assert(t < kMaxTypes);
            strides_[t] = sizeof(T);
        }

        template <typename T>
        static void write(Archetype& a, std::uint32_t row, T value)
        {
            std::memcpy(a.columns[a.columnOf[typeId<T>()]].at(row), &value, sizeof(T));
        }

        void setRecord(Entity e, Archetype* a, std::uint32_t row)
        {
            if (e.index() >= records_.size()) records_.resize(e.index() + 1u);
            records_[e.index()] = Record{ a, row };
        }

        /** Move entity row src->dst, copying shared columns. Returns new row in dst. */
        std::uint32_t moveRow(Entity e, Archetype& src, std::uint32_t srcRow, Archetype& dst)
        {
            const std::uint32_t dstRow = dst.pushRow(e);
            for (auto& col : src.columns)
            {
                const std::int8_t d = dst.columnOf[col.type];
                if (d != kNoColumn) std::memcpy(dst.columns[d].at(dstRow), col.at(srcRow), col.stride);
            }
            const Entity moved = src.swapRemove(srcRow);
            if (!(moved == kNullEntity)) records_[moved.index()].row = srcRow;
            records_[e.index()] = Record{ &dst, dstRow };
            return dstRow;
        }

        Archetype& archetypeFor(Mask mask)
        {
            auto it = byMask_.find(mask);
            if (it != byMask_.end()) return *it->second;

            auto a = std::make_unique<Archetype>();
            a->mask = mask;
            a->columnOf.fill(kNoColumn);
            for (std::uint32_t t = 0; t < kMaxTypes; ++t)
            {
                if (mask & (Mask{ 1 } << t))
                {
                    a->columnOf[t] = static_cast<std::int8_t>(a->columns.size());
                    a->columns.push_back(Column{ t, strides_[t], {} });
                }
            }
            Archetype& ref = *a;
            byMask_.emplace(mask, a.get());
            archetypes_.push_back(std::move(a));
            return ref;
        }

        const std::vector<Archetype*>& matching(Mask need)
        {
            QueryCache& q = queries_[need];
            if (q.seen != archetypes_.size())
            {
                for (std::size_t i = q.seen; i < archetypes_.size(); ++i)
                {
                    if ((archetypes_[i]->mask & need) == need) q.archetypes.push_back(archetypes_[i].get());
                }
                q.seen = archetypes_.size();
            }
            return q.archetypes;
        }

        struct QueryCache
        {
            std::size_t seen = 0;
            std::vector<Archetype*> archetypes;
        };

        EntityAllocator entities_;
        std::vector<Record> records_;
        std::vector<std::unique_ptr<Archetype>> archetypes_;
        std::unordered_map<Mask, Archetype*> byMask_;
        std::unordered_map<Mask, QueryCache> queries_;
        std::array<std::size_t, kMaxTypes> strides_{};
    };

} // namespace bench::archetype
