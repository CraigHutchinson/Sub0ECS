#pragma once
#include <cstddef>
#include <utility>
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0pipeline/executor/priority_executor.hpp"
namespace bench
{
/** Borrow through the ECS adapter; destroy the pool facade before its executor. */
template <typename Simulation, std::size_t Rows>
struct PipelineSimulation
{
    Simulation simulation;
    sub0pipeline::PriorityExecutor executor;
    sub0ecs::adapters::PipelinePool pool;
    template <typename... Args>
    PipelineSimulation(std::size_t size, unsigned workers, Args&&... args)
        : simulation(size, std::forward<Args>(args)...),
          executor({.threadCount = workers, .queueCapacity = workers}), pool(executor) {}
    void tick() { simulation.template tick<Rows>(pool); }
    const auto& state() { return simulation.state(); }
    auto ticks() const { return simulation.ticks(); }
};
}
