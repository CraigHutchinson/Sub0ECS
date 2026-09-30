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
#include "../common/query.hpp"

namespace spike::qpart
{
    using Mask = std::uint64_t;
    inline constexpr std::uint32_t kMaxTypes = 64;
    inline constexpr std::int8_t kNoColumn = -1;

    using spike::Query;
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

        void emplace(Entity e, const T& value)
        {
            const std::uint32_t i = e.index();
            if (i >= sparse_.size()) sparse_.resize(i + 1u, kNull);
            sparse_[i] = static_cast<std::uint32_t>(dense_.size());
            dense_.push_back(e);
            data_.push_back(value);
        }

        void remove(Entity e) override
        {
            const std::uint32_t i = e.index();
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
            // Plan-time fact: components outside this mask can never change an
            // entity's partition, so add/remove of them is a pure side-storage op.
            fragmenting_ = Carry ? ~volatile_ : queried_;
            strides_.fill(0);
        }

        void reserve(std::size_t n) { records_.reserve(n); }

        template <typename... Cs>
        Entity create(Cs... cs)
        {
            (registerType<Cs>(), ...);
            const Entity e = entities_.create();
            const Mask has = (Mask{ 0 } | ... | bit<Cs>()) & fragmenting_;
            const std::uint32_t pi = partitionFor(columnsFor(has));
            Partition& p = *partitions_[pi];
            const std::uint32_t row = p.pushRow(e);
            setRecord(e, Record{ pi, row, has });
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
            if (!(fragmenting_ & bit<C>()))   // membership lives in the pool (which may not exist yet)
            {
                auto* pool = side_[typeId<C>()].get();
                return pool ? static_cast<SidePool<C>*>(pool)->find(e) : nullptr;
            }
            const Record& r = records_[e.index()];
            const std::uint32_t t = typeId<C>();
            if (!(r.has & bit<C>())) return nullptr;
            Partition& p = *partitions_[r.partition];
            const std::int8_t c = p.columnOf[t];
            if (c != kNoColumn) return reinterpret_cast<C*>(p.columns[c].at(r.row));
            return side<C>().find(e);
        }

        template <typename C>
        void add(Entity e, C value)
        {
            registerType<C>();
            if (!(fragmenting_ & bit<C>()))
            {
                side<C>().emplace(e, value);   // non-fragmenting: exactly a sparse-set op
                return;
            }
            Record& r = records_[e.index()];
            const Mask newHas = r.has | bit<C>();
            const Mask newCols = columnsFor(newHas);
            if (newCols != part(r).columnsMask)
            {
                moveTo(e, r, newCols, newHas);
            }
            r.has = newHas;
            store(part(r), r.row, e, std::move(value));
        }

        template <typename C>
        void remove(Entity e)
        {
            if (!(fragmenting_ & bit<C>()))
            {
                side<C>().remove(e);           // non-fragmenting: exactly a sparse-set op
                return;
            }
            Record& r = records_[e.index()];
            const std::uint32_t t = typeId<C>();
            const Mask newHas = r.has & ~bit<C>();
            const Mask newCols = columnsFor(newHas);
            if (newCols == part(r).columnsMask)
            {
                side<C>().remove(e);   // not a column: pure side-storage op, no data moves
            }
            else
            {
                if (part(r).columnOf[t] == kNoColumn) side<C>().remove(e);
                moveTo(e, r, newCols, newHas);
            }
            r.has = newHas;
        }

        void destroy(Entity e)
        {
            Record& r = records_[e.index()];
            Partition& p = part(r);
            Mask sideBits = r.has & ~p.columnsMask;
            while (sideBits)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(sideBits));
                side_[t]->remove(e);
                sideBits &= sideBits - 1u;
            }
            for (std::uint32_t t : nonFragmentingTypes_) side_[t]->removeIfPresent(e);
            const Entity moved = p.swapRemove(r.row);
            if (!(moved == kNullEntity)) records_[moved.index()].row = r.row;
            r = Record{};
            entities_.release(e);
        }

        void commit() {}

        /** H7 system fusion: one pass per partition applying every system that
         *  matches it, in argument (= schedule) order, row by row.
         *
         *  Legal because each system is row-local (touches only the current
         *  entity's components) and no structural change happens inside the
         *  group, so per-row interleaving equals running the passes back to back.
         *  The subset of systems per partition is resolved once per partition
         *  and dispatched to a compile-time specialised loop, so the inner loop
         *  has no per-row branches on "does system j apply". */
        template <typename... Systems>
        void runFused(const Systems&... systems)
        {
            constexpr std::size_t k = sizeof...(Systems);
            static_assert(k >= 1 && k <= 6, "spike: 2^k loop specialisations");
            static_assert(((indexOf<typename Systems::Query, Qs...>() < kQueries) && ...),
                          "every fused system must use a declared query");
            constexpr std::array<std::size_t, k> qidx{ indexOf<typename Systems::Query, Qs...>()... };

            for (auto& up : partitions_)
            {
                Partition& p = *up;
                const std::size_t n = p.size();
                if (n == 0) continue;
                unsigned subset = 0;
                for (std::size_t j = 0; j < k; ++j)
                {
                    if (p.signature & (1u << qidx[j])) subset |= 1u << j;
                }
                if (subset != 0) dispatchSubset<0>(subset, p, n, systems...);
            }
        }

        std::size_t partitionCount() const { return partitions_.size(); }

    private:
        // ---- fusion helpers ----
        template <unsigned M, typename... Systems>
        void dispatchSubset(unsigned subset, Partition& p, std::size_t n, const Systems&... systems)
        {
            if constexpr (M < (1u << sizeof...(Systems)))
            {
                if (subset == M) fusedLoop<M>(p, n, std::index_sequence_for<Systems...>{}, systems...);
                else dispatchSubset<M + 1>(subset, p, n, systems...);
            }
        }

        // Union of all component types used by the fused systems (deduplicated).
        template <typename... Ts> struct TypeList {};
        template <typename L, typename T> struct Append;
        template <typename... Ts, typename T>
        struct Append<TypeList<Ts...>, T>
        {
            using type = std::conditional_t<(std::is_same_v<T, Ts> || ...), TypeList<Ts...>, TypeList<Ts..., T>>;
        };
        template <typename L, typename Q> struct AppendQuery { using type = L; };
        template <typename L, typename C, typename... Cs>
        struct AppendQuery<L, Query<C, Cs...>>
        {
            using type = typename AppendQuery<typename Append<L, C>::type, Query<Cs...>>::type;
        };
        template <typename L, typename... Qs2> struct UnionOf { using type = L; };
        template <typename L, typename Q, typename... Rest>
        struct UnionOf<L, Q, Rest...>
        {
            using type = typename UnionOf<typename AppendQuery<L, Q>::type, Rest...>::type;
        };

        /** ONE pointer per component type for the whole fused group. Systems that
         *  share a component therefore share the same pointer, so after inlining
         *  the compiler sees a single merged kernel (no false aliasing between
         *  per-system copies of the same column). Types only used by systems that
         *  are inactive for this partition may have no column: nullptr, never read. */
        template <typename... Ts>
        static std::tuple<Ts*...> bindUnion(Partition& p, TypeList<Ts...>)
        {
            return { (p.columnOf[typeId<Ts>()] != kNoColumn
                          ? reinterpret_cast<Ts*>(p.columns[p.columnOf[typeId<Ts>()]].bytes.data())
                          : nullptr)... };
        }

        template <bool Active, typename S, typename Cols, typename... Cs>
        static void step(const S& s, const Cols& cols, std::size_t i, Query<Cs...>)
        {
            if constexpr (Active) s(std::get<Cs*>(cols)[i]...);
        }

        // Fusion only pays if every system body is inlined into one loop body
        // (then the compiler sees a single merged kernel). Left to heuristics,
        // GCC stopped inlining in large TUs (spike_bench) and the fused loop ran
        // at unfused speed, so force it: flatten = inline everything called here.
        template <unsigned M, std::size_t... J, typename... Systems>
#if defined(__GNUC__)
        __attribute__((flatten))
#endif
        void fusedLoop(Partition& p, std::size_t n, std::index_sequence<J...>, const Systems&... systems)
        {
            using Types = typename UnionOf<TypeList<>, typename Systems::Query...>::type;
            const auto cols = bindUnion(p, Types{});
            for (std::size_t i = 0; i < n; ++i)
            {
                (step<((M >> J) & 1u) != 0>(systems, cols, i, typename Systems::Query{}), ...);
            }
        }

        struct Record   // 16 bytes
        {
            std::uint32_t partition = 0;   // index into partitions_
            std::uint32_t row = 0;
            Mask has = 0;                  // the entity's *fragmenting* components only
        };
        static_assert(sizeof(Record) == 16);

        Partition& part(const Record& r) { return *partitions_[r.partition]; }

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
                side_[t] = std::make_unique<SidePool<T>>();
                if (!(fragmenting_ & (Mask{ 1 } << t))) nonFragmentingTypes_.push_back(t);
            }
        }

        template <typename T>
        SidePool<T>& side() { return static_cast<SidePool<T>&>(*side_[typeId<T>()]); }

        template <typename T>
        void store(Partition& p, std::uint32_t row, Entity e, T value)
        {
            const std::int8_t c = p.columnOf[typeId<T>()];
            if (c != kNoColumn) std::memcpy(p.columns[c].at(row), &value, sizeof(T));
            else side<T>().emplace(e, value);
        }

        void setRecord(Entity e, Record r)
        {
            if (e.index() >= records_.size()) records_.resize(e.index() + 1u);
            records_[e.index()] = r;
        }

        /** Move e to the partition for newCols; values migrate column<->side as needed. */
        void moveTo(Entity e, Record& r, Mask newCols, Mask newHas)
        {
            const std::uint32_t di = partitionFor(newCols);
            Partition& src = part(r);
            Partition& dst = *partitions_[di];
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
                    SidePoolBase& pool = *side_[col.type];
                    std::memcpy(col.at(dstRow), pool.findRaw(e), col.stride);
                    pool.remove(e);
                }
                // else: component being added; caller stores it after the move
            }
            for (auto& col : src.columns)
            {
                if (dst.columnOf[col.type] == kNoColumn && (newHas & (Mask{ 1 } << col.type)))
                {
                    side_[col.type]->emplaceRaw(e, col.at(srcRow));   // demote: column -> side storage
                }
            }

            const Entity moved = src.swapRemove(srcRow);
            if (!(moved == kNullEntity)) records_[moved.index()].row = srcRow;
            r.partition = di;
            r.row = dstRow;
        }

        std::uint32_t partitionFor(Mask cols)
        {
            auto it = byColumns_.find(cols);
            if (it != byColumns_.end()) return it->second;

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
            const auto index = static_cast<std::uint32_t>(partitions_.size());
            byColumns_.emplace(cols, index);
            partitions_.push_back(std::move(p));
            return index;
        }

        std::array<Mask, kQueries> required_;
        Mask queried_ = 0;
        Mask volatile_ = 0;
        Mask fragmenting_ = 0;
        std::vector<std::uint32_t> nonFragmentingTypes_;
        std::array<std::size_t, kMaxTypes> strides_{};
        std::array<std::unique_ptr<SidePoolBase>, kMaxTypes> side_{};

        EntityAllocator entities_;
        std::vector<Record> records_;
        std::vector<std::unique_ptr<Partition>> partitions_;
        std::unordered_map<Mask, std::uint32_t> byColumns_;
        std::array<std::vector<Partition*>, kQueries> byQuery_{};
    };

    /** The spike workload's system set (scenarios.hpp). */
    using SpikeQueries = std::tuple<Query<Position>, Query<Position, Velocity>, Query<Health, Rotation>,
                                    Query<Scale, Color>, Query<Position, Velocity, Tag>>;

    using World = BasicWorld<false, SpikeQueries>;
    using HintedWorld = BasicWorld<true, SpikeQueries, Volatile<Frozen>>;

} // namespace spike::qpart
