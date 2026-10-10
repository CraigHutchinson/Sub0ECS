#pragma once
#include "row_workloads/state.hpp"
namespace bench::rows
{
/** Contiguous oracle; includes the same snapshot and logical tag updates as ECS. */
class Plain
{
public:
    Plain(std::size_t size, Scenario scenario) : rows_(size), tags_(size), snapshot_(size), scenario_(scenario)
    {
        for (std::size_t i = 0; i < size; ++i) { rows_[i] = initial(i); tags_[i] = scenario == Scenario::FragmentedChurn ? unsigned(i & 7) : 0; }
    }
    void tick()
    {
        if (scenario_ == Scenario::StagedNeighbors)
            for (const auto& row : rows_) snapshot_[row.id] = {row.x, row.y};
        for (auto& row : rows_) update(row, scenario_, snapshot_);
        if (scenario_ == Scenario::FragmentedChurn)
        {
            for (std::size_t i = ticks_ % 16; i < rows_.size(); i += 16) tags_[i] ^= 1;
            // Tags drive three real selection-dependent behaviors, not unused partitions.
            for (auto& row : rows_)
            {
                if (tags_[row.id] & 1) row.vx += 1.0 / 1048576.0;
                if (tags_[row.id] & 2) row.vy -= 1.0 / 1048576.0;
                if (tags_[row.id] & 4) row.x += 1.0 / 1048576.0;
            }
        }
        ++ticks_;
    }
    const auto& state() const { return rows_; }
    const auto& tags() const { return tags_; }
    auto ticks() const { return ticks_; }
private:
    std::vector<State> rows_;
    std::vector<unsigned> tags_;
    std::vector<Point> snapshot_;
    Scenario scenario_;
    std::size_t ticks_{};
};
}
