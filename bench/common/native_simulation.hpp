#pragma once
#include <cstddef>
#include <utility>
#include "sub0ecs/fusion/executors/parallel.hpp"
namespace bench
{
/** Own a simulation and native pool for the complete prepared benchmark lifetime. */
template <typename Simulation, std::size_t Rows>
struct NativeSimulation
{
    Simulation simulation;
    sub0ecs::fusion::Parallel pool;
    template <typename... Args>
    NativeSimulation(std::size_t size, unsigned workers, Args&&... args)
        : simulation(size, std::forward<Args>(args)...), pool(workers) {}
    void tick() { simulation.template tick<Rows>(pool); }
    const auto& state() { return simulation.state(); }
    auto ticks() const { return simulation.ticks(); }
};
}
