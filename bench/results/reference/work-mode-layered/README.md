# Layered optimization receipt

Base: `459a0d0f2eceeca97c0d10ce365f023a173a7548`. See
[the review](../../../../docs/optimization/layered-review.md) for the architecture
and retained implementation. `manifest.json` identifies source and binary hashes;
all source changes needed to reproduce the final candidate are in this commit.
The control uses the base `world.hpp` with the same new benchmark sources.

## Final measurements

GCC 13.3.0, portable x86-64, Release `-O3 -ffp-contract=off`, no LTO/native tuning.
AMD EPYC 9V74 virtual host, pinned to logical CPUs 0–3, eight-CPU cgroup quota.
Five independent alternating process pairs; each group uses 11 interleaved epochs.
All five successful samples per arm are included below, including those marked
BUSY. `final/rotation.md` also retains the harness's normal busy-sample filtering.
Ranges are the full range of process medians, not confidence intervals.
Power, thermal state and external host contention are uncontrolled.

Times below are microseconds per complete row pass. Relative throughput is the
paired handwritten time / ECS time; 1.0 means parity. Grain1 is deliberately an
extreme bookkeeping diagnostic, with an inline pool, not a recommended task size.

| Case | Control median [range], µs | Candidate median [range], µs | Candidate / handwritten throughput [range] |
|---|---:|---:|---:|
| Coherent/Grain1/100000 | 231.6576 [217.4114–272.1064] | 20.5513 [20.2347–22.0720] | 1.020 [0.994–1.035] |
| Coherent/Grain64/4096 | 1.1175 [1.0360–1.2069] | 0.7060 [0.6951–0.7917] | 0.980 [0.976–1.005] |
| Coherent/Grain64/100000 | 28.1904 [26.7439–31.6024] | 21.1054 [20.4494–22.0881] | 1.019 [0.995–1.042] |
| Coherent/Grain1024/100000 | 21.9842 [21.3232–27.8591] | 20.7950 [20.5347–22.7308] | 1.009 [1.003–1.022] |
| Coherent/Grain1024/64 | 0.0179 [0.0177–0.0199] | 0.0227 [0.0225–0.0266] | 0.620 [0.604–0.675] |
| Fragmented/Grain1024/100000 | 22.4755 [21.5993–25.5946] | 21.3572 [21.0512–23.0865] | 0.986 [0.967–1.013] |

The 100K-row grain-1 case improves 11.3× by the ratio of process medians; the
4096-row grain-64 case improves 1.6×. These are diagnostic workloads. The small
64-row/default-grain path costs about **4.8 ns more per call** (17.9 → 22.7 ns),
a retained setup tradeoff, not a universal speedup. Unchanged `each` timings also
move across builds: do not attribute those changes to an `each` source change.
The full executable text shrinks 464 bytes; data and BSS are unchanged.

For matched 4096-body n-body groups, candidate median relative throughput across
native/Pipeline 1/2/4 lanes is 0.977–1.019 of handwritten. Complete-tick A/B ranges
overlap substantially; **no qualified n-body application speedup is established**.
The shared scalar kernel is an equal-work control, not the best possible n-body
algorithm. All individual rows, ratios, ranges and outliers are in
`summary-all-samples.json` and `final/`.

## Validation

- Full assertions-enabled GCC `-O1` build with Pipeline: all 14 CTest entries pass
  (12 examples plus gate/exhaustive), 81 doctest cases and 376,137,592 assertions.
- Fully instrumented focused ASan/UBSan, including Pipeline implementation sources:
  9 cases / 70,131 assertions pass. Leak detection disabled; no leak-check claim.
- Focused standalone C++20 validation: 6 cases / 33,210 assertions pass.
- Python profiler tests: 6 tests pass. No TSan or cross-platform result is
  asserted by this local receipt; those require separate validation.
- Initial full Release parallel compilation exhausted the 8 GiB container budget;
  full local validation therefore uses assertions-enabled `-O1`. Benchmark binaries
  remain Release `-O3`. The focused standalone binary uses `-O2`.

## Refinements and profiling

`layered/`: first variant, absolute 64-bit indices, five pairs including n-body.
`row-index/`: second variant, 32-bit indices, three row-only pairs.
`offset-columns/`: third variant, chunk-offset pointers, three row-only pairs.
`final/`: retained third variant, five pairs including matched n-body controls.
The first two variants are preserved as patches against the base. They were
rejected for wider-grain regressions. The first two series ran on host
`3d8b9b3722fa`; resumed comparisons ran both arms on `0e19da0ca84a`. Do not compare
absolute times across these hosts. Earlier candidate n-body binaries were not
retained; the final pair has complete binary/source hashes.

The gprof capture places 63 samples in the original inlined dispatch operation;
no line-level or hardware-counter attribution is claimed. `control-*` and
`candidate-*` diagnostics refer to the original and first variant. `final-*`
contains the retained loop's compiler diagnostics, linked disassembly excerpt,
size and validation. Instrumented times are excluded from speed comparisons.

## Reproduce

Configure both trees with `SUB0ECS_BUILD_BENCHMARKS=ON`,
`SUB0ECS_WITH_PIPELINE=ON`, `CMAKE_BUILD_TYPE=Release`, native tuning OFF. Use the
same new benchmark files in the control tree with base `world.hpp`; this avoids
comparing different workloads. The exact n-body compile/link commands and
include-overlay path are recorded in `nbody-build-commands.json`.

```sh
NBODY_SIZES=64,1024,4096 NBODY_THREADS=1,2,4 python3 bench/tools/rotate.py \
  --build control=build/control --build candidate=build/candidate \
  --rounds 5 --suites rows,nbody --profile compare --pin 0-3 \
  --cooldown 1 --quiet-timeout 0 --label final-layered
```

Choose allowed CPUs on another machine. `meta.json` files retain original absolute
paths as receipts; the raw files are stored alongside each metadata file here.
`manifest.json` records the inherited n-body environment explicitly because the
standard runner metadata lists only profile-specific overrides.
