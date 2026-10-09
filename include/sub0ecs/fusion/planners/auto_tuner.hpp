#pragma once
/** AutoTuner<Planners...>: FFTW-style "measure" planner — times each
 *  compile-time candidate in the live workload and keeps the fastest. */

#include <array>
#include <chrono>
#include <cstddef>
#include <limits>
#include <utility>

#include "sub0ecs/fusion/planners/plan.hpp"

namespace sub0ecs::fusion
{
    /** Picks a plan by timing the candidates in the live workload.
     *  @tparam Planners The candidate planners.
     *  @note Use one tuner per schedule: it decides once, after kTrialsPerCandidate
     *        runs of each candidate, and keeps that choice. */
    template <typename... Planners>
    class AutoTuner
    {
    public:
        static constexpr std::size_t kCandidates = sizeof...(Planners);
        static constexpr int kTrialsPerCandidate = 5;

        /** Runs the schedule once: under the next candidate while measuring, under the
         *  chosen plan afterwards.
         *  @param w       The world.
         *  @param exec    The executor each group runs on.
         *  @param host    The fallback executor for groups exec may not run.
         *  @param systems The systems, in schedule order; the same on every call. */
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

        /** Tells whether measuring is over.
         *  @return true once every candidate has had its trials. */
        [[nodiscard]] bool decided() const { return trial_ >= kCandidates * kTrialsPerCandidate; }

        /** Reports the chosen candidate.
         *  @return Its index among Planners; meaningful once decided(). */
        [[nodiscard]] std::size_t chosen() const { return chosen_; }

        /** Lists the candidates' names.
         *  @return Planners::kName, in order. */
        [[nodiscard]] static constexpr std::array<const char*, kCandidates> names() { return { Planners::kName... }; }

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

} // namespace sub0ecs::fusion
