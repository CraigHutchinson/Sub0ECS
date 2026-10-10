# Unit tests (doctest). One executable, registered with ctest.

include(CPM)
CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY doctest/doctest
    GIT_TAG v2.5.3
    OPTIONS "DOCTEST_WITH_TESTS OFF" "DOCTEST_NO_INSTALL ON"
)

add_executable(sub0ecs_tests
    test_main.cpp
    test_entity.cpp
    test_store.cpp
    test_executors.cpp
    test_design_conformance.cpp
    test_fusion.cpp
    test_dynamic.cpp
    test_skirmish.cpp
)
# Include the real std headers instead of forward-declaring std (MSVC C5285)
target_compile_definitions(sub0ecs_tests PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS)
target_link_libraries(sub0ecs_tests PRIVATE sub0ecs_bench_support doctest::doctest)

# Tiers: the gate runs on every build and must stay fast (budget: ~5 s Release,
# ~60 s under sanitizers); heavier variants go in the "exhaustive" doctest suite,
# run on demand (ctest -L exhaustive, the `exhaustive` test preset, nightly CI)
# while a small sample of each stays in the gate.
add_test(NAME sub0ecs_tests COMMAND sub0ecs_tests --test-suite-exclude=exhaustive)
add_test(NAME sub0ecs_tests_exhaustive COMMAND sub0ecs_tests --test-suite=exhaustive)
set_tests_properties(sub0ecs_tests PROPERTIES LABELS gate)
set_tests_properties(sub0ecs_tests_exhaustive PROPERTIES LABELS exhaustive)

# Representative application conformance is part of the existing gate.
target_sources(sub0ecs_tests PRIVATE test_nbody.cpp test_row_grain.cpp)
if(SUB0ECS_WITH_PIPELINE)
    target_sources(sub0ecs_tests PRIVATE test_pipeline_pool.cpp)
    target_compile_definitions(sub0ecs_tests PRIVATE SUB0ECS_TEST_PIPELINE=1)
    target_link_libraries(sub0ecs_tests PRIVATE Sub0ECS::Pipeline Sub0Pipeline::Priority)
endif()
