#pragma once
/** SubzeroECS v2: everything in one include.
 *
 *   entity.hpp          Entity handle, EntityAllocator
 *   query.hpp           Query<Cs...>: the components a system requires
 *   store.hpp           the store: World / BasicWorld and its building blocks (store/)
 *   fusion/access.hpp   Read/Write access declarations and capabilities
 *   fusion/planner.hpp  which systems fuse (fusion/planners/)
 *   fusion/executors.hpp where and how a fused group runs (fusion/executors/)
 */

#include "entity.hpp"
#include "fusion/access.hpp"
#include "fusion/executors.hpp"
#include "fusion/planner.hpp"
#include "query.hpp"
#include "store.hpp"
