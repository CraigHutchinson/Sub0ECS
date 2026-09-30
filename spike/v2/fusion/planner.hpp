#pragma once
/** Fusion extension point 1 — the PLANNER: which consecutive systems fuse.
 *
 * A planner is a type with
 *     template <typename... S> static constexpr std::array<bool, sizeof...(S)> plan();
 * returning "system i starts a new group". Groups are runs of consecutive
 * systems (schedule order is preserved; reordering is a later extension).
 *
 * Compile-time policies (zero runtime cost, static/embedded friendly):
 *   NeverFuse, AlwaysFuse, ShareColumns (the H7 rule G1/G2),
 *   DeviceAware<Base> (never mix device-safe and host-only systems in a group).
 *
 * Runtime decision (FFTW-style "measure"): AutoTuner<Planners...> enumerates
 * candidate plans at compile time and picks one by timing them in the live
 * workload. This is safe *because every legal plan produces bit-identical
 * results* (fusion legality L1–L6), so trial runs and switching plans never
 * change the simulation.
 */

#include <array>
#include <chrono>
#include <cstddef>
#include <limits>
#include <tuple>
#include <utility>

#include "access.hpp"

namespace spike::fusion
{
    // ---- compile-time planners -------------------------------------------------

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

    /** H7 rule: extend the current group while the next system shares a column with it. */
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

    /** Capability-aware refinement: split wherever device-safety changes, so a
     *  group is either entirely device-safe (offloadable) or entirely host-only. */
    template <typename Base>
    struct DeviceAware
    {
        static constexpr const char* kName = "DeviceAware";
        template <typename... S>
        static constexpr auto plan()
        {
            auto b = Base::template plan<S...>();
            constexpr std::array<bool, sizeof...(S)> safe{ kDeviceSafe<S>... };
            for (std::size_t i = 1; i < sizeof...(S); ++i)
                if (safe[i] != safe[i - 1]) b[i] = true;
            return b;
        }
    };

    // ---- plan materialisation --------------------------------------------------

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

    // ---- runtime decision: measure-and-choose ---------------------------------

    template <typename... Planners>
    class AutoTuner
    {
    public:
        static constexpr std::size_t kCandidates = sizeof...(Planners);
        static constexpr int kTrialsPerCandidate = 5;

        template <typename World, typename Exec, typename Host, typename... S>
        void run(World& w, Exec& exec, Host& host, const S&... systems)
        {
            if (trial_ < kCandidates * kTrialsPerCandidate)
            {
                const std::size_t c = trial_ % kCandidates;
                const auto t0 = std::chrono::steady_clock::now();
                dispatch(c, w, exec, host, systems...);
                const double t = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
                // median-ish: keep the minimum (robust to one-off noise)
                if (t < best_[c]) best_[c] = t;
                if (++trial_ == kCandidates * kTrialsPerCandidate)
                {
                    chosen_ = 0;
                    for (std::size_t i = 1; i < kCandidates; ++i)
                        if (best_[i] < best_[chosen_]) chosen_ = i;
                }
                return;
            }
            dispatch(chosen_, w, exec, host, systems...);
        }

        bool decided() const { return trial_ >= kCandidates * kTrialsPerCandidate; }
        std::size_t chosen() const { return chosen_; }
        static constexpr std::array<const char*, kCandidates> names() { return { Planners::kName... }; }

    private:
        template <typename World, typename Exec, typename Host, typename... S>
        void dispatch(std::size_t c, World& w, Exec& exec, Host& host, const S&... systems)
        {
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                (void)((c == I ? (runPlanned<Planners>(w, exec, host, systems...), 0) : 0), ...);
            }(std::index_sequence_for<Planners...>{});
        }

        std::size_t trial_ = 0;
        std::size_t chosen_ = 0;
        std::array<double, kCandidates> best_ = [] {
            std::array<double, kCandidates> a{};
            a.fill(std::numeric_limits<double>::max());
            return a;
        }();
    };

} // namespace spike::fusion
