#pragma once
#include <algorithm>
#include "nbody/body.hpp"
namespace bench::nbody
{
/** Plain contiguous reference: snapshot and complete evolving tick are timed. */
class Plain
{
public:
    explicit Plain(std::size_t size) : bodies_(size), input_(size)
    {
        for (std::size_t i = 0; i < size; ++i) bodies_[i] = initial(i);
    }
    void tick()
    {
        input_ = bodies_;
        for (std::size_t i = 0; i < bodies_.size(); ++i) bodies_[i] = advanceBody(input_, i);
        ++ticks_;
    }
    /** Handwritten parallel control: the same snapshot, pool and grain as ECS.
     * Each task owns disjoint target bodies; source reduction order is unchanged.
     */
    template <std::size_t Rows = 1024, typename Pool>
    void tick(Pool& pool)
    {
        static_assert(Rows > 0);
        input_ = bodies_;
        const auto n = bodies_.size();
        const auto chunks = n == 0 ? 0 : 1 + (n - 1) / Rows;
        if (chunks < 4)
        {
            for (std::size_t i = 0; i < n; ++i) bodies_[i] = advanceBody(input_, i);
        }
        else
        {
            auto update = [&](std::size_t item, unsigned) {
                const auto begin = item * Rows;
                const auto end = begin + std::min(Rows, n - begin);
                for (auto i = begin; i < end; ++i) bodies_[i] = advanceBody(input_, i);
            };
            pool.parallelFor(chunks, update);
        }
        ++ticks_;
    }
    const std::vector<Body>& state() const { return bodies_; }
    std::size_t ticks() const { return ticks_; }
private:
    std::vector<Body> bodies_, input_;
    std::size_t ticks_{};
};

}
