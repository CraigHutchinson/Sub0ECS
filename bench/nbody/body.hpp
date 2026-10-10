#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>


namespace bench::nbody
{
struct Position { double x{}, y{}, z{}; };
struct Velocity { double x{}, y{}, z{}; };
struct Mass { double value{1.0}; };
struct Id { std::size_t value{}; };
struct Body { Position position; Velocity velocity; Mass mass; };

/** Fixed seed without library RNG differences; all bodies have positive mass. */
inline Body initial(std::size_t index)
{
    const auto i = static_cast<std::uint64_t>(index);
    return {{double((i * 17 + 3) % 251) / 16.0, double((i * 31 + 7) % 257) / 16.0,
             double((i * 43 + 11) % 263) / 16.0}, {}, {1.0 + double(i % 7) / 8.0}};
}

/** Softened all-pairs gravity; ascending source order is part of the oracle.
 * Independent target rows can run concurrently, but their reductions cannot be
 * reassociated. Every row reads the same immutable tick-start snapshot.
 */
inline Body advanceBody(const std::vector<Body>& input, std::size_t row)
{
    constexpr double kDt = 1.0 / 1024.0;
    constexpr double kSofteningSquared = 1.0 / 16.0;
    Body output = input[row];
    double ax = 0.0, ay = 0.0, az = 0.0;
    for (std::size_t other = 0; other < input.size(); ++other)
    {
        if (other == row) continue;
        const double dx = input[other].position.x - input[row].position.x;
        const double dy = input[other].position.y - input[row].position.y;
        const double dz = input[other].position.z - input[row].position.z;
        const double squared = dx * dx + dy * dy + dz * dz + kSofteningSquared;
        const double scale = (0.01 * input[other].mass.value) / (squared * std::sqrt(squared));
        ax += dx * scale;
        ay += dy * scale;
        az += dz * scale;
    }
    output.velocity.x += ax * kDt;
    output.velocity.y += ay * kDt;
    output.velocity.z += az * kDt;
    output.position.x += output.velocity.x * kDt;
    output.position.y += output.velocity.y * kDt;
    output.position.z += output.velocity.z * kDt;
    return output;
}

}
