#pragma once
/** ShareColumns: the sharing rule. Extend the current group while the next system
 *  shares a column with it. The recommended static default. */

#include <array>
#include <cstddef>
#include <tuple>
#include <utility>

#include "../access.hpp"

namespace sub0ecs::fusion
{
    /** Row of the share matrix for system A against all S (kept separate: writing
     *  kShares<tuple_element_t<I, T>, S>... inline would expand I and S in
     *  lockstep, silently producing kShares<Sj, Sj> == true everywhere). */
    template <typename A, typename... S>
    constexpr std::array<bool, sizeof...(S)> shareRow() { return { kShares<A, S>... }; }

    template <typename... S>
    constexpr auto shareMatrix()
    {
        using T = std::tuple<S...>;
        constexpr std::size_t K = sizeof...(S);
        std::array<std::array<bool, K>, K> m{};
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            ((m[I] = shareRow<std::tuple_element_t<I, T>, S...>()), ...);
        }(std::make_index_sequence<K>{});
        return m;
    }

    /** Extend the current group while the next system shares a column with it. */
    struct ShareColumns
    {
        static constexpr const char* kName = "ShareColumns";
        template <typename... S>
        static constexpr auto plan()
        {
            constexpr std::size_t K = sizeof...(S);
            constexpr auto m = shareMatrix<S...>();
            std::array<bool, K> b{};
            std::size_t start = 0;
            for (std::size_t j = 0; j < K; ++j)
            {
                bool shares = false;
                for (std::size_t i = start; i < j; ++i) shares = shares || m[i][j];
                if (j == 0 || !shares)
                {
                    b[j] = true;
                    start = j;
                }
            }
            return b;
        }
    };

} // namespace sub0ecs::fusion
