#pragma once
/** NeverFuse: every system is its own group — the unfused baseline and the
 *  debugging fallback. */

#include <array>

namespace sub0ecs::fusion
{
    /** Puts every system in a group of its own. */
    struct NeverFuse
    {
        static constexpr const char* kName = "NeverFuse";
        /** Decides where groups start.
         *  @tparam S The systems, in schedule order.
         *  @return One flag per system: true = this system starts a new group. */
        template <typename... S>
        static constexpr auto plan()
        {
            std::array<bool, sizeof...(S)> b{};
            b.fill(true);
            return b;
        }
    };

} // namespace sub0ecs::fusion
