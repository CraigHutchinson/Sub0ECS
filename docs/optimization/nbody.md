# Ordered n-body: an optional integration workload

This is a library benchmark application, not a received Crucible production
optimization. See [the consumer campaign](crucible.md) for the separate production
gates and the architecture/DRY decisions across ECS, Pipeline and Pub.

## Composition and contracts

`Sub0ECS::Sub0ECS` remains standalone C++20. `SUB0ECS_WITH_PIPELINE=ON`
adds the C++23 `Sub0ECS::Pipeline` interface target, pinned to Pipeline
`f730c4ec2973a449c45fbf9a74595414b9bf30e1` unless the parent already supplies
`Sub0Pipeline::Sub0Pipeline`. The adapter lives in ECS because it translates the
ECS pool contract. Include `sub0ecs/adapters/pipeline_pool.hpp` explicitly;
the core umbrella does not import optional dependencies. The benchmark uses
Pipeline's Priority target; a parent enabling receiving tests must supply it too.

```cpp
sub0pipeline::PriorityExecutor executor({.threadCount = 4, .queueCapacity = 4});
sub0ecs::adapters::PipelinePool pool(executor);
world.eachParallel<Position, Velocity>(pool, update,
    sub0ecs::store::RowGrain<64>{});
```

One coordinator exclusively owns the executor's dispatch/wait interval and waits
outside its worker pool. The executor outlives the adapter. The adapter submits
at most one task per lane, distributes chunk ordinals evenly and joins all
accepted work before propagating submission/body exceptions. Lane indices own
scratch slots; they are not physical worker IDs. Single-lane and inline executors
run on the coordinator. No row borrows escape joined return. Structural mutation,
re-entry and overlapping world access are forbidden. Failures can leave partial
row writes; production callers needing atomic commit must retain staging.

This does not establish FP environment inheritance, safe nested pool waits,
cancellation, or hazard-derived DAG scheduling. Those remain explicit caller
contracts. It also does not claim dispatch is allocation-free: Pipeline stores
`std::function` jobs. Sharing its executor avoids creating a second pool only in
callers that would otherwise create one; Crucible currently already has one pool.

`RowGrain<Rows>` is an explicit positive compile-time third argument to
`eachParallel`; existing calls retain 1024 rows/chunk and the four-chunk inline
threshold. Expensive kernels can expose enough chunks without changing defaults
for cheap streaming work. This does not change `runFusedParallel` or pool width.

## Workload and metric interpretation

The scalar oracle uses deterministic initial positions/masses and softened,
ordered all-pairs gravity. Each tick snapshots the previous state, evaluates
sources in ascending stable-ID order, then updates velocity and position.
The ECS variants gather by ID and write disjoint target rows after synchronous
join. Snapshot/gather and the entire update are timed; pool construction is not.
The plain reference shares the exact arithmetic kernel, isolating storage and
scheduling costs. It is not a claim of the fastest possible n-body algorithm.

The existing paired harness compares plain, sequential ECS, native pool and
Pipeline pool, with default and explicit 64-row grains at 1/2/4 lanes. Two warmup
ticks and one tick per epoch preserve equal evolving work within each comparison.
The registry's `nbody` suite uses the existing JSON result schema, counters,
filters and profiler machinery. `completed_ticks`, `state_checksum` and
`directed_interactions_per_tick` identify useful work. Conformance checks all
position/velocity/mass bits after each tick, including a chunk tail, zero/single
worlds, and an analytic two-body force. Large all-pairs tests are exhaustive.

```sh
cmake --preset default -DSUB0ECS_WITH_PIPELINE=ON
cmake --build --preset default
ctest --preset default
ctest --preset exhaustive
python3 bench/tools/run.py --profile nbody --build-dir build/default
NBODY_SIZES=1024 NBODY_THREADS=2 build/default/bench/sub0ecs_nbody_bench \
  --filter='(HandWritten|Pipeline2|PipelineG64x2)/1024$' --out=run.json
```

Use the profiler wrapper for supervised VTune collection. Where hardware events
are unavailable, Callgrind provides dynamic guest instruction references (`Ir`),
not retired hardware instructions or a reliable elapsed time. Collect one arm in
one process with fixed epochs; include startup and two warmups explicitly in the
receipt. Keep native execution-time runs separate from instrumented runs.
Never normalize total-process Ir as kernel instructions without subtracting or
attributing setup. Keep raw captures, binary/source hashes, logs, compile flags,
load and failures. The shared-cloud measurements here are diagnostic; whole-frame
Crucible p95/p99 and replay/FP/lifecycle receiving remain outstanding.

## Next representative scenarios

Keep optional dependencies in the owning library's test composition. A future
Pub arm should compare direct completion, fixed `Wiring`, and scoped `Domain`
using Pub's own architectural types and a meaningful delivery workload. Avoid
publishing borrowed asynchronous spans or adding a router inside ECS. No Pub API
change or fabricated shared type layer is required by this n-body experiment.
The [representative suite](representative.md) now supplies streaming,
fragmented/churning worlds and staged fixed-neighbor work with bitwise oracles.
Bounded command delivery and a real spatial grid remain future scenarios.

## Same-resource handwritten controls

The `MatchedNativeN` and `MatchedPipelineN` benchmark groups compare ECS with
handwritten contiguous-body ranges using the same pool implementation, N lanes,
64-row grain, four-chunk inline threshold and immutable tick snapshot. Both
include snapshot/gather in timing and join before returning. Their oracle and
reduction order are unchanged. These ratios isolate storage/dispatch overhead
from thread scaling; the original `Ordered` group remains for continuity. The
shared arithmetic kernel is still not a claim of the fastest n-body algorithm.

## Compact ordered kernel

`CompactEcs` and `CompactPlain` stage only positions and pre-scaled masses, then
split each ascending source reduction around self. `CompactMatchedNativeN` and
`CompactMatchedPipelineN` use the same resource controls as above. The original
scalar `advanceBody` oracle is unchanged. These are benchmark application kernels,
not a new storage API or a Crucible implementation. See the
[representative receipt](../../bench/results/reference/work-mode-representative/README.md)
for current measurements, instruction attribution and memory tradeoffs.
