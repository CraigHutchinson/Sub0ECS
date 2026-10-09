#pragma once
/** The library store (sub0ecs::store) as a benchmark candidate, declared with the
 *  benchmark workload's query set (common/scenarios.hpp).
 *
 *   QueryPart    pure automatic: unqueried components live in side storage
 *   QPartHinted  the recommended model: unqueried components ride along as
 *                dense columns unless declared Volatile (here: Frozen)
 */

#include <tuple>

#include "sub0ecs/store.hpp"

#include "../common/components.hpp"

namespace bench::qpart
{
    using sub0ecs::store::BasicWorld;
    using sub0ecs::store::Volatile;

    using BenchQueries = std::tuple<Query<Position>, Query<Position, Velocity>, Query<Health, Rotation>,
                                    Query<Scale, Color>, Query<Position, Velocity, Tag>>;

    /** The store with the adapter surface every benchmark design has
     *  (common/scenarios.hpp): a name, capability flags, and commit(), which the
     *  store does not need because its structural changes are immediate. */
    template <bool Carry, typename Queries, typename Volatiles = Volatile<>>
    struct Adapter : BasicWorld<Carry, Queries, Volatiles>
    {
        static constexpr const char* kName = Carry ? "QPartHinted" : "QueryPart";
        static constexpr bool kSupportsRemove = true;
        static constexpr bool kSupportsDestroy = true;
        void commit() {}
    };

    using World = Adapter<false, BenchQueries>;
    using HintedWorld = Adapter<true, BenchQueries, Volatile<Frozen>>;
} // namespace bench::qpart
