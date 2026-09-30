#pragma once
/** The v2 store (sub0ecs::store) as a benchmark candidate, declared with the
 *  benchmark workload's query set (common/scenarios.hpp).
 *
 *   QueryPart    pure automatic: unqueried components live in side storage
 *   QPartHinted  the recommended v2 model: unqueried components ride along as
 *                dense columns unless declared Volatile (here: Frozen)
 */

#include <tuple>

#include <sub0ecs/store.hpp>

#include "../common/components.hpp"

namespace bench::qpart
{
    using sub0ecs::store::BasicWorld;
    using sub0ecs::store::Volatile;

    using SpikeQueries = std::tuple<Query<Position>, Query<Position, Velocity>, Query<Health, Rotation>,
                                    Query<Scale, Color>, Query<Position, Velocity, Tag>>;

    using World = BasicWorld<false, SpikeQueries>;
    using HintedWorld = BasicWorld<true, SpikeQueries, Volatile<Frozen>>;
} // namespace bench::qpart
