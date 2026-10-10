#pragma once
/** The store: query-signature partitions ("automatic archetypes").
 *
 *   store/world.hpp        World / BasicWorld: entities, components, queries,
 *                          fused and parallel iteration, dynamic queries
 *   store/volatile.hpp     Volatile<Cs...>: the churn hint
 *   store/dynamic_query.hpp DynamicQuery<Cs...>: handle of a query added at runtime
 *   store/partition.hpp    Partition: rows matching one set of declared queries
 *   store/column.hpp       Column: one dense component column of a partition
 *   store/side_pool.hpp    SidePool<T>: sparse-set storage for non-column components
 *   store/type_index.hpp   TypeIndices<Owner>: per-World-type component numbering
 *   store/mask.hpp         Mask and the per-World type limit
 *
 * Start with store/world.hpp; the rest are its building blocks.
 */

#include "sub0ecs/store/column.hpp"
#include "sub0ecs/store/dynamic_query.hpp"
#include "sub0ecs/store/mask.hpp"
#include "sub0ecs/store/partition.hpp"
#include "sub0ecs/store/side_pool.hpp"
#include "sub0ecs/store/type_index.hpp"
#include "sub0ecs/store/volatile.hpp"
#include "sub0ecs/store/row_grain.hpp"
#include "sub0ecs/store/world.hpp"
