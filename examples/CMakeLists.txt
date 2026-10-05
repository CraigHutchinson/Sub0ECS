# Examples: one small standalone program per feature (see docs/EXAMPLES.md). Each is
# built and run by CTest as Sub0ECS_Example_<name> and fails through its exit code.

function(sub0ecs_example name)
    add_executable(sub0ecs_example_${name} ${name}.cpp)
    target_link_libraries(sub0ecs_example_${name} PRIVATE Sub0ECS::Sub0ECS sub0ecs_dev_flags Threads::Threads ${ARGN})
    add_test(NAME Sub0ECS_Example_${name} COMMAND sub0ecs_example_${name})
    set_tests_properties(Sub0ECS_Example_${name} PROPERTIES LABELS "gate;example")
endfunction()

# Core usage
sub0ecs_example(minimal_world)
sub0ecs_example(rocket)
sub0ecs_example(hinted_partitions)
sub0ecs_example(structural_changes)
sub0ecs_example(random_access)

# Scheduling
sub0ecs_example(fusion_planners)
sub0ecs_example(auto_tuner)
sub0ecs_example(executors)
sub0ecs_example(parallel_systems)
sub0ecs_example(determinism)

# Advanced usage
sub0ecs_example(dynamic_systems)
# The RTS testbed lives with the benchmarks, which share it.
sub0ecs_example(skirmish sub0ecs_bench_support)
