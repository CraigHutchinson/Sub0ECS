cmake_minimum_required(VERSION 3.20)

# SubzeroECS v2 design spike — standalone so it can be built independently of
# the v1 package (and on hosts where the v1 top-level configure does not work).
#
#   cmake -S spike/v2 -B build/spike -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build build/spike
#   ctest --test-dir build/spike --output-on-failure
#   ./build/spike/spike_bench --benchmark_repetitions=5 --benchmark_report_aggregates_only=true

project(SubzeroECS_V2Spike LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(SPIKE_NATIVE "Tune for the build host (-march=native; /arch:AVX2 on MSVC)" ON)
option(SPIKE_SANITIZE "Build conformance test with ASan/UBSan" OFF)

set(V1_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
list(APPEND CMAKE_MODULE_PATH ${V1_ROOT}/cmake)
include(CPM)

CPMAddPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    VERSION 1.9.4
    OPTIONS "BENCHMARK_ENABLE_TESTING OFF" "BENCHMARK_ENABLE_GTEST_TESTS OFF" "BENCHMARK_ENABLE_WERROR OFF"
)

# v1 compiled from source (unmodified) for the baseline adapter
add_library(spike_v1 STATIC
    ${V1_ROOT}/source/SubzeroECS/CollectionRegistry.cpp
    ${V1_ROOT}/source/SubzeroECS/UniqueIndex32.cpp
)
# SYSTEM: v1 headers are the baseline under test, not spike code — keep spike warnings visible
target_include_directories(spike_v1 SYSTEM PUBLIC ${V1_ROOT}/source)

add_library(spike_flags INTERFACE)
if(MSVC)
    # /bigobj: the benchmark TUs instantiate every design x scenario and exceed COFF's section limit.
    # _CRT_SECURE_NO_WARNINGS: std::getenv/std::fopen are the portable spelling; the _s variants are not.
    target_compile_options(spike_flags INTERFACE /W4 /permissive- /Zc:__cplusplus /bigobj $<$<CONFIG:Release>:/O2 /Oi /Ot>)
    target_compile_definitions(spike_flags INTERFACE _CRT_SECURE_NO_WARNINGS)
    if(SPIKE_NATIVE)
        # MSVC has no -march=native; AVX2 is the widest ISA every x86-64 bench host here supports.
        target_compile_options(spike_flags INTERFACE /arch:AVX2)
    endif()
else()
    target_compile_options(spike_flags INTERFACE -Wall -Wextra $<$<CONFIG:Release>:-O3>)
    if(SPIKE_NATIVE)
        target_compile_options(spike_flags INTERFACE -march=native)
    endif()
endif()
target_link_libraries(spike_v1 PRIVATE spike_flags)

add_executable(spike_bench bench/main.cpp)
target_link_libraries(spike_bench PRIVATE spike_v1 spike_flags benchmark::benchmark)

add_executable(spike_conformance test/conformance.cpp)
target_link_libraries(spike_conformance PRIVATE spike_v1 spike_flags)
if(SPIKE_SANITIZE AND NOT MSVC)
    target_compile_options(spike_conformance PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(spike_conformance PRIVATE -fsanitize=address,undefined)
endif()

enable_testing()
add_test(NAME spike_conformance COMMAND spike_conformance)

# ---- Fusion extension points (planners / executors) ----
find_package(Threads REQUIRED)
add_executable(spike_fusion_test test/fusion_test.cpp)
target_link_libraries(spike_fusion_test PRIVATE spike_flags Threads::Threads)
if(SPIKE_SANITIZE AND NOT MSVC)
    target_compile_options(spike_fusion_test PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(spike_fusion_test PRIVATE -fsanitize=address,undefined)
endif()
add_test(NAME spike_fusion_test COMMAND spike_fusion_test)

# ---- H9 dynamic system lifetimes ----
add_executable(spike_dynamic_test test/dynamic_test.cpp)
target_link_libraries(spike_dynamic_test PRIVATE spike_flags Threads::Threads)
if(SPIKE_SANITIZE AND NOT MSVC)
    target_compile_options(spike_dynamic_test PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(spike_dynamic_test PRIVATE -fsanitize=address,undefined)
endif()
add_test(NAME spike_dynamic_test COMMAND spike_dynamic_test)

# ---- Skirmish testbed: an RTS simulation driving every storage design ----
add_executable(skirmish_demo testbed/skirmish/demo.cpp)
target_link_libraries(skirmish_demo PRIVATE spike_flags Threads::Threads)
add_executable(skirmish_conformance testbed/skirmish/conformance.cpp)
target_link_libraries(skirmish_conformance PRIVATE spike_flags Threads::Threads)
if(SPIKE_SANITIZE AND NOT MSVC)
    target_compile_options(skirmish_conformance PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(skirmish_conformance PRIVATE -fsanitize=address,undefined)
endif()
add_test(NAME skirmish_conformance COMMAND skirmish_conformance)
add_executable(skirmish_bench testbed/skirmish/bench.cpp)
target_link_libraries(skirmish_bench PRIVATE spike_flags benchmark::benchmark Threads::Threads)

# Micro-benchmark: span-count cost (design review evidence)
add_executable(spike_spans_micro bench/spans_micro.cpp)
target_link_libraries(spike_spans_micro PRIVATE spike_flags benchmark::benchmark)

# H9 timeline: cost of paging a system in (degraded path, bounded migration, stall)
add_executable(spike_dynamic_timeline bench/dynamic_timeline.cpp)
target_link_libraries(spike_dynamic_timeline PRIVATE spike_flags Threads::Threads)
