# Example plan

Examples are the user-facing guide to choosing and using the library's features.
Each example will be a small, standalone C++ program with a short **Use when /
Demonstrates / Story / Keep in mind / Run** introduction, a deterministic
observable check, and its own CTest entry named `Sub0ECS_Example_<name>`.

## Delivery order

| Wave | Example | Teaches | Acceptance check |
|---|---|---|---|
| 1: core usage | `minimal_world` | Declare a query, create an entity, run a system | Build and run independently; resulting component values are checked |
| 1: core usage | `hinted_partitions` | Choose `Volatile<T>` for frequently changing, unqueried components | Compare carried and side-stored values and verify both query results |
| 1: core usage | `structural_changes` | Add, overwrite, remove, destroy, stale handles, and commit points | Check membership, preserved values, and stale-handle behavior |
| 1: core usage | `random_access` | Use `find<T>(entity)` and understand column/side-pool lookup | Check present and absent components, including after structural changes |
| 2: scheduling | `fusion_planners` | Compare `NeverFuse`, `AlwaysFuse`, `ShareColumns`, and `DeviceAware` | Show the resulting groups and compare final state to sequential execution |
| 2: scheduling | `auto_tuner` | Let measured candidates select a legal plan | Verify the selected plan and final state against the sequential reference |
| 2: scheduling | `executors` | Choose `Inline`, `Tiled`, `Parallel`, or emulated `Offload` | Check row coverage and written-column results for every executor |
| 2: scheduling | `parallel_systems` | Use `eachParallel`, `runFusedParallel`, and per-worker command buffers | Verify exact-once row processing and deterministic committed changes |
| 2: scheduling | `determinism` | Understand the `kBitExact` contract across plans and executors | Compare all supported paths against one sequential result |
| 3: advanced usage | `dynamic_systems` | Add a runtime query, process the degraded path, and bound migration work | Check exact-once results before, during, and after migration |
| 3: advanced usage | `skirmish` | See storage, scheduling, and structural changes together | Link to the maintained RTS testbed and its lockstep check |

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

## Current groundwork

`minimal_world` and the CMake/CTest pattern are in place. The other ten examples
remain planned; they are intentionally not represented as complete. This branch
is a draft landing PR until the required example set is implemented and checked.

Build and run the current example with:

```sh
cmake --build --preset default --target sub0ecs_example_minimal_world
ctest --preset default -R '^Sub0ECS_Example_'
```
