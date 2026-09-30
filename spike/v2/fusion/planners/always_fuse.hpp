#pragma once
/** AlwaysFuse: one group for the whole schedule. Wins when systems share
 *  columns; loses (~0.8x on Frame3) when they don't. */

#include <array>

namespace spike::fusion
{
    struct AlwaysFuse
    {
        static constexpr const char* kName = "AlwaysFuse";
        template <typename... S>
        static constexpr auto plan()
        {
            std::array<bool, sizeof...(S)> b{};
            if constexpr (sizeof...(S) > 0) b[0] = true;
            return b;
        }
    };

} // namespace spike::fusion
