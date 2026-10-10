#pragma once
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
    const std::vector<Body>& state() const { return bodies_; }
    std::size_t ticks() const { return ticks_; }
private:
    std::vector<Body> bodies_, input_;
    std::size_t ticks_{};
};

}
