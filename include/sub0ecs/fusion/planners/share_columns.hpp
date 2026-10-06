#pragma once
/** ShareColumns: the sharing rule. Extend the current group while the next system
 *  shares a column with it. The recommended static default. */

#include <array>
#include <cstddef>
#include <tuple>
#include <utility>

#include "sub0ecs/fusion/access.hpp"

namespace sub0ecs::fusion
{
    /** Row of the share matrix for system A against all S (kept separate: writing
     *  kShares<tuple_element_t<I, T>, S>... inline would expand I and S in
     *  lockstep, silently producing kShares<Sj, Sj> == true everywhere).
     *  @tparam A The system the row is for.
     *  @tparam S Every system of the schedule.
     *  @return One flag per system: true = it shares a component with A. */
    template <typename A, typename... S>
    [[nodiscard]] constexpr std::array<bool, sizeof...(S)> shareRow() { return { kShares<A, S>... }; }

    /** Builds the "shares a component" matrix of a schedule.
     *  @tparam S The systems, in schedule order.
     *  @return m[i][j] = systems i and j share a component. */
    template <typename... S>
    [[nodiscard]] constexpr auto shareMatrix()
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
        /** Decides where groups start.
         *  @tparam S The systems, in schedule order.
         *  @return One flag per system: true = this system starts a new group. */
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
