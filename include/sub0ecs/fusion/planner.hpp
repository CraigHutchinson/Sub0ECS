#pragma once
/** Fusion extension point 1 — the PLANNER: which consecutive systems fuse.
 *
 * A planner is a type with
 *     template <typename... S> static constexpr std::array<bool, sizeof...(S)> plan();
 * returning "system i starts a new group". Groups are runs of consecutive
 * systems (schedule order is preserved; reordering is a later extension).
 *
 * One header per planner under planners/.
 *
 * Compile-time policies (zero runtime cost, static/embedded friendly):
 *   NeverFuse, AlwaysFuse, ShareColumns (the sharing rule),
 *   DeviceAware<Base> (never mix device-safe and host-only systems in a group).
 *
 * Runtime decision (FFTW-style "measure"): AutoTuner<Planners...> enumerates
 * candidate plans at compile time and picks one by timing them in the live
 * workload. This is safe *because every legal plan produces bit-identical
 * results* (fusion legality L1–L6), so trial runs and switching plans never
 * change the simulation.
 */

#include "planners/always_fuse.hpp"
#include "planners/auto_tuner.hpp"
#include "planners/device_aware.hpp"
#include "planners/never_fuse.hpp"
#include "planners/plan.hpp"
#include "planners/share_columns.hpp"
