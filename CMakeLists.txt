cmake_minimum_required(VERSION 3.21)

# Detect if Sub0ECS is being included inside another project
if(DEFINED PROJECT_NAME)
    set(IS_SUBPROJECT ON)
endif()

project(Sub0ECS VERSION 2.0.0 LANGUAGES CXX)

option(SUB0ECS_BUILD_TESTING    "Build unit tests"                                              ON)
option(SUB0ECS_BUILD_BENCHMARKS "Build the comparison benchmarks (fetches Google Benchmark)"   OFF)
option(SUB0ECS_NATIVE           "Tune tests/benchmarks for the build host (-march=native; /arch:AVX2 on MSVC)" OFF)
option(SUB0ECS_SANITIZE         "Build tests with sanitizers (ASan+UBSan; ASan on MSVC)"        OFF)

option(SUB0ECS_WITH_PIPELINE "Enable the optional Sub0Pipeline pool adapter and its receiving" OFF)

# ── Core library (header-only) ───────────────────────────────────────────────

add_library(Sub0ECS INTERFACE)
add_library(Sub0ECS::Sub0ECS ALIAS Sub0ECS)
target_compile_features(Sub0ECS INTERFACE cxx_std_20)
target_include_directories(Sub0ECS INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(SUB0ECS_WITH_PIPELINE)
    include(cmake/PipelineAdapter.cmake)
endif()

if(IS_SUBPROJECT OR NOT (SUB0ECS_BUILD_TESTING OR SUB0ECS_BUILD_BENCHMARKS))
    return()
endif()

# ── Flags for this repository's own tests and benchmarks (never consumers) ───

add_library(sub0ecs_dev_flags INTERFACE)
if(MSVC)
    # /bigobj: the benchmark TUs instantiate every design x scenario and exceed COFF's section limit.
    # _CRT_SECURE_NO_WARNINGS: std::getenv/std::fopen are the portable spelling; the _s variants are not.
    target_compile_options(sub0ecs_dev_flags INTERFACE /W4 /permissive- /Zc:__cplusplus /bigobj)
    target_compile_definitions(sub0ecs_dev_flags INTERFACE _CRT_SECURE_NO_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(sub0ecs_dev_flags INTERFACE /clang:-ffp-contract=off)   # see the GCC/Clang branch
    endif()
    if(SUB0ECS_NATIVE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            # clang-cl: the MSVC command line and standard library, with LLVM's optimiser,
            # which can target the build host exactly.
            target_compile_options(sub0ecs_dev_flags INTERFACE /clang:-march=native)
        else()
            # MSVC has no -march=native; AVX2 is the widest ISA every x86-64 bench host here supports.
            target_compile_options(sub0ecs_dev_flags INTERFACE /arch:AVX2)
        endif()
    endif()
else()
    # -ffp-contract=off: the designs must agree bit for bit, and every compiler should be
    # measured on the same arithmetic. GCC's default (fast) fuses multiply-adds across
    # statements, differently in differently shaped loops, so on an FMA target (-march=native,
    # ARM64) the conformance tests fail (seen with GCC 15). MSVC's /fp:precise does not contract.
    target_compile_options(sub0ecs_dev_flags INTERFACE -Wall -Wextra -ffp-contract=off $<$<CONFIG:Release>:-O3>)
    if(SUB0ECS_NATIVE)
        target_compile_options(sub0ecs_dev_flags INTERFACE -march=native)
    endif()
endif()

if(SUB0ECS_SANITIZE)
    # On the shared flags, not just the test binary: MSVC's ASan annotates STL
    # containers and refuses to link objects built without it.
    if(MSVC)
        target_compile_options(sub0ecs_dev_flags INTERFACE /fsanitize=address)
    else()
        target_compile_options(sub0ecs_dev_flags INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(sub0ecs_dev_flags INTERFACE -fsanitize=address,undefined)
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
find_package(Threads REQUIRED)

# bench/ also provides the reference designs the conformance tests check
# against, so it is added whenever tests are built.
add_subdirectory(bench)

if(SUB0ECS_BUILD_TESTING)
    enable_testing()
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()
