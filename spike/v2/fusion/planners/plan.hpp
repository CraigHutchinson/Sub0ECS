#pragma once
/** Plan materialisation and runPlanned: turns a planner's group starts into
 *  groups and runs each on the executor, with host fallback for groups that
 *  are not device-safe. */

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../access.hpp"

namespace spike::fusion
{
    template <typename Planner, typename... S>
    struct Plan
    {
        static constexpr std::size_t K = sizeof...(S);
        static constexpr auto starts = Planner::template plan<S...>();
        static constexpr auto groupOf = [] {
            std::array<int, K> g{};
            int cur = -1;
            for (std::size_t i = 0; i < K; ++i)
            {
                if (starts[i] || cur < 0) ++cur;
                g[i] = cur;
            }
            return g;
        }();
        static constexpr int kGroups = K == 0 ? 0 : groupOf[K - 1] + 1;
    };

    template <typename P, int G>
    struct GroupMembers
    {
        struct Sel
        {
            std::array<std::size_t, P::K> idx{};
            std::size_t n = 0;
        };
        static constexpr Sel value = [] {
            Sel s;
            for (std::size_t i = 0; i < P::K; ++i)
                if (P::groupOf[i] == G) s.idx[s.n++] = i;
            return s;
        }();
    };

    /** Executor chooser: which executor runs a group. Default: the given one
     *  for device-safe groups, the host fallback otherwise. */
    template <typename P, int G, typename Tup>
    constexpr bool groupDeviceSafe()
    {
        constexpr auto sel = GroupMembers<P, G>::value;
        return [&]<std::size_t... J>(std::index_sequence<J...>) {
            return (kDeviceSafe<std::remove_cvref_t<std::tuple_element_t<sel.idx[J], Tup>>> && ...);
        }(std::make_index_sequence<sel.n>{});
    }

    template <typename P, int G, typename World, typename Exec, typename Host, typename Tup>
    void runGroup(World& w, Exec& exec, Host& host, Tup& tup)
    {
        constexpr auto sel = GroupMembers<P, G>::value;
        [&]<std::size_t... J>(std::index_sequence<J...>) {
            if constexpr (Exec::kRequiresDeviceSafe && !groupDeviceSafe<P, G, Tup>())
                w.runFusedOn(host, std::get<sel.idx[J]>(tup)...);   // capability fallback
            else
                w.runFusedOn(exec, std::get<sel.idx[J]>(tup)...);
        }(std::make_index_sequence<sel.n>{});
    }

    /** Run systems under a compile-time planner on an executor (host fallback for non-device-safe groups). */
    template <typename Planner, typename World, typename Exec, typename Host, typename... S>
    void runPlanned(World& w, Exec& exec, Host& host, const S&... systems)
    {
        using P = Plan<Planner, S...>;
        auto tup = std::forward_as_tuple(systems...);
        [&]<int... G>(std::integer_sequence<int, G...>) {
            (runGroup<P, G>(w, exec, host, tup), ...);
        }(std::make_integer_sequence<int, P::kGroups>{});
    }

} // namespace spike::fusion
