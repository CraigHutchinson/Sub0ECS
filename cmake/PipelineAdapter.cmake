# Optional integration only: the standalone ECS target remains C++20.
if(NOT TARGET Sub0Pipeline::Sub0Pipeline)
    include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)
    CPMAddPackage(
        NAME Sub0Pipeline
        GITHUB_REPOSITORY CraigHutchinson/Sub0Pipeline
        GIT_TAG f730c4ec2973a449c45fbf9a74595414b9bf30e1
        OPTIONS "SUB0PIPELINE_BUILD_TESTING OFF" "SUB0PIPELINE_BUILD_EXAMPLES OFF"
                "SUB0PIPELINE_BUILD_BENCHMARKS OFF" "SUB0PIPELINE_PLATFORM_DESKTOP OFF"
                "SUB0PIPELINE_PLATFORM_PRIORITY ON"
    )
endif()
add_library(Sub0ECS_Pipeline INTERFACE)
add_library(Sub0ECS::Pipeline ALIAS Sub0ECS_Pipeline)
target_link_libraries(Sub0ECS_Pipeline INTERFACE Sub0ECS::Sub0ECS Sub0Pipeline::Sub0Pipeline)
