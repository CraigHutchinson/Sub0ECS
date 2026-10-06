#pragma once
/** BasicWorld / World: query-signature partitions ("automatic archetypes").
 *
 * Design and evidence: docs/FINDINGS.md; background in
 * docs/research/holographic-storage.md. The world is told its system
 * queries up front. Physical partitions are keyed by the set of declared
 * queries an entity matches, not by its full component signature.
 *
 * Invariant (columns follow queries): in a partition, the dense columns are
 * the union of the required components of every query the partition
 * matches. Every other component of an entity lives in type-erased side
 * storage (sparse set) and costs nothing for iteration.
 *
 * Consequences:
 *   - add/remove of a component no query requires moves no data
 *   - iteration of a declared query = whole partitions of dense columns
 *
 * Two modes:
 *   Carry = false ("QueryPart")  pure automatic: every unqueried component
 *                                goes to side storage.
 *   Carry = true  ("QPartHinted") the recommended model: unqueried components
 *                                ride along as dense columns (extra key bits)
 *                                unless declared Volatile<...>; trades churn
 *                                cost for memory and find() locality.
 *
 * Handle semantics: operations on a stale (destroyed) handle are no-ops and
 * find() returns nullptr; add() of a component the entity already has
 * overwrites it; remove() of a component it lacks is a no-op.
 *
 * Type indices: every World type numbers its own component types, shared by all
 * its instances. The types known at compile time, queried and Volatile, come
 * first (checked by static_assert to fit the 64 layout bits); every other type is
 * numbered after them on first use, without limit. A type with one of the 64
 * layout bits can be a dense column. A type numbered beyond them is kept in side
 * storage instead: every operation still works, it just is never a column. In
 * carry mode that means the first unqueried types seen are carried and later
 * ones are side-stored; in pure mode unqueried types never need a bit at all.
 *
 * Constraints: trivially copyable components of at most 64 bytes; at most 64
 * queried plus Volatile component types and at most 32 declared queries (both
 * compile-time errors); each<Cs...> must name a declared Query<Cs...> exactly;
 * a query added at runtime (addQuery) must name types that have a layout bit.
 */


#include <array>
#include <bit>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../detail/hints.hpp"
#include "../entity.hpp"
#include "../fusion/executors.hpp"
#include "../query.hpp"
#include "detail/meta.hpp"
#include "mask.hpp"
#include "partition.hpp"
#include "side_pool.hpp"
#include "type_index.hpp"
#include "volatile.hpp"

namespace sub0ecs::store
{
    template <bool Carry, typename Queries, typename Volatiles = Volatile<>>
    class BasicWorld;

    template <bool Carry, typename... Qs, typename... Vs>
    class BasicWorld<Carry, std::tuple<Qs...>, Volatile<Vs...>>
    {
        static constexpr std::size_t kQueries = sizeof...(Qs);
        static_assert(kQueries <= 32, "signature is 32-bit");

        // Component numbering: see "Type indices" above.
        using StaticTypes = typename detail::UnionOf<detail::TypeList<>, Qs..., Query<Vs...>>::type;
        static constexpr std::size_t kStaticTypes = detail::Size<StaticTypes>::value;
        static_assert(kStaticTypes <= kMaxTypes, "more than 64 queried + Volatile component types in one World type");

        using Indices = TypeIndices<BasicWorld>;   // numbering of the types met at runtime, after the static ones

        template <typename T>
        static std::uint32_t typeIndex()
        {
            constexpr std::size_t fixed = detail::IndexIn<T, StaticTypes>::value;
            if constexpr (fixed != ~std::size_t{ 0 }) return static_cast<std::uint32_t>(fixed);
            else return static_cast<std::uint32_t>(kStaticTypes) + Indices::template of<T>();
        }

        /** The layout bit of a type index; no bit (0) for indices beyond the 64 layout bits. */
        static constexpr Mask bitAt(std::uint32_t index) { return index < kMaxTypes ? Mask{ 1 } << index : Mask{ 0 }; }

        template <typename T>
        static Mask bit() { return bitAt(typeIndex<T>()); }

        /** Column of C in partition p, or nullptr when C is not a column there. */
        template <typename C>
        static C* col(const Partition& p)
        {
            const std::uint32_t index = typeIndex<C>();
            return index < kMaxTypes ? reinterpret_cast<C*>(p.base[index]) : nullptr;
        }

    public:
        using Entity = sub0ecs::Entity;
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
        }

        void reserve(std::size_t n) { records_.reserve(n); }

        /** True while e refers to a live entity of this world (false once destroyed). */
        bool alive(Entity e) const { return entities_.alive(e); }

        /** Live entity count. */
        std::size_t size() const { return entities_.liveCount(); }

        template <typename C>
        bool has(Entity e) { return find<C>(e) != nullptr; }

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
            constexpr std::size_t qi = detail::indexOf<Query<Cs...>, Qs...>();
            static_assert(qi < kQueries, "each<Cs...> must name a declared Query<Cs...>");
            for (Partition* p : byQuery_[qi])
            {
                const std::size_t n = p->size();
                if (n == 0) continue;
                std::tuple<Cs*...> cols{ col<Cs>(*p)... };
                for (std::size_t i = 0; i < n; ++i) SUB0ECS_FLATTEN_CALLS f(std::get<Cs*>(cols)[i]...);
            }
        }

        template <typename C>
        C* find(Entity e)
        {
            if (!entities_.alive(e)) return nullptr;
            // The index is resolved once: for a type numbered at runtime each lookup
            // re-checks a thread-safe static, which is measurable in churn-heavy paths.
            const std::uint32_t t = typeIndex<C>();
            const Mask bitC = bitAt(t);
            if (!(fragmenting_ & bitC))   // membership lives in the pool (which may not exist yet)
            {
                auto* sidePool = poolAt(t);
                return sidePool ? static_cast<SidePool<C>*>(sidePool)->find(e) : nullptr;
            }
            const Record& r = records_[e.index()];
            // Column first: columnsFor(has) is a subset of has, so a column
            // implies membership. Checking r.has before this dependent load cost
            // RandomGet ~20% (measured), hence the order. (A fragmenting type has a
            // layout bit, so t indexes the partition's tables.)
            if (std::byte* column = partitions_[r.partition]->base[t]) return reinterpret_cast<C*>(column) + r.row;
            if (!(r.has & bitC))   // runtime query: not yet migrated entities still hold it in side storage
                return (migrating_ & bitC) ? poolOf<C>(t).find(e) : nullptr;
            return poolOf<C>(t).find(e);
        }

        template <typename C>
        void add(Entity e, C value)
        {
            if (!entities_.alive(e)) return;
            const std::uint32_t t = typeIndex<C>();
            const Mask bitC = bitAt(t);
            registerType<C>(t);
            if (!(fragmenting_ & bitC))
            {
                poolOf<C>(t).emplace(e, value);   // non-fragmenting: exactly a sparse-set op
                return;
            }
            Record& r = records_[e.index()];
            const Mask newHas = r.has | bitC;
            const Mask newCols = columnsFor(newHas);
            if (newCols != part(r).columnsMask)
            {
                moveTo(e, r, newCols, newHas, part(r).addEdge[t]);
            }
            r.has = newHas;
            store(part(r), r.row, e, std::move(value));
        }

        template <typename C>
        void remove(Entity e)
        {
            if (!entities_.alive(e)) return;
            const std::uint32_t t = typeIndex<C>();
            const Mask bitC = bitAt(t);
            if (!(fragmenting_ & bitC))
            {
                // non-fragmenting: exactly a sparse-set op (the pool may not exist yet)
                if (auto* sidePool = static_cast<SidePool<C>*>(poolAt(t))) sidePool->removeIfPresent(e);
                return;
            }
            Record& r = records_[e.index()];
            if (!(r.has & bitC))
            {
                if (migrating_ & bitC) poolOf<C>(t).removeIfPresent(e);   // runtime query: unmigrated holder, still side-stored
                return;                                                  // otherwise: absent, nothing to do
            }
            const Mask newHas = r.has & ~bitC;
            const Mask newCols = columnsFor(newHas);
            if (newCols == part(r).columnsMask)
            {
                poolOf<C>(t).remove(e);   // not a column: pure side-storage op, no data moves
            }
            else
            {
                if (part(r).columnOf[t] == kNoColumn) poolOf<C>(t).remove(e);
                moveTo(e, r, newCols, newHas, part(r).removeEdge[t]);
            }
            r.has = newHas;
        }

        void destroy(Entity e)
        {
            if (!entities_.alive(e)) return;   // stale handle: must not release the slot twice
            Record& r = records_[e.index()];
            Partition& p = part(r);
            Mask sideBits = r.has & ~p.columnsMask;
            while (sideBits)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(sideBits));
                side_[t]->remove(e);
                sideBits &= sideBits - 1u;
            }
            for (std::uint32_t t : nonFragmentingTypes_) poolAt(t)->removeIfPresent(e);
            for (Mask m = migrating_; m; m &= m - 1u) side_[std::countr_zero(m)]->removeIfPresent(e);
            const Entity moved = p.swapRemove(r.row);
            if (!(moved == kNullEntity)) records_[moved.index()].row = r.row;
            r = Record{};
            entities_.release(e);
        }

        void commit() {}

        /** System fusion: one pass per partition applying every system that
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
            fusion::Inline exec;
            runFusedOn(exec, systems...);
        }

        /** Fusion with a pluggable executor (fusion/executors.hpp): the store
         *  supplies per-partition columns + the fused kernel; the executor
         *  decides how rows are run (inline, tiled, threads, offload). */
        template <typename Exec, typename... Systems>
        void runFusedOn(Exec& exec, const Systems&... systems)
        {
            runFusedOnMasked(exec, ~0u, systems...);
        }

        /** Runtime enable mask over the group's members (bit j = systems[j]).
         *  A disabled member simply drops out of every partition's subset. */
        template <typename Exec, typename... Systems>
        void runFusedOnMasked(Exec& exec, unsigned enabled, const Systems&... systems)
        {
            constexpr std::size_t k = sizeof...(Systems);
            static_assert(k >= 1 && k <= 6, "at most 6 systems per fused group (2^k loop specialisations)");
            static_assert(((detail::indexOf<typename Systems::Query, Qs...>() < kQueries) && ...),
                          "every fused system must use a declared query");
            constexpr std::array<std::size_t, k> qidx{ detail::indexOf<typename Systems::Query, Qs...>()... };

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
                subset &= enabled;
                if (subset != 0) dispatchSubset<0>(exec, subset, p, n, systems...);
            }
        }

        /** Fused group, data-parallel at CHUNK granularity across ALL partitions.
         *  Work items are (partition, system subset, row range); each worker streams
         *  its chunks through the whole fused chain — no barrier between systems,
         *  one fork-join per group (vs one per partition in runFusedOn(Parallel)). */
        template <typename Pool, typename... Systems>
        void runFusedParallel(Pool& pool, const Systems&... systems)
        {
            constexpr std::size_t k = sizeof...(Systems);
            constexpr std::array<std::size_t, k> qidx{ detail::indexOf<typename Systems::Query, Qs...>()... };
            constexpr std::size_t kChunk = 2048;
            fchunks_.clear();
            for (auto& up : partitions_)
            {
                Partition& p = *up;
                unsigned subset = 0;
                for (std::size_t j = 0; j < k; ++j)
                    if (p.signature & (1u << qidx[j])) subset |= 1u << j;
                if (subset == 0) continue;
                for (std::size_t b = 0, n = p.size(); b < n; b += kChunk)
                    fchunks_.push_back(FChunk{ &p, subset, static_cast<std::uint32_t>(b), static_cast<std::uint32_t>(std::min(n, b + kChunk)) });
            }
            if (fchunks_.size() < 4)
            {
                fusion::Inline exec;
                runFusedOn(exec, systems...);
                return;
            }
            auto body = [&](std::size_t item, unsigned) {
                const FChunk& c = fchunks_[item];
                fusion::InlineRange exec{ c.begin, c.end };
                dispatchSubset<0>(exec, c.subset, *c.p, c.end - c.begin, systems...);
            };
            pool.parallelFor(fchunks_.size(), body);
        }

        /** Data-parallel iteration: fixed-size row chunks across ALL partitions
         *  matching the query become pool work items (one fork-join per system,
         *  not per partition). f(worker, Cs&...) — the worker index lets callers
         *  keep per-thread command buffers / accumulators without locks. */
        template <typename... Cs, typename Pool, typename F>
        void eachParallel(Pool& pool, F&& f)
        {
            constexpr std::size_t qi = detail::indexOf<Query<Cs...>, Qs...>();
            static_assert(qi < kQueries, "eachParallel<Cs...> must name a declared Query<Cs...>");
            constexpr std::size_t kChunk = 1024;
            chunks_.clear();
            for (Partition* p : byQuery_[qi])
            {
                for (std::size_t b = 0, n = p->size(); b < n; b += kChunk)
                    chunks_.push_back(Chunk{ p, static_cast<std::uint32_t>(b), static_cast<std::uint32_t>(std::min(n, b + kChunk)) });
            }
            if (chunks_.size() < 4)   // grain control: too little work to be worth a fork-join
            {
                for (Partition* p : byQuery_[qi])
                {
                    std::tuple<Cs*...> cols{ col<Cs>(*p)... };
                    for (std::size_t i = 0, n = p->size(); i < n; ++i) SUB0ECS_FLATTEN_CALLS f(0u, std::get<Cs*>(cols)[i]...);
                }
                return;
            }
            auto body = [&](std::size_t item, unsigned worker) {
                const Chunk& c = chunks_[item];
                std::tuple<Cs*...> cols{ col<Cs>(*c.p)... };
                for (std::uint32_t i = c.begin; i < c.end; ++i) SUB0ECS_FLATTEN_CALLS f(worker, std::get<Cs*>(cols)[i]...);
            };
            pool.parallelFor(chunks_.size(), body);
        }

        // ---- Runtime queries (systems added while running) ------------------------------------
        //
        // A system registered at runtime (e.g. paged in with a new world region)
        // declares its query here. Components it requires that are currently in
        // side storage (Volatile) are PROMOTED to dense columns: a relayout.
        //   - the query is usable immediately in a DEGRADED mode: fast path over
        //     already-matching partitions + a sparse join over the side pool for
        //     entities not yet migrated (each entity visited exactly once);
        //   - migrateStep(budget) moves at most `budget` entities (all their
        //     pending components at once) per call — bounded, incremental;
        //   - when the side pools drain, the query flips to the full fast path.
        // migrateStep(SIZE_MAX) is the "stall acceptable" (level load) relayout.

        template <typename... Cs>
        std::size_t addQuery()
        {
            static_assert(Carry, "dynamic queries are designed for the hinted (carry) layout");
            (registerType<Cs>(), ...);
            // A query's components become columns, so each needs a layout bit. Queried and
            // Volatile types always have one; a type first seen at runtime has one only
            // while bits remain.
            if (((typeIndex<Cs>() >= kMaxTypes) || ...)) std::abort();
            const Mask req = (Mask{ 0 } | ... | bit<Cs>());
            const Mask promote = req & volatile_;
            if (promote)
            {
                volatile_ &= ~promote;
                fragmenting_ |= promote;
                migrating_ |= promote;
                std::erase_if(nonFragmentingTypes_, [&](std::uint32_t t) { return (promote >> t) & 1u; });
            }
            DynQuery q{ req, promote, {}, true };
            for (auto& p : partitions_)
                if ((p->columnsMask & req) == req) q.partitions.push_back(p.get());
            dyn_.push_back(std::move(q));
            refreshPending();
            return dyn_.size() - 1u;
        }

        /** Scheduler-level enable/disable: no layout change (demotion back to side
         *  storage is only done at stall points). */
        void setQueryEnabled(std::size_t id, bool on) { dyn_[id].enabled = on; }

        bool queryDegraded(std::size_t id) const { return dyn_[id].pending != 0; }

        /** Entities still waiting to be migrated (upper bound: side-pool sizes). */
        std::size_t pendingMigration() const
        {
            std::size_t n = 0;
            for (Mask m = migrating_; m; m &= m - 1u) n += side_[std::countr_zero(m)]->size();
            return n;
        }

        /** Migrate up to `budget` entities; returns entities still pending. */
        std::size_t migrateStep(std::size_t budget)
        {
            std::size_t moved = 0;
            while (migrating_ && moved < budget)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(migrating_));
                SidePoolBase& pool = *side_[t];
                if (pool.size() == 0)
                {
                    migrating_ &= ~(Mask{ 1 } << t);
                    refreshPending();
                    continue;
                }
                promoteEntity(pool.entityAt(pool.size() - 1u));
                ++moved;
            }
            return pendingMigration();
        }

        template <typename... Cs, typename F>
        void eachDyn(std::size_t id, F&& f)
        {
            const DynQuery& q = dyn_[id];
            if (!q.enabled) return;
            for (Partition* p : q.partitions)   // full path: dense columns
            {
                const std::size_t n = p->size();
                if (n == 0) continue;
                std::tuple<Cs*...> cols{ col<Cs>(*p)... };
                for (std::size_t i = 0; i < n; ++i) SUB0ECS_FLATTEN_CALLS f(std::get<Cs*>(cols)[i]...);
            }
            if (q.pending)                      // degraded path: sparse join over unmigrated holders
            {
                const auto d = static_cast<std::uint32_t>(std::countr_zero(q.pending));
                (void)((typeIndex<Cs>() == d ? (compatJoin<Cs, Cs...>(f), true) : false) || ...);
            }
        }

        std::size_t partitionCount() const { return partitions_.size(); }

    private:
        // ---- runtime-query helpers ----
        struct Record   // 16 bytes
        {
            std::uint32_t partition = 0;   // index into partitions_
            std::uint32_t row = 0;
            Mask has = 0;                  // the entity's *fragmenting* components only
        };
        static_assert(sizeof(Record) == 16);

        Partition& part(const Record& r) { return *partitions_[r.partition]; }

        struct DynQuery
        {
            Mask required = 0;
            Mask pending = 0;          // required components still being promoted
            std::vector<Partition*> partitions;
            bool enabled = true;
        };

        void refreshPending()
        {
            for (auto& q : dyn_) q.pending = q.required & migrating_;
        }

        template <typename C>
        bool holds(Entity e, const Record& r)
        {
            if (r.has & bit<C>()) return true;
            auto* sidePool = poolAt(typeIndex<C>());
            return sidePool && sidePool->tryFindRaw(e) != nullptr;
        }

        template <typename C>
        C& refOf(Entity e, const Record& r)
        {
            if (r.has & bit<C>())
            {
                if (C* column = col<C>(*partitions_[r.partition])) return column[r.row];
            }
            return *side<C>().find(e);
        }

        /** Driver = side pool of one pending component; every holder there is unmigrated. */
        template <typename D, typename... Cs, typename F>
        void compatJoin(F& f)
        {
            SidePool<D>& pool = side<D>();
            for (std::size_t i = 0, n = pool.size(); i < n; ++i)
            {
                const Entity e = pool.entityAt(i);
                const Record& r = records_[e.index()];
                if ((holds<Cs>(e, r) && ...)) f(refOf<Cs>(e, r)...);
            }
        }

        /** Move one entity's pending components from side storage into columns
         *  (all at once, so an entity is never half-migrated). */
        void promoteEntity(Entity e)
        {
            Record& r = records_[e.index()];
            Mask pend = 0;
            for (Mask m = migrating_; m; m &= m - 1u)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(m));
                if (const void* src = side_[t]->tryFindRaw(e))
                {
                    std::memcpy(stash_[t].data(), src, strides_[t]);
                    side_[t]->remove(e);
                    pend |= Mask{ 1 } << t;
                }
            }
            assert(pend != 0);   // e was taken from a migrating pool
            const Mask newHas = r.has | pend;
            moveTo(e, r, columnsFor(newHas), newHas, part(r).addEdge[std::countr_zero(pend)]);
            r.has = newHas;
            Partition& p = part(r);
            for (Mask m = pend; m; m &= m - 1u)
            {
                const auto t = static_cast<std::uint32_t>(std::countr_zero(m));
                std::memcpy(p.columns[p.columnOf[t]].at(r.row), stash_[t].data(), strides_[t]);
            }
        }

        // ---- fusion helpers ----
        template <unsigned M, typename Exec, typename... Systems>
        void dispatchSubset(Exec& exec, unsigned subset, Partition& p, std::size_t n, const Systems&... systems)
        {
            if constexpr (M < (1u << sizeof...(Systems)))
            {
                if (subset == M) fusedLoop<M>(exec, p, n, std::index_sequence_for<Systems...>{}, systems...);
                else dispatchSubset<M + 1>(exec, subset, p, n, systems...);
            }
        }

        /** ONE pointer per component type for the whole fused group. Systems that
         *  share a component therefore share the same pointer, so after inlining
         *  the compiler sees a single merged kernel (no false aliasing between
         *  per-system copies of the same column). Types only used by systems that
         *  are inactive for this partition may have no column: nullptr, never read. */
        template <typename... Ts>
        static std::tuple<Ts*...> bindUnion(Partition& p, detail::TypeList<Ts...>)
        {
            return { col<Ts>(p)... };   // nullptr when not a column
        }

        template <bool Active, typename S, typename Cols, typename... Cs>
        static void step(const S& s, const Cols& cols, std::size_t i, Query<Cs...>)
        {
            if constexpr (Active) s(std::get<Cs*>(cols)[i]...);
        }

        // Fusion only pays if every system body is inlined into one loop body
        // (then the compiler sees a single merged kernel). Left to heuristics,
        // GCC stopped inlining in large TUs (sub0ecs_bench) and the fused loop ran
        // at unfused speed, so force it: flatten = inline everything called here.
        template <unsigned M, typename Exec, std::size_t... J, typename... Systems>
        // GCC/Clang only. MSVC's [[msvc::flatten]] was tried (2026-10-01): no consistent gain in
        // the paired fusion benchmarks, and +4 GB / +60 s compiling every file that
        // includes the Skirmish systems (64 subset specialisations x full inlining).
#if defined(__GNUC__)
        __attribute__((flatten))
#endif
        void fusedLoop(Exec& exec, Partition& p, std::size_t n, std::index_sequence<J...>, const Systems&... systems)
        {
            using Types = typename detail::UnionOf<detail::TypeList<>, typename Systems::Query...>::type;
            const auto cols = bindUnion(p, Types{});
            // The fused kernel over rows [0, count) of whatever column pointers
            // the executor passes (original, offset tile, or device-staged copy).
            auto kernel = [&](const auto& c, std::size_t count) {
                for (std::size_t i = 0; i < count; ++i)
                {
                    (step<((M >> J) & 1u) != 0>(systems, c, i, typename Systems::Query{}), ...);
                }
            };
            exec.template run<fusion::GroupInfo<Systems...>>(n, cols, kernel);
        }

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
            // A query matches iff its required set ⊆ columns (proof: docs/research/holographic-storage.md, section 4.1).
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
            registerType<T>(typeIndex<T>());
        }

        /** As above, for a caller that has already resolved T's index. */
        template <typename T>
        void registerType(std::uint32_t t)
        {
            static_assert(std::is_trivially_copyable_v<T>, "sub0ecs::store requires trivially copyable components");
            static_assert(sizeof(T) <= 64, "components must be at most 64 bytes (promotion scratch size)");
            if (t < kMaxTypes)
            {
                if (strides_[t] == 0)
                {
                    strides_[t] = sizeof(T);
                    side_[t] = std::make_unique<SidePool<T>>();
                    if (!(fragmenting_ & bitAt(t))) nonFragmentingTypes_.push_back(t);
                }
                return;
            }
            // Beyond the layout bits: never a column, so only a side pool is needed.
            const std::uint32_t slot = t - kMaxTypes;
            if (slot >= sideBeyond_.size()) sideBeyond_.resize(slot + 1u);
            if (!sideBeyond_[slot])
            {
                sideBeyond_[slot] = std::make_unique<SidePool<T>>();
                nonFragmentingTypes_.push_back(t);
            }
        }

        /** The side pool of a type index, or nullptr if this world has never stored that
         *  type. Indices with a layout bit sit in an inline array (one load, as before the
         *  limit was lifted); the rest in a table that grows with the types seen. */
        SidePoolBase* poolAt(std::uint32_t index) const
        {
            if (index < kMaxTypes) return side_[index].get();
            const std::uint32_t slot = index - kMaxTypes;
            return slot < sideBeyond_.size() ? sideBeyond_[slot].get() : nullptr;
        }

        template <typename T>
        SidePool<T>& side() { return poolOf<T>(typeIndex<T>()); }

        /** T's side pool by its resolved index; the type must have been stored in this world. */
        template <typename T>
        SidePool<T>& poolOf(std::uint32_t index) { return static_cast<SidePool<T>&>(*poolAt(index)); }

        template <typename T>
        void store(Partition& p, std::uint32_t row, Entity e, T value)
        {
            const std::uint32_t t = typeIndex<T>();
            const std::int8_t c = t < kMaxTypes ? p.columnOf[t] : kNoColumn;   // no layout bit: never a column
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
            moveTo(e, r, newHas, partitionFor(newCols));
        }

        /** Single-type transition: resolve the destination through the source
         *  partition's edge cache (hit = no hash lookup). */
        void moveTo(Entity e, Record& r, Mask newCols, Mask newHas, Partition::Edge& edge)
        {
            if (edge.index == Partition::kNoEdge || edge.cols != newCols)
            {
                const std::uint32_t di = partitionFor(newCols);   // may grow partitions_, edge stays valid (Partition is heap-pinned)
                edge = Partition::Edge{ newCols, di };
            }
            moveTo(e, r, newHas, edge.index);
        }

        void moveTo(Entity e, Record& r, Mask newHas, std::uint32_t di)
        {
            Partition& src = part(r);
            Partition& dst = *partitions_[di];
            const std::uint32_t srcRow = r.row;
            const std::uint32_t dstRow = dst.pushRow(e);

            for (auto& col : dst.columns)
            {
                const std::int8_t s = src.columnOf[col.type];
                if (s != kNoColumn)
                {
                    copyRow(col.at(dstRow), src.columns[s].at(srcRow), col.stride);
                }
                else if (r.has & (Mask{ 1 } << col.type))
                {
                    // promote: side storage -> column
                    SidePoolBase& pool = *side_[col.type];
                    copyRow(col.at(dstRow), static_cast<const std::byte*>(pool.findRaw(e)), col.stride);
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
                if (cols & (Mask{ 1 } << t)) p->addColumn(t, strides_[t]);
            }
            for (std::size_t i = 0; i < kQueries; ++i)
            {
                if (p->signature & (1u << i)) byQuery_[i].push_back(p.get());
            }
            for (auto& q : dyn_)
            {
                if ((p->columnsMask & q.required) == q.required) q.partitions.push_back(p.get());
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
        Mask migrating_ = 0;                          // components being promoted to columns
        std::vector<DynQuery> dyn_;                   // runtime-registered queries
        std::array<std::array<std::byte, 64>, kMaxTypes> stash_{};   // promotion scratch (components <= 64 B)
        std::vector<std::uint32_t> nonFragmentingTypes_;
        std::array<std::size_t, kMaxTypes> strides_{};                    // by layout index; 0 = not stored here yet
        std::array<std::unique_ptr<SidePoolBase>, kMaxTypes> side_{};     // by layout index
        std::vector<std::unique_ptr<SidePoolBase>> sideBeyond_;           // types numbered beyond the layout bits

        EntityAllocator entities_;
        std::vector<Record> records_;
        std::vector<std::unique_ptr<Partition>> partitions_;
        std::unordered_map<Mask, std::uint32_t> byColumns_;

        struct Chunk { Partition* p; std::uint32_t begin, end; };
        std::vector<Chunk> chunks_;   // eachParallel scratch
        struct FChunk { Partition* p; unsigned subset; std::uint32_t begin, end; };
        std::vector<FChunk> fchunks_; // runFusedParallel scratch
        std::array<std::vector<Partition*>, kQueries> byQuery_{};
    };

    /** The recommended model: carry mode, with churn-heavy components declared Volatile.
     *
     *   using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Health>>;
     *   sub0ecs::store::World<Queries, sub0ecs::store::Volatile<Selected>> world;
     */
    template <typename Queries, typename Volatiles = Volatile<>>
    using World = BasicWorld<true, Queries, Volatiles>;

} // namespace sub0ecs::store
