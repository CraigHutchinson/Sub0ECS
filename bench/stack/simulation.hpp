#pragma once
#include <limits>
#include <tuple>
#include "stack/delivery.hpp"
#include "sub0ecs/store/world.hpp"
#include "sub0ecs/adapters/pipeline_pool.hpp"
#include "sub0pipeline/executor/priority_executor.hpp"
#include "sub0pipeline/sub0pipeline.hpp"
namespace bench::stack
{
/** Complete command -> staged proposal -> commit -> owned publication reproduction.
 * Stable IDs are dense and bounded, but partition traversal is deliberately permuted.
 * Structural changes and published state remain coordinator-owned after synchronous join.
 */
template <typename Route, unsigned Layout>
class Simulation
{
public:
    Simulation(std::size_t n, unsigned workers)
        : ingress_(8), route_(ingress_), input_(n), proposal_(n), published_(n), staging_(n), byId_(n),
          executor_({.threadCount = workers, .queueCapacity = workers}), pool_(executor_)
    {
        world_.reserve(n);
        for (std::size_t i = n; i > 0; --i)
        {
            const auto id = i - 1;
            if (id & 1) world_.create(initial(id), Tag{});
            else world_.create(initial(id));
            published_[id] = initial(id);
        }
        const auto proposeJob = graph_.emplace([this] { prepare(); compute(); }).name("proposal");
        const auto commitJob = graph_.emplace([this] { commit(); }).name("commit").succeed(proposeJob);
        graph_.emplace([this] { capture(); }).name("publish").succeed(commitJob);
    }
    bool admit(std::span<const Command> commands)
    {
        if (request_ == std::numeric_limits<std::uint64_t>::max()) throw std::overflow_error("request exhausted");
        const Batch batch{++request_, commands};
        const auto receipt = route_.send(batch);
        if (receipt.request != batch.request) throw std::logic_error("missing receipt");
        return receipt.accepted;
    }
    void tick()
    {
        if (!graph_.runInline()) throw std::runtime_error("stack boundary failed");
        ++ticks_;
    }
    const auto& state() const { return published_; }
    auto ticks() const { return ticks_; }
private:
    struct Tag {};
    using Queries = std::tuple<sub0ecs::Query<Row>, sub0ecs::Query<Row, Tag>>;
    void prepare()
    {
        impulse_ = ingress_.drain();
        if constexpr (Layout == 2)
        {
            // Fixed population proves a complete permutation of [0,n). No sparse IDs,
            // deletion/reuse or mobile filtering are allowed by this workload contract.
            world_.template each<Row>([&](const Row& row) { input_[row.id] = row; });
        }
        else
        {
            std::size_t i{};
            world_.template each<Row>([&](const Row& row) { input_[i++] = row; });
            std::sort(input_.begin(), input_.end(), [](const Row& a, const Row& b) { return a.id < b.id; });
        }
        // Rebuild each epoch; do not rely on partition order or stale row addresses.
        if constexpr (Layout == 1)
            for (std::size_t row = 0; row < input_.size(); ++row) byId_[input_[row].id] = row;
    }
    void compute()
    {
        constexpr std::size_t grain = 256;
        const auto n = input_.size();
        const auto count = n == 0 ? 0 : 1 + (n - 1) / grain;
        auto body = [&](std::size_t item, unsigned) {
            const auto begin = item * grain;
            const auto end = begin + std::min(grain, n - begin);
            for (auto row = begin; row < end; ++row) proposal_[row] = propose(input_[row], impulse_);
        };
        if (count < 4) for (std::size_t item = 0; item < count; ++item) body(item, 0u);
        else pool_.parallelFor(count, body);
    }
    void commit()
    {
        world_.template each<Row>([&](Row& row) {
            if constexpr (Layout == 2) row = proposal_[row.id];
            else if constexpr (Layout == 1) row = proposal_[byId_[row.id]];
            else row = *std::lower_bound(proposal_.begin(), proposal_.end(), row.id,
                [](const Row& candidate, std::uint64_t id) { return candidate.id < id; });
        });
    }
    void capture()
    {
        world_.template each<Row>([&](const Row& row) { staging_[row.id] = row; });
        published_.swap(staging_);
    }
    Ingress ingress_;
    Route route_;
    sub0ecs::store::World<Queries> world_;
    std::vector<Row> input_, proposal_, published_, staging_;
    std::vector<std::size_t> byId_;
    std::uint64_t impulse_{}, request_{};
    std::size_t ticks_{};
    sub0pipeline::PriorityExecutor executor_;
    sub0ecs::adapters::PipelinePool pool_;
    sub0pipeline::Pipeline graph_;
};
}
