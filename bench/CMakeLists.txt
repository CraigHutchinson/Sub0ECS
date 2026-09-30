# Comparison benchmarks: the v2 store against the alternative designs and v1.
# See README.md (candidates, workloads, fairness guards) and BENCHMARKING.md (harness).

# ── Support used by the benchmarks AND the conformance tests ──────────────────

# v1 compiled from its frozen, unmodified source as the baseline comparator.
# v1 calls members of an incomplete type (Entity.hpp -> World), which GCC and MSVC
# accept and Clang rightly rejects. v1 stays byte-identical, so Clang builds run
# every comparison except against v1.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(SUB0ECS_HAS_V1_BASELINE OFF)
else()
    set(SUB0ECS_HAS_V1_BASELINE ON)
    add_library(sub0ecs_v1_baseline STATIC
        baselines/v1/SubzeroECS/CollectionRegistry.cpp
        baselines/v1/SubzeroECS/UniqueIndex32.cpp
    )
    # SYSTEM: v1 is the baseline under test, not code we maintain; keep our own warnings visible.
    target_include_directories(sub0ecs_v1_baseline SYSTEM PUBLIC baselines/v1)
    target_compile_features(sub0ecs_v1_baseline PUBLIC cxx_std_20)
    target_link_libraries(sub0ecs_v1_baseline PRIVATE sub0ecs_dev_flags)
endif()

# Workload components, scenarios, systems, comparator designs and the Skirmish testbed.
add_library(sub0ecs_bench_support INTERFACE)
target_include_directories(sub0ecs_bench_support INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(sub0ecs_bench_support INTERFACE Sub0ECS::Sub0ECS sub0ecs_dev_flags Threads::Threads)
target_compile_definitions(sub0ecs_bench_support INTERFACE SUB0ECS_HAS_V1_BASELINE=$<BOOL:${SUB0ECS_HAS_V1_BASELINE}>)
if(SUB0ECS_HAS_V1_BASELINE)
    target_link_libraries(sub0ecs_bench_support INTERFACE sub0ecs_v1_baseline)
endif()

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

# Storage designs on micro scenarios, fusion, planners x executors
add_executable(sub0ecs_bench scenarios_bench.cpp)
target_link_libraries(sub0ecs_bench PRIVATE sub0ecs_bench_harness)

# Partition-count (span) overhead
add_executable(sub0ecs_spans_bench spans_bench.cpp)
target_link_libraries(sub0ecs_spans_bench PRIVATE sub0ecs_bench_harness)

# H9 timeline: cost of paging a system in (degraded path, bounded migration, stall)
add_executable(sub0ecs_dynamic_timeline dynamic_timeline.cpp)
target_link_libraries(sub0ecs_dynamic_timeline PRIVATE sub0ecs_bench_support)

# Skirmish RTS testbed: time per tick per design, thread scaling, and an interactive demo
add_executable(sub0ecs_skirmish_bench skirmish/bench.cpp)
target_link_libraries(sub0ecs_skirmish_bench PRIVATE sub0ecs_bench_harness)
add_executable(sub0ecs_skirmish_demo skirmish/demo.cpp)
target_link_libraries(sub0ecs_skirmish_demo PRIVATE sub0ecs_bench_support)
