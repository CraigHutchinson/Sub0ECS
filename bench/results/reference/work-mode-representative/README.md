# Representative workload receipt

Base: merged PR19, `671eedb3bf1b92a668692640fbf5bf50a50e1b74`.
The final sources are in this commit; `manifest.json` records source/binary hashes
and build commands. No library header changes are retained in this increment.
See [the consolidated review](../../../../docs/optimization/representative.md).

## Method and scope

The final capture uses one Release GCC 13.3 executable, portable x86-64,
`-O3 -ffp-contract=off`, no fast-math, native tuning or LTO. Five independent
control/candidate process pairs alternate AB/BA/AB/BA/AB. Each process executes
only the original or compact n-body kernel, with the same four arms, sizes,
2-lane resource count and 64-row grain. Each group uses the common interleaved
harness, two equal warmup ticks and one complete tick per epoch. The collector
rejects unequal completed work or checksum within groups and between A/B processes.
Checksums are coarse smoke checks; bitwise tests provide conformance.

Every pair also captures the same-resource handwritten native/Pipeline groups
and all three representative row workloads at 1/2 lanes and grains 64/1024.
Construction, pool startup and final state inspection are untimed. Source
staging, row updates, joins and selected structural/behavior work are timed.
All raw samples, including slow ones, remain in `final/`; `summary.md` reports
medians and full ranges of process medians. Ranges are not confidence intervals.
CPU identity, cgroup quota, load and timestamps are in `final/receipt.json`.
Power, thermal state and external contention are uncontrolled on this virtual host.
These are advisory library application measurements, not production qualification.

## Mechanisms and discarded work

1. Compact snapshot pass 1 stored position/scaled mass in 32 bytes instead of
   reading 56-byte bodies; the self branch remained. It gave a small/noisy change.
2. Compact pass 2 split each ascending source loop around self. This removes the
   inner equality branch without reassociating the reduction. It is retained.
3. Reconciliation with PR19 added a compact handwritten bar and reran the final
   comparison against the merged arithmetic row-dispatch implementation.

Compact ECS still allocates its full-state inspection cache and adds 32 bytes/body
of scratch. Compact handwritten storage replaces its old input snapshot. A smaller
hot input is not a claim of lower total ECS allocation or a universal layout win.
The independent scalar physics function `advanceBody` is unchanged.

`rejected-pre-pr19.tar.gz` preserves earlier source patches, raw measurements and
profiles against PR18. It includes three one-lane traversal experiments: shared
inline lambda (streaming gain, churn regression), direct `each` delegation (lost
streaming gain), and a private flattened helper (recovered streaming gain).
The final timing series for that helper was promising for native single-lane
streaming, but integration review rejected **all three**: a width query alone
does not authorize bypassing custom pool dispatch. None is shipped. Those numbers
are historical, are not combined with current samples, and are not current claims.
The archive also preserves the slower churn follow-up and pre-integration tests.

## Reproduce

```sh
cmake --preset default -DSUB0ECS_WITH_PIPELINE=ON
cmake --build --preset default
ctest --preset default
ctest --preset exhaustive
python3 bench/results/reference/work-mode-representative/collect.py \
  build/default/bench build/representative-receipt
python3 bench/results/reference/work-mode-representative/summarize.py \
  build/representative-receipt
```

The local build uses Pipeline source override at the exact default pin
`f730c4ec2973a449c45fbf9a74595414b9bf30e1`; the pinned dependency is unchanged.
Run after builds and instrumented tools stop. Keep controls on the same machine,
compiler and executable. The broader `--profile representative` includes 1/2/4 lanes.

## Instruction attribution

Callgrind 3.27.1 records dynamic guest instruction references (`Ir`), including
startup, harness, two warmups and shutdown; these are not hardware retired
instruction counts or per-kernel normalized counts. Instrumented elapsed times
are excluded from timing results. VTune and usable PMU sampling are unavailable
on this host. Compiler vectorizer diagnostics and linked disassembly accompany
the instruction capture. Exact commands are retained with the manifest.

At 1,024 bodies, two Pipeline lanes and five ticks, total-process Ir falls from
184,491,868 to 119,168,119 (35.4%). Callgrind attributes 178.3M references to the
original inlined physics lines and 110.3M to compact force lines; setup and other
attribution remain in the raw graphs. GCC reports both split loops vectorized
with 16-byte vectors. The linked Pipeline callback contains packed `sqrtpd` and
`divpd`; low/high contributions are accumulated in source order (x/y in separate
packed lanes, z using sequential `addsd`). The original inner force loop uses
scalar sqrt/division. This is compiler-generated SIMD under the unchanged strict
FP flags, supported by bitwise conformance, not an unsafe reassociated reduction.

An initial profiling attempt found a zero-byte benchmark link output and failed
with exit 126. The empty generated output was removed and the target relinked;
its smoke run then passed before the successful profiles. The failed command/log
is preserved; it contributes no measurements.

## Validation and current results

Local Release receiving passes all 14 CTest entries (12 examples plus gate and
exhaustive), 84 doctest cases and 377,684,382 assertions. The focused joined-work
binary passes 13 cases / 1,731,641 assertions. Six Python profiler failure tests
also pass. Standalone C++20 focused receiving passes 10 cases / 1,121,765
assertions. Fully instrumented ThreadSanitizer receiving, including Pipeline's
implementation sources, passes 8 gate cases / 1,444,551 assertions; the quadratic
exhaustive case is excluded from that run. The complete quick benchmark profile
passes with both new suites. Exact logs accompany this receipt; cross-platform
and ASan/UBSan receiving are checked on the PR's exact head before merging.
The independent review still requires production Crucible FP/replay/staging,
frame p95/p99, lifecycle and actual pinned-consumer receiving. Fixed-neighbor
work is synthetic and does not establish spatial-grid performance. No global
grain/worker policy or hazard-derived scheduler is selected by these measurements.

The current headline is the same-resource Pipeline 1,024-body tick: 2.429 ms
[2.333–2.632] → 1.538 ms [1.388–1.680], 36.7% less elapsed time by ratio of
process medians. At 4,096 bodies the corresponding medians are 36.821 → 22.755 ms,
with a retained slow compact sample at 34.553 ms. Native compact timings are also
noisy. See [all samples](final/summary.md), [machine-readable summary](final/summary.json)
and [FINDINGS](../../../../docs/FINDINGS.md) for the handwritten and broader workload
comparisons. Same-pool matched groups are reported separately; there is no
universal zero-cost or production-speedup claim.
