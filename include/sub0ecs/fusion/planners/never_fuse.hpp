#pragma once
/** NeverFuse: every system is its own group — the unfused baseline and the
 *  debugging fallback. */

#include <array>

namespace sub0ecs::fusion
{
    struct NeverFuse
    {
        static constexpr const char* kName = "NeverFuse";
        template <typename... S>
        static constexpr auto plan()
        {
            std::array<bool, sizeof...(S)> b{};
            b.fill(true);
            return b;
        }
    };

} // namespace sub0ecs::fusion
