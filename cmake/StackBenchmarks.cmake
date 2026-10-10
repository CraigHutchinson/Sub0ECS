# Optional benchmark-only integration. No Pub dependency on the public ECS targets.
if(NOT SUB0ECS_WITH_PIPELINE)
    message(FATAL_ERROR "SUB0ECS_STACK_BENCHMARKS requires SUB0ECS_WITH_PIPELINE=ON")
endif()
if(NOT TARGET Sub0Pub::Sub0Pub)
    include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)
    CPMAddPackage(NAME Sub0Pub GITHUB_REPOSITORY CraigHutchinson/Sub0Pub
        GIT_TAG 504d772ecd8da6f22a22a26dd78ae3cb61c97868
        OPTIONS "SUB0PUB_BUILD_TESTING OFF" "SUB0PUB_BUILD_EXAMPLES OFF")
endif()
add_library(sub0ecs_stack_support INTERFACE)
target_link_libraries(sub0ecs_stack_support INTERFACE sub0ecs_bench_support
    Sub0Pub::Sub0Pub Sub0ECS::Pipeline Sub0Pipeline::Priority)
