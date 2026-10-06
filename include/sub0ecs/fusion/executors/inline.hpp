#pragma once
/** Inline executors: run the fused kernel on the calling thread, over all
 *  rows (Inline, the fused loop) or over one row range (InlineRange, used by
 *  runFusedParallel to hand chunks of many partitions to one kernel). */

#include <cstddef>

#include "sub0ecs/fusion/executors/contract.hpp"

namespace sub0ecs::fusion
{
    /** Runs the kernel once, over all rows, on the calling thread. */
    struct Inline
    {
        static constexpr const char* kName = "Inline";
        static constexpr bool kRequiresDeviceSafe = false;
        /** Runs a fused group's kernel over a partition's rows.
         *  @tparam Info  GroupInfo of the group.
         *  @param n      The number of rows.
         *  @param cols   One column pointer per component type of the group (null
         *                where the partition has no such column).
         *  @param kernel Called as kernel(cols, count) over rows [0, count). */
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel) { kernel(cols, n); }
    };

    /** Runs the kernel on a sub-range of rows: lets a pool hand chunks of many
     *  partitions to one fused kernel (runFusedParallel). */
    struct InlineRange
    {
        static constexpr const char* kName = "InlineRange";
        static constexpr bool kRequiresDeviceSafe = false;
        std::size_t begin = 0;   ///< First row of the range.
        std::size_t end = 0;     ///< One past the last row.
        /** Runs a fused group's kernel over rows [begin, end) of a partition.
         *  @tparam Info  GroupInfo of the group.
         *  @param cols   One column pointer per component type of the group.
         *  @param kernel Called once, as kernel(cols + begin, end - begin). */
        template <typename Info, typename Cols, typename K>
        void run(std::size_t, const Cols& cols, K& kernel) { kernel(offsetCols(cols, begin), end - begin); }
    };

} // namespace sub0ecs::fusion
