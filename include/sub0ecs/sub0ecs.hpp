#pragma once
/** SubzeroECS: everything in one include.
 *
 *   entity.hpp          Entity handle, EntityAllocator
 *   query.hpp           Query<Cs...>: the components a system requires
 *   store.hpp           the store: World / BasicWorld and its building blocks (store/)
 *   fusion/access.hpp   Read/Write access declarations and capabilities
 *   fusion/planner.hpp  which systems fuse (fusion/planners/)
 *   fusion/executors.hpp where and how a fused group runs (fusion/executors/)
 */

#include "sub0ecs/entity.hpp"
#include "sub0ecs/fusion/access.hpp"
#include "sub0ecs/fusion/executors.hpp"
#include "sub0ecs/fusion/planner.hpp"
#include "sub0ecs/query.hpp"
#include "sub0ecs/store.hpp"
