# Shared-host diagnostic capture

GCC 13.3.0, Release `-O3 -ffp-contract=off`, portable x86-64 (no native tuning),
AMD EPYC 9V74 virtual host, eight-CPU cgroup quota. The receipt includes CPU info,
load before/after each process, full commands and SHA-256 of the tested binary.
Five pairs alternate order AB/BA/AB/BA/AB. No failed/slower timing arms were removed.
Power and thermal conditions are not controllable; these are not production
Crucible qualification numbers. NativeG64x2 has a conspicuous 1,024-body slow sample
(4.06 ms); it is retained. `summary.json` reports median and full range of process
medians. Plain and sequential ECS have ten samples each, the other arms five.

The compressed Callgrind graphs and function summaries are from Valgrind 3.27.1,
built from the official tarball (SHA-1 0b113f5c80d34743195b78651c2520901ba630cf).
Each process executes five ticks: two warmups plus three measured epochs. Ir
includes setup, shutdown and harness work. It is a dynamic guest instruction
reference count, not a hardware PMU receipt. Instrumented elapsed times are not
used in any speed comparison. The scalar source reduction remains ordered;
GCC reports control flow preventing loop vectorization and the linked code uses
scalar sqrt/division, alongside some packed basic-block operations. No fast-math
or reassociation optimization is inferred to be safe from this report.

Local validation: all 79 Release tests pass (376,083,392 assertions), including
exhaustive. Standalone C++20 application/grain tests: five pass. Fully instrumented
focused ASan/UBSan gate: seven pass; focused TSan adapter tests: three pass.
LeakSanitizer cannot operate under the environment's ptrace conditions; its failed
receipt is retained and is not a passing leak check. Cross-platform/full-sanitizer
receiving is delegated to the PR CI matrix. There was no VTune collection here.

The initial pre-grain exploratory run is retained in the workspace's ignored
build/nbody-evidence directory. It overlaps the tail of tool setup/test work and
is excluded from this curated five-pair timing series. These measurements support
an explicit chunk policy, not a claim that all workloads should use 64 rows.
