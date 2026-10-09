#pragma once
/** Plan materialisation and runPlanned: turns a planner's group starts into
 *  groups and runs each on the executor, with host fallback for groups that
 *  are not device-safe. */

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "sub0ecs/fusion/access.hpp"

namespace sub0ecs::fusion
{
    /** A planner's decision for one schedule, as compile-time tables.
     *  @tparam Planner The planner.
     *  @tparam S       The systems, in schedule order. */
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

    /** The systems of one group of a plan.
     *  @tparam P A Plan.
     *  @tparam G The group's index. */
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

    /** Tells whether every system of a group may run on a device.
     *  @tparam P   A Plan.
     *  @tparam G   The group's index.
     *  @tparam Tup std::tuple of the schedule's system types.
     *  @return true when all of the group's systems are device-safe. */
    template <typename P, int G, typename Tup>
    [[nodiscard]] constexpr bool groupDeviceSafe()
    {
        constexpr auto sel = GroupMembers<P, G>::value;
        return [&]<std::size_t... J>(std::index_sequence<J...>) {
            return (kDeviceSafe<std::remove_cvref_t<std::tuple_element_t<sel.idx[J], Tup>>> && ...);
        }(std::make_index_sequence<sel.n>{});
    }

    /** Runs one group of a plan: on exec, or on host when exec needs device-safe
     *  systems and the group has one that is not.
     *  @tparam P  A Plan.
     *  @tparam G  The group's index.
     *  @param w    The world.
     *  @param exec The executor.
     *  @param host The fallback executor for host-only groups.
     *  @param tup  The schedule's systems. */
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

    /** Runs a schedule under a compile-time planner.
     *  @tparam Planner The planner that groups the systems.
     *  @param w       The world.
     *  @param exec    The executor each group runs on.
     *  @param host    The fallback executor for groups exec may not run.
     *  @param systems The systems, in schedule order. */
    template <typename Planner, typename World, typename Exec, typename Host, typename... S>
    void runPlanned(World& w, Exec& exec, Host& host, const S&... systems)
    {
        using P = Plan<Planner, S...>;
        auto tup = std::forward_as_tuple(systems...);
        [&]<int... G>(std::integer_sequence<int, G...>) {
            (runGroup<P, G>(w, exec, host, tup), ...);
        }(std::make_integer_sequence<int, P::kGroups>{});
    }

} // namespace sub0ecs::fusion
