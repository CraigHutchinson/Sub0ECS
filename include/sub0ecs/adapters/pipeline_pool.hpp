#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <vector>

#include "sub0pipeline/executor/executor.hpp"

namespace sub0ecs::adapters
{
/** Synchronous ECS pool facade over a borrowed Pipeline executor.
 *  @note The coordinator exclusively owns the executor's dispatch/wait interval.
 *  Never invoke from one of that executor's jobs: a bounded pool can starve while
 *  its workers wait for child jobs. The executor must outlive this object.
 *  Uses at most one submitted task per lane, never one task per entity. Lane IDs
 *  identify exclusive scratch slots, not OS threads. Callbacks may read immutable
 *  shared inputs and write only their disjoint rows/scratch. No structural changes
 *  or borrowed-state destruction are permitted until joined return.
 *  Worker floating-point state is the executor owner's responsibility. This bridge
 *  does not inherit/restore the coordinator's floating-point environment.
 */
class PipelinePool
{
public:
    /** Fixes the lane/scratch bound at construction.
     *  @param executor Borrowed execution resource satisfying IExecutor's contract.
     *  @param lanes Maximum simultaneous lanes; zero uses executor.concurrency().
     *  @throws std::invalid_argument Invalid executor width or excessive lane count.
     */
    explicit PipelinePool(sub0pipeline::IExecutor& executor, unsigned lanes = 0)
        : executor_(executor), lanes_(resolveLanes(executor, lanes)), errors_(lanes_)
    {
    }

    PipelinePool(const PipelinePool&) = delete;
    PipelinePool& operator=(const PipelinePool&) = delete;

    /** @return Exclusive scratch slots needed by callbacks. */
    [[nodiscard]] unsigned concurrency() const noexcept { return lanes_; }

    /** Calls fn(item, lane) once for every item on success, then joins all work.
     *  @tparam F Concurrent callable type.
     *  @param items Number of independent items, including zero.
     *  @param fn Borrowed callable; must support simultaneous disjoint invocations.
     *  @throws Submission or callback exceptions, only after accepted work joins.
     *  Callback failure may leave partially modified output; no rollback is promised.
     *  Re-entry is rejected. Normal failed calls are reusable after joined return.
     *  Callable/executor storage may allocate; only this adapter's lane storage is
     *  preallocated. Inline executors and single-lane calls run on the coordinator.
     */
    template <typename F>
    void parallelFor(std::size_t items, F& fn)
    {
        if (active_.test_and_set(std::memory_order_acquire))
        {
            throw std::logic_error("PipelinePool requires non-reentrant coordinator access");
        }
        struct Reset
        {
            std::atomic_flag& active;
            ~Reset() { active.clear(std::memory_order_release); }
        } reset{active_};
        if (items == 0) return;
        const auto count = static_cast<unsigned>(std::min<std::size_t>(items, lanes_));
        if (count == 1 || executor_.runsInline())
        {
            for (std::size_t item = 0; item < items; ++item) fn(item, 0u);
            return;
        }
        std::fill(errors_.begin(), errors_.end(), std::exception_ptr{});
        struct Context
        {
            F& function;
            std::exception_ptr* errors;
            std::size_t quotient;
            std::size_t remainder;
        } context{fn, errors_.data(), items / count, items % count};
        std::exception_ptr rejected;
        try
        {
            for (unsigned lane = 0; lane < count; ++lane)
            {
                executor_.dispatch("ecs.rows", [pointer = &context, lane]() noexcept {
                    try
                    {
                        const auto first = lane * pointer->quotient + std::min<std::size_t>(lane, pointer->remainder);
                        const auto length = pointer->quotient + (lane < pointer->remainder ? 1u : 0u);
                        for (std::size_t offset = 0; offset < length; ++offset)
                            pointer->function(first + offset, lane);
                    }
                    catch (...)
                    {
                        pointer->errors[lane] = std::current_exception();
                    }
                }, []() noexcept {}, -1, 5, 0);
            }
        }
        catch (...)
        {
            rejected = std::current_exception();
        }
        // IExecutor cannot throw while accepted callbacks or callable targets live.
        executor_.waitAll();
        if (rejected) std::rethrow_exception(rejected);
        for (const auto& error : errors_)
            if (error) std::rethrow_exception(error);
    }

private:
    static unsigned resolveLanes(sub0pipeline::IExecutor& executor, unsigned lanes)
    {
        const int available = executor.concurrency();
        if (available <= 0 || lanes > static_cast<unsigned>(available))
            throw std::invalid_argument("PipelinePool lane count exceeds executor width");
        return lanes == 0 ? static_cast<unsigned>(available) : lanes;
    }

    sub0pipeline::IExecutor& executor_;
    unsigned lanes_;
    std::vector<std::exception_ptr> errors_;
    std::atomic_flag active_ = ATOMIC_FLAG_INIT;
};
}
