#pragma once
/** AutoTuner<Planners...>: FFTW-style "measure" planner — times each
 *  compile-time candidate in the live workload and keeps the fastest. */

#include <array>
#include <chrono>
#include <cstddef>
#include <limits>
#include <utility>

#include "plan.hpp"

namespace sub0ecs::fusion
{
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

} // namespace sub0ecs::fusion
