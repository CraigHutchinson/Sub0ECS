#pragma once
/** Fusion extension point 2 — the EXECUTOR: where/how a fused group runs.
 *
 * The store hands an executor, per partition:
 *   n      rows,
 *   cols   std::tuple<T*...> — ONE pointer per component type in the group
 *          (nullptr where the partition has no such column),
 *   kernel callable kernel(cols, count) running the fused systems over
 *          rows [0, count) of the given column pointers,
 * plus compile-time Info (Info::kWritten<T>) derived from declared Access.
 *
 * An executor is free to split rows (tiles, threads), move data (DMA to a
 * coprocessor's local memory) or dispatch to another engine, as long as each
 * row is processed exactly once — row-locality (legality L1) makes any
 * split/order of rows produce bit-identical results.
 *
 * One header per executor under executors/:
 *
 *   Inline            call the kernel once (the H7 fused loop)
 *   Tiled<N>          fixed-size tiles: the shape of DMA double-buffering,
 *                     cache blocking and work splitting
 *   Parallel          persistent thread pool, contiguous row chunks
 *   Offload<Dev, N>   emulated coprocessor: stage each tile into device-local
 *                     buffers, launch, copy back only WRITTEN columns;
 *                     requires device-safe systems (capability)
 */

#include "executors/contract.hpp"
#include "executors/inline.hpp"
#include "executors/offload.hpp"
#include "executors/parallel.hpp"
#include "executors/tiled.hpp"
