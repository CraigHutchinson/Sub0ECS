#pragma once
#include "nbody/body.hpp"
namespace bench::nbody
{
/** Immutable force inputs: omit velocity and compute invariant source scaling once.
 * The per-target reduction retains the oracle's exact source and arithmetic order.
 */
class CompactSources
{
public:
    explicit CompactSources(std::size_t size) : sources_(size) {}
    void set(std::size_t row, const Position& position, const Mass& mass)
    {
        sources_[row] = {position, 0.01 * mass.value};
    }
    Body advance(Body output, std::size_t row) const
    {
        const auto origin = output.position;
        double ax = 0.0, ay = 0.0, az = 0.0;
        auto accumulate = [&](std::size_t other)
        {
            const auto& source = sources_[other];
            const double dx = source.position.x - origin.x;
            const double dy = source.position.y - origin.y;
            const double dz = source.position.z - origin.z;
            const double squared = dx * dx + dy * dy + dz * dz + 1.0 / 16.0;
            const double scale = source.scaledMass / (squared * std::sqrt(squared));
            ax += dx * scale;
            ay += dy * scale;
            az += dz * scale;
        };
        // Split around self while preserving the oracle's ascending source order.
        for (std::size_t other = 0; other < row; ++other) accumulate(other);
        for (std::size_t other = row + 1; other < sources_.size(); ++other) accumulate(other);
        constexpr double kDt = 1.0 / 1024.0;
        output.velocity.x += ax * kDt;
        output.velocity.y += ay * kDt;
        output.velocity.z += az * kDt;
        output.position.x += output.velocity.x * kDt;
        output.position.y += output.velocity.y * kDt;
        output.position.z += output.velocity.z * kDt;
        return output;
    }
private:
    struct Source { Position position; double scaledMass; };
    std::vector<Source> sources_;
};
}
