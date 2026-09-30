# Unit tests (doctest). One executable, registered with ctest.

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
target_include_directories(sub0ecs_tests PRIVATE vendor)
# Include the real std headers instead of forward-declaring std (MSVC C5285)
target_compile_definitions(sub0ecs_tests PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS)
target_link_libraries(sub0ecs_tests PRIVATE sub0ecs_bench_support)

add_test(NAME sub0ecs_tests COMMAND sub0ecs_tests)
