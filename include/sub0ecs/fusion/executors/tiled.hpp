#pragma once
/** Tiled<N>: fixed-size row tiles — the shape of DMA double-buffering,
 *  cache blocking and work splitting. */

#include <algorithm>
#include <cstddef>

#include "sub0ecs/fusion/executors/contract.hpp"

namespace sub0ecs::fusion
{
    /** Runs the kernel tile by tile on the calling thread.
     *  @tparam Tile Rows per tile; at least 1. */
    template <std::size_t Tile>
    struct Tiled
    {
        static_assert(Tile > 0, "Tiled<0> would never advance");
        static constexpr const char* kName = "Tiled";
        static constexpr bool kRequiresDeviceSafe = false;
        /** Runs a fused group's kernel over a partition's rows.
         *  @tparam Info  GroupInfo of the group.
         *  @param n      The number of rows.
         *  @param cols   One column pointer per component type of the group (null
         *                where the partition has no such column).
         *  @param kernel Called as kernel(cols, count) over rows [0, count). */
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            for (std::size_t b = 0; b < n; b += Tile) kernel(offsetCols(cols, b), std::min(Tile, n - b));
        }
    };

} // namespace sub0ecs::fusion
