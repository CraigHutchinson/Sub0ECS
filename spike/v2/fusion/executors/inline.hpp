#pragma once
/** Inline executors: run the fused kernel on the calling thread, over all
 *  rows (Inline, the H7 fused loop) or over one row range (InlineRange, used by
 *  runFusedParallel to hand chunks of many partitions to one kernel). */

#include <cstddef>

#include "contract.hpp"

namespace spike::fusion
{
    struct Inline
    {
        static constexpr const char* kName = "Inline";
        static constexpr bool kRequiresDeviceSafe = false;
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel) { kernel(cols, n); }
    };

    /** Runs the kernel on a sub-range of rows: lets a pool hand chunks of many
     *  partitions to one fused kernel (runFusedParallel). */
    struct InlineRange
    {
        static constexpr bool kRequiresDeviceSafe = false;
        std::size_t begin = 0, end = 0;
        template <typename Info, typename Cols, typename K>
        void run(std::size_t, const Cols& cols, K& kernel) { kernel(offsetCols(cols, begin), end - begin); }
    };

} // namespace spike::fusion
