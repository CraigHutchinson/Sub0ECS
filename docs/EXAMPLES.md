# Examples

Examples are the user-facing guide to choosing and using the library's features.
Each example is a small, standalone C++ program with a short **Use when /
Demonstrates / Story / Keep in mind / Run** introduction, a deterministic
observable check, and its own CTest entry named `Sub0ECS_Example_<name>`.

## The examples

| Group | Example | Teaches | What it checks |
|---|---|---|---|
| Core usage | `minimal_world` | Declare a query, create an entity, run a system | Build and run independently; resulting component values are checked |
| Core usage | `rocket` | The first game loop: a movement system and a render system over different components | Rockets reach their expected cells; an entity without `Velocity` is drawn but never moved |
| Core usage | `hinted_partitions` | Choose `Volatile<T>` for frequently changing, unqueried components | Compare carried and side-stored values and verify both query results |
| Core usage | `structural_changes` | Add, overwrite, remove, destroy, stale handles, and commit points | Check membership, preserved values, and stale-handle behavior |
| Core usage | `random_access` | Use `find<T>(entity)` and understand column/side-pool lookup | Check present and absent components, including after structural changes |
| Scheduling | `fusion_planners` | Compare `NeverFuse`, `AlwaysFuse`, `ShareColumns`, and `DeviceAware` | Show the resulting groups and compare final state to sequential execution |
| Scheduling | `auto_tuner` | Let measured candidates select a legal plan | Verify the selected plan and final state against the sequential reference |
| Scheduling | `executors` | Choose `Inline`, `Tiled`, `Parallel`, or emulated `Offload` | Check row coverage and written-column results for every executor |
| Scheduling | `parallel_systems` | Use `eachParallel`, `runFusedParallel`, and per-worker command buffers | Verify exact-once row processing and deterministic committed changes |
| Scheduling | `determinism` | Understand the `kBitExact` contract across plans and executors | Compare all supported paths against one sequential result |
| Advanced | `dynamic_systems` | Add a runtime query, process the degraded path, and bound migration work | Check exact-once results before, during, and after migration |
| Advanced | `skirmish` | See storage, scheduling, and structural changes together | Link to the maintained RTS testbed and its lockstep check |

## Rules for each example

- Keep the program focused on one choice; use local, small component types unless
  the shared Skirmish testbed is the feature being demonstrated.
- Make correctness visible in the program's exit status. Do not use benchmark
  timings or external data as correctness gates.
- Build each source as a separate executable and register it with CTest. The
  test suite must remain self-contained and work with the project's supported
  compilers.
- Keep the introductory five-part comment in the source, and link to the
  example from the matching library documentation.

## Running them

The sources are in [`examples/`](../examples/). Each is a CTest test labelled `example`.

Build and run them with:

```sh
cmake --build --preset default
ctest --preset default -L example
```

For an interactive showcase see the Skirmish testbed (`sub0ecs_skirmish_demo`). The
comparison with hand-written, class-hierarchy and naive component designs is in the
benchmarks ([bench/](../bench/README.md)).
