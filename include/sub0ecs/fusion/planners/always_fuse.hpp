#pragma once
/** AlwaysFuse: one group for the whole schedule. Wins when systems share
 *  columns; loses when they don't. */

#include <array>

namespace sub0ecs::fusion
{
    /** Puts the whole schedule in one group. */
    struct AlwaysFuse
    {
        static constexpr const char* kName = "AlwaysFuse";
        /** Decides where groups start.
         *  @tparam S The systems, in schedule order.
         *  @return One flag per system: true = this system starts a new group. */
        template <typename... S>
        static constexpr auto plan()
        {
            std::array<bool, sizeof...(S)> b{};
            if constexpr (sizeof...(S) > 0) b[0] = true;
            return b;
        }
    };

} // namespace sub0ecs::fusion
