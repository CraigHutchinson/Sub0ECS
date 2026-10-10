#pragma once
#include <cstddef>
#include <vector>
#include "row_workloads/point.hpp"
#include "row_workloads/scenario.hpp"
namespace bench::rows
{
/** Evolving row state and stable identity, shared by the independent storage arms. */
struct State
{
    double x{}, y{}, vx{}, vy{};
    std::size_t id{};
};
inline State initial(std::size_t id)
{
    return {double(id % 251) / 16.0, double(id % 257) / 16.0,
            double(id % 7) / 1024.0, -double(id % 11) / 1024.0, id};
}
/** Fixed neighbor topology is synthetic; it models gather/read/update, not a spatial grid. */
inline void update(State& row, Scenario scenario, const std::vector<Point>& snapshot)
{
    if (scenario == Scenario::StagedNeighbors)
    {
        double ax = 0.0, ay = 0.0;
        for (std::size_t offset = 1; offset <= 8; ++offset)
        {
            const auto& neighbor = snapshot[(row.id + offset) % snapshot.size()];
            ax += (neighbor.x - snapshot[row.id].x) * (1.0 / 65536.0);
            ay += (neighbor.y - snapshot[row.id].y) * (1.0 / 65536.0);
        }
        row.vx += ax;
        row.vy += ay;
    }
    row.x += row.vx * (1.0 / 64.0);
    row.y += row.vy * (1.0 / 64.0);
}
}
