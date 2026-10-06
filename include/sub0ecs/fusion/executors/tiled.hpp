#pragma once
/** Tiled<N>: fixed-size row tiles — the shape of DMA double-buffering,
 *  cache blocking and work splitting. */

#include <algorithm>
#include <cstddef>

#include "contract.hpp"

namespace sub0ecs::fusion
{
    template <std::size_t Tile>
    struct Tiled
    {
        static_assert(Tile > 0, "Tiled<0> would never advance");
        static constexpr const char* kName = "Tiled";
        static constexpr bool kRequiresDeviceSafe = false;
        template <typename Info, typename Cols, typename K>
        void run(std::size_t n, const Cols& cols, K& kernel)
        {
            for (std::size_t b = 0; b < n; b += Tile) kernel(offsetCols(cols, b), std::min(Tile, n - b));
        }
    };

} // namespace sub0ecs::fusion
