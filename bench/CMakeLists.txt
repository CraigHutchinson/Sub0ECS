# Comparison benchmarks: the library store against reference implementations.
# See README.md (references, workloads, fairness guards) and BENCHMARKING.md (harness).

# ── Support used by the benchmarks AND the conformance tests ──────────────────

# Workload components, scenarios, systems, reference designs and the Skirmish testbed.
add_library(sub0ecs_bench_support INTERFACE)
target_include_directories(sub0ecs_bench_support INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sub0ecs_bench_support INTERFACE Sub0ECS::Sub0ECS sub0ecs_dev_flags Threads::Threads)

if(NOT SUB0ECS_BUILD_BENCHMARKS)
    return()
endif()

# ── Benchmark executables (run through tools/run.py, not ctest) ───────────────

include(CPM)
CPMAddPackage(
    NAME nanobench
    GITHUB_REPOSITORY martinus/nanobench
    GIT_TAG v4.6.0
)

# The harness: nanobench plus names, --filter, paired group comparisons and the
# results JSON that tools/*.py read (see harness/harness.hpp).
add_library(sub0ecs_bench_harness STATIC
    harness/entry.cpp
    harness/options.cpp
    harness/report.cpp
    harness/runner.cpp
)
target_link_libraries(sub0ecs_bench_harness PUBLIC sub0ecs_bench_support nanobench::nanobench)

# Symbols in the benchmark binaries, so profilers can name what they sample
# (tools/profile.py). Debug information does not change the generated code.
if(MSVC)
    target_compile_options(sub0ecs_bench_harness PUBLIC /Z7)
    target_link_options(sub0ecs_bench_harness PUBLIC /DEBUG /OPT:REF /OPT:ICF)
else()
    target_compile_options(sub0ecs_bench_harness PUBLIC -g)
endif()

# Storage designs on micro scenarios, fusion, planners x executors
add_executable(sub0ecs_bench scenarios_bench.cpp)
target_link_libraries(sub0ecs_bench PRIVATE sub0ecs_bench_harness)

# Partition-count (span) overhead
add_executable(sub0ecs_spans_bench spans_bench.cpp)
target_link_libraries(sub0ecs_spans_bench PRIVATE sub0ecs_bench_harness)

# Runtime-query timeline: cost of paging a system in (degraded path, bounded migration, stall)
add_executable(sub0ecs_dynamic_timeline dynamic_timeline.cpp)
target_link_libraries(sub0ecs_dynamic_timeline PRIVATE sub0ecs_bench_support)

# Skirmish RTS testbed: time per tick per design, thread scaling, and an interactive demo
add_executable(sub0ecs_skirmish_bench skirmish/bench.cpp)
target_link_libraries(sub0ecs_skirmish_bench PRIVATE sub0ecs_bench_harness)
add_executable(sub0ecs_skirmish_demo skirmish/demo.cpp)
target_link_libraries(sub0ecs_skirmish_demo PRIVATE sub0ecs_bench_support)

# Representative evolving application; shares the existing paired JSON harness.
add_executable(sub0ecs_nbody_bench nbody/bench.cpp)
target_link_libraries(sub0ecs_nbody_bench PRIVATE sub0ecs_bench_harness)
if(SUB0ECS_WITH_PIPELINE)
    target_compile_definitions(sub0ecs_nbody_bench PRIVATE SUB0ECS_TEST_PIPELINE=1)
    target_link_libraries(sub0ecs_nbody_bench PRIVATE Sub0ECS::Pipeline Sub0Pipeline::Priority)
endif()

# Separate chunk bookkeeping from kernel and scheduler cost.
add_executable(sub0ecs_rows_bench rows/bench.cpp)
target_link_libraries(sub0ecs_rows_bench PRIVATE sub0ecs_bench_harness)

add_executable(sub0ecs_row_workloads_bench row_workloads/bench.cpp)
target_link_libraries(sub0ecs_row_workloads_bench PRIVATE sub0ecs_bench_harness)
if(SUB0ECS_WITH_PIPELINE)
    target_compile_definitions(sub0ecs_row_workloads_bench PRIVATE SUB0ECS_TEST_PIPELINE=1)
    target_link_libraries(sub0ecs_row_workloads_bench PRIVATE Sub0ECS::Pipeline Sub0Pipeline::Priority)
endif()
