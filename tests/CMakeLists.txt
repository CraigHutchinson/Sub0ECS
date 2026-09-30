# Unit tests (doctest). One executable, registered with ctest.

include(CPM)
# Header-only: fetched, not configured (its CMakeLists predates what CMake 4 accepts).
CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY doctest/doctest
    VERSION 2.4.11
    DOWNLOAD_ONLY YES
)
add_library(sub0ecs_doctest INTERFACE)
target_include_directories(sub0ecs_doctest SYSTEM INTERFACE ${doctest_SOURCE_DIR})

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
target_link_libraries(sub0ecs_tests PRIVATE sub0ecs_bench_support sub0ecs_doctest)

add_test(NAME sub0ecs_tests COMMAND sub0ecs_tests)
