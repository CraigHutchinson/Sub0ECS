#pragma once
/** Design H1 — Query-signature partitions ("automatic archetypes").
 *
 * See research/holographic-storage.md §4.1. The world is told its system
 * queries up front. Physical partitions are keyed by the set of declared
 * queries an entity matches, not by its full component signature.
 *
 * Invariant (columns follow queries): in a partition, the dense columns are
 * the union of the required components of every query the partition
 * matches. Every other component of an entity lives in type-erased side
 * storage (sparse set) and costs nothing for iteration.
 *
 * Consequences tested by H1:
 *   - add/remove of a component no query requires moves no data
 *   - iteration of a declared query = whole partitions of dense columns
 *
 * Two modes:
 *   Carry = false ("QueryPart")  pure automatic: every unqueried component
 *                                goes to side storage.
 *   Carry = true  ("QPartHinted") unqueried components ride along as dense
 *                                columns (extra key bits) unless declared
 *                                Volatile<...>; trades churn cost for memory
 *                                and find() locality (§4.5 planner choice).
 *
 * Spike constraints (same as archetype.hpp): trivially copyable components,
 * ≤ 64 component types, ≤ 32 declared queries, each<Cs...> must name a
 * declared Query<Cs...> exactly.
 */

#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../common/components.hpp"
#include "../common/entity.hpp"

namespace spike::qpart
{
    using Mask = std::uint64_t;
    inline constexpr std::uint32_t kMaxTypes = 64;
    inline constexpr std::int8_t kNoColumn = -1;

    template <typename... Cs> struct Query {};
    template <typename... Cs> struct Volatile {};

    template <typename T>
    Mask bit() { return Mask{ 1 } << typeId<T>(); }

    /** Type-erased dense column (one per hot component in a partition). */
    struct Column
    {
        std::uint32_t type = 0;
        std::size_t stride = 0;
        std::vector<std::byte> bytes;

        std::byte* at(std::size_t row) { return bytes.data() + row * stride; }
    };

    /** Type-erased sparse set for components that are not columns. */
    class SidePool
    {
    public:
        static constexpr std::uint32_t kNull = ~0u;

        explicit SidePool(std::size_t stride) : stride_(stride) {}

        std::byte* find(Entity e)
        {
            const std::uint32_t i = e.index();
            return (i < sparse_.size() && sparse_[i] != kNull) ? bytes_.data() + sparse_[i] * stride_ : nullptr;
        }

        void emplace(Entity e, const void* src)
        {
            const std::uint32_t i = e.index();
            if (i >= sparse_.size()) sparse_.resize(i + 1u, kNull);
            sparse_[i] = static_cast<std::uint32_t>(dense_.size());
            dense_.push_back(e);
            bytes_.resize(bytes_.size() + stride_);
            std::memcpy(bytes_.data() + (dense_.size() - 1u) * stride_, src, stride_);
        }

        void remove(Entity e)
        {
            const std::uint32_t i = e.index();
            const std::uint32_t row = sparse_[i];
            const std::uint32_t last = static_cast<std::uint32_t>(dense_.size() - 1u);
            if (row != last)
            {
                std::memcpy(bytes_.data() + row * stride_, bytes_.data() + last * stride_, stride_);
                dense_[row] = dense_[last];
                sparse_[dense_[row].index()] = row;
            }
            dense_.pop_back();
            bytes_.resize(bytes_.size() - stride_);
            sparse_[i] = kNull;
        }

    private:
        std::size_t stride_;
        std::vector<std::uint32_t> sparse_;
        std::vector<Entity> dense_;
        std::vector<std::byte> bytes_;
    };

    struct Partition
    {
        Mask columnsMask = 0;
        std::uint32_t signature = 0;   // bit i set = matches declared query i
        std::vector<Column> columns;
        std::array<std::int8_t, kMaxTypes> columnOf{};
        std::vector<Entity> entities;

        std::size_t size() const { return entities.size(); }

        std::uint32_t pushRow(Entity e)
        {
            entities.push_back(e);
            for (auto& c : columns) c.bytes.resize(c.bytes.size() + c.stride);
            return static_cast<std::uint32_t>(entities.size() - 1u);
        }

        /** Swap-remove; returns the entity moved into `row` (or null). */
        Entity swapRemove(std::uint32_t row)
        {
            const std::uint32_t last = static_cast<std::uint32_t>(entities.size() - 1u);
            Entity moved = kNullEntity;
            if (row != last)
            {
                for (auto& c : columns) std::memcpy(c.at(row), c.at(last), c.stride);
                entities[row] = entities[last];
                moved = entities[row];
            }
            entities.pop_back();
            for (auto& c : columns) c.bytes.resize(c.bytes.size() - c.stride);
            return moved;
        }
    };

    template <typename T, typename... Ts>
    constexpr std::size_t indexOf()
    {
        std::size_t i = 0;
        const bool found = ((std::is_same_v<T, Ts> ? true : (++i, false)) || ...);
        return found ? i : ~std::size_t{ 0 };
    }

    template <bool Carry, typename Queries, typename Volatiles = Volatile<>>
    class BasicWorld;

    template <bool Carry, typename... Qs, typename... Vs>
    class BasicWorld<Carry, std::tuple<Qs...>, Volatile<Vs...>>
    {
        static constexpr std::size_t kQueries = sizeof...(Qs);
        static_assert(kQueries <= 32, "signature is 32-bit");

    public:
        using Entity = spike::Entity;
        static constexpr const char* kName = Carry ? "QPartHinted" : "QueryPart";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;

        BasicWorld()
            : required_{ requiredMask(Qs{})... }
        {
            for (Mask m : required_) queried_ |= m;
            volatile_ = (Mask{ 0 } | ... | bit<Vs>()) & ~queried_;   // a queried component is never volatile
            strides_.fill(0);
        }

        void reserve(std::size_t n) { records_.reserve(n); }

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            (registerType<Cs>(), ...);
            const Entity e = entities_.create();
            const Mask has = (Mask{ 0 } | ... | bit<Cs>());
            Partition& p = partitionFor(columnsFor(has));
            const std::uint32_t row = p.pushRow(e);
            setRecord(e, Record{ &p, row, has });
            (store(p, row, e, std::move(cs)), ...);
            return e;
        }

        template <typename... Cs, typename F>
        void each(F&& f)
        {
            constexpr std::size_t qi = indexOf<Query<Cs...>, Qs...>();
            static_assert(qi < kQueries, "each<Cs...> must name a declared Query<Cs...>");
            for (Partition* p : byQuery_[qi])
            {
                const std::size_t n = p->size();
                if (n == 0) continue;
                std::tuple<Cs*...> cols{ reinterpret_cast<Cs*>(p->columns[p->columnOf[typeId<Cs>()]].bytes.data())... };
                for (std::size_t i = 0; i < n; ++i) f(std::get<Cs*>(cols)[i]...);
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            if (!entities_.alive(e)) return nullptr;
            const Record& r = records_[e.index()];
            const std::uint32_t t = typeId<C>();
            if (!(r.has & bit<C>())) return nullptr;
            const std::int8_t c = r.partition->columnOf[t];
            if (c != kNoColumn) return reinterpret_cast<C*>(r.partition->columns[c].at(r.row));
            return reinterpret_cast<C*>(side_[t]->find(e));
        }

        template <typename C>
        void add(Entity e, C value)
        {
            registerType<C>();
            Record& r = records_[e.index()];
            const Mask newHas = r.has | bit<C>();
            const Mask newCols = columnsFor(newHas);
            if (newCols != r.partition->columnsMask)
            {
                moveTo(e, r, newCols, newHas);
            }
            r.has = newHas;
            store(*r.partition, r.row, e, std::move(value));
        }

        template <typename C>
        void remove(Entity e)
        {
            Record& r = records_[e.index()];
            const std::uint32_t t = typeId<C>();
            const Mask newHas = r.has & ~bit<C>();
            const Mask newCols = columnsFor(newHas);
            if (newCols == r.partition->columnsMask)
            {
                side_[t]->remove(e);   // not a column: pure side-storage op, no data moves
            }
            else
            {
                if (r.partition->columnOf[t] == kNoColumn) side_[t]->remove(e);
                moveTo(e, r, newCols, newHas);
            }
            r.has = newHas;
        }

        void destroy(Entity e)
        {
            Record& r = records_[e.index()];
            Mask sideBits = r.has & ~r.partition->columnsMask;
            while (sideBits)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(sideBits));
                side_[t]->remove(e);
                sideBits &= sideBits - 1u;
            }
            const Entity moved = r.partition->swapRemove(r.row);
            if (!(moved == kNullEntity)) records_[moved.index()].row = r.row;
            r = Record{};
            entities_.release(e);
        }

        void commit() {}

        std::size_t partitionCount() const { return partitions_.size(); }

    private:
        struct Record
        {
            Partition* partition = nullptr;
            std::uint32_t row = 0;
            Mask has = 0;   // full component set of the entity
        };

        template <typename... Cs>
        static Mask requiredMask(Query<Cs...>) { return (Mask{ 0 } | ... | bit<Cs>()); }

        /** Plan function: which components are dense columns for an entity with `has`. */
        Mask columnsFor(Mask has) const
        {
            if constexpr (Carry)
            {
                return has & ~volatile_;
            }
            else
            {
                Mask cols = 0;
                for (Mask req : required_)
                {
                    if ((has & req) == req) cols |= req;
                }
                return cols;
            }
        }

        std::uint32_t signatureOf(Mask cols) const
        {
            // A query matches iff its required set ⊆ columns (see §4.1 proof).
            std::uint32_t sig = 0;
            for (std::size_t i = 0; i < kQueries; ++i)
            {
                if ((cols & required_[i]) == required_[i]) sig |= 1u << i;
            }
            return sig;
        }

        template <typename T>
        void registerType()
        {
            static_assert(std::is_trivially_copyable_v<T>, "query-partition spike requires trivially copyable components");
            const std::uint32_t t = typeId<T>();
            assert(t < kMaxTypes);
            if (strides_[t] == 0)
            {
                strides_[t] = sizeof(T);
                side_[t] = std::make_unique<SidePool>(sizeof(T));
            }
        }

        template <typename T>
        void store(Partition& p, std::uint32_t row, Entity e, T value)
        {
            const std::uint32_t t = typeId<T>();
            const std::int8_t c = p.columnOf[t];
            if (c != kNoColumn) std::memcpy(p.columns[c].at(row), &value, sizeof(T));
            else side_[t]->emplace(e, &value);
        }

        void setRecord(Entity e, Record r)
        {
            if (e.index() >= records_.size()) records_.resize(e.index() + 1u);
            records_[e.index()] = r;
        }

        /** Move e to the partition for newCols; values migrate column<->side as needed. */
        void moveTo(Entity e, Record& r, Mask newCols, Mask newHas)
        {
            Partition& src = *r.partition;
            Partition& dst = partitionFor(newCols);
            const std::uint32_t srcRow = r.row;
            const std::uint32_t dstRow = dst.pushRow(e);

            for (auto& col : dst.columns)
            {
                const std::int8_t s = src.columnOf[col.type];
                if (s != kNoColumn)
                {
                    std::memcpy(col.at(dstRow), src.columns[s].at(srcRow), col.stride);
                }
                else if (r.has & (Mask{ 1 } << col.type))
                {
                    // promote: side storage -> column
                    SidePool& pool = *side_[col.type];
                    std::memcpy(col.at(dstRow), pool.find(e), col.stride);
                    pool.remove(e);
                }
                // else: component being added; caller stores it after the move
            }
            for (auto& col : src.columns)
            {
                if (dst.columnOf[col.type] == kNoColumn && (newHas & (Mask{ 1 } << col.type)))
                {
                    side_[col.type]->emplace(e, col.at(srcRow));   // demote: column -> side storage
                }
            }

            const Entity moved = src.swapRemove(srcRow);
            if (!(moved == kNullEntity)) records_[moved.index()].row = srcRow;
            r.partition = &dst;
            r.row = dstRow;
        }

        Partition& partitionFor(Mask cols)
        {
            auto it = byColumns_.find(cols);
            if (it != byColumns_.end()) return *it->second;

            auto p = std::make_unique<Partition>();
            p->columnsMask = cols;
            p->signature = signatureOf(cols);
            p->columnOf.fill(kNoColumn);
            for (std::uint32_t t = 0; t < kMaxTypes; ++t)
            {
                if (cols & (Mask{ 1 } << t))
                {
                    p->columnOf[t] = static_cast<std::int8_t>(p->columns.size());
                    p->columns.push_back(Column{ t, strides_[t], {} });
                }
            }
            for (std::size_t i = 0; i < kQueries; ++i)
            {
                if (p->signature & (1u << i)) byQuery_[i].push_back(p.get());
            }
            Partition& ref = *p;
            byColumns_.emplace(cols, p.get());
            partitions_.push_back(std::move(p));
            return ref;
        }

        std::array<Mask, kQueries> required_;
        Mask queried_ = 0;
        Mask volatile_ = 0;
        std::array<std::size_t, kMaxTypes> strides_{};
        std::array<std::unique_ptr<SidePool>, kMaxTypes> side_{};

        EntityAllocator entities_;
        std::vector<Record> records_;
        std::vector<std::unique_ptr<Partition>> partitions_;
        std::unordered_map<Mask, Partition*> byColumns_;
        std::array<std::vector<Partition*>, kQueries> byQuery_{};
    };

    /** The spike workload's system set (scenarios.hpp). */
    using SpikeQueries = std::tuple<Query<Position>, Query<Position, Velocity>, Query<Health, Rotation>,
                                    Query<Scale, Color>, Query<Position, Velocity, Tag>>;

    using World = BasicWorld<false, SpikeQueries>;
    using HintedWorld = BasicWorld<true, SpikeQueries, Volatile<Frozen>>;

} // namespace spike::qpart
