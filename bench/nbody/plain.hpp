#pragma once
#include <algorithm>
#include "nbody/body.hpp"
#include "nbody/compact_sources.hpp"
namespace bench::nbody
{
/** Plain contiguous reference: snapshot and complete evolving tick are timed. */
template <bool Compact = false>
class BasicPlain
{
public:
    explicit BasicPlain(std::size_t size) : bodies_(size), input_(Compact ? 0 : size), sources_(Compact ? size : 0)
    {
        for (std::size_t i = 0; i < size; ++i) bodies_[i] = initial(i);
    }
    void tick()
    {
        prepareSources();
        for (std::size_t i = 0; i < bodies_.size(); ++i) bodies_[i] = advance(i);
        ++ticks_;
    }
    /** Handwritten parallel control: the same snapshot, pool and grain as ECS.
     * Each task owns disjoint target bodies; source reduction order is unchanged.
     */
    template <std::size_t Rows = 1024, typename Pool>
    void tick(Pool& pool)
    {
        static_assert(Rows > 0);
        prepareSources();
        const auto n = bodies_.size();
        const auto chunks = n == 0 ? 0 : 1 + (n - 1) / Rows;
        if (chunks < 4)
        {
            for (std::size_t i = 0; i < n; ++i) bodies_[i] = advance(i);
        }
        else
        {
            auto update = [&](std::size_t item, unsigned) {
                const auto begin = item * Rows;
                const auto end = begin + std::min(Rows, n - begin);
                for (auto i = begin; i < end; ++i) bodies_[i] = advance(i);
            };
            pool.parallelFor(chunks, update);
        }
        ++ticks_;
    }
    const std::vector<Body>& state() const { return bodies_; }
    std::size_t ticks() const { return ticks_; }
private:
    void prepareSources()
    {
        if constexpr (Compact)
        {
            for (std::size_t i = 0; i < bodies_.size(); ++i) sources_.set(i, bodies_[i].position, bodies_[i].mass);
        }
        else input_ = bodies_;
    }
    Body advance(std::size_t row) const
    {
        if constexpr (Compact) return sources_.advance(bodies_[row], row);
        else return advanceBody(input_, row);
    }
    std::vector<Body> bodies_, input_;
    CompactSources sources_;
    std::size_t ticks_{};
};

using Plain = BasicPlain<false>;
using CompactPlain = BasicPlain<true>;
}
