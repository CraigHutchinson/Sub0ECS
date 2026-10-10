# Performance guide

Sub0ECS targets optimized handwritten code for the **same complete work and
correctness contract**, while retaining library flexibility. A fast row loop is
only one part of that goal: staging, membership changes, dispatch, routing, joins
and publication can dominate a complete tick.

## Choose a workload

| Layer / question | Suite or capture | Contract and evidence |
|---|---|---|
| Storage, lookup, fusion and structural change | `baseline`, `fusion`, `fusion-exec`, `dynamic`, `spans` | [Reference designs](../../bench/README.md), [design findings](../FINDINGS.md), [original captures](../../bench/results/reference/README.md) |
| Dispatch bookkeeping and partition shape | `rows` | [Layered architecture-to-function review](layered-review.md), [receipt including the small-world regression](../../bench/results/reference/work-mode-layered/README.md) |
| Complete streaming, churn and staged-neighbor ticks | `row-workloads` | [Representative review](representative.md), [five-pair receipt](../../bench/results/reference/work-mode-representative/README.md) |
| Ordered all-pairs forces and compact snapshots | `nbody` | [Numeric and scheduling contract](nbody.md), [compact-kernel comparison](../../bench/results/reference/work-mode-representative/README.md) |
| Stateful game testbed and thread scaling | `skirmish`, `threads` | [Skirmish model](../../bench/skirmish/README.md), [original captures](../../bench/results/reference/README.md) |
| Pub admission → Pipeline → ECS → publication | Optional `stack` | [Full-stack contract](full-stack.md), [synthetic and consumer receipt](../../bench/results/reference/work-mode-stack/README.md) |
| Actual Crucible direct/integrated routes and complete missions | `bench/tools/crucible.py` | [Frozen sources and build/capture instructions](full-stack.md#reproduction), [future workload and exhaustive logic requirements](full-stack.md#crucible-workload-and-exhaustive-logic-reference) |

The first storage bars are a plain handwritten loop and an explicitly tuned SIMD
reference. Parallel n-body/row comparisons include matched handwritten execution
controls. The stack floor is a sequential handwritten complete boundary; compare
integrated alternatives at equal worker counts to isolate composition changes.
Crucible's direct route is a production comparator, not a handwritten game.

## Run from the repository root

Use CMake 3.25+, Ninja, Python 3 and a [C++23 toolchain](../cxx23.md).
Dependencies are fetched during configuration. First check conformance and the
harness; `quick` timings are smoke checks, not performance evidence:

```sh
cmake --preset default
cmake --build --preset default
ctest --preset default
python3 bench/tools/run.py --build-dir build/default --profile quick
python3 bench/tools/run.py --suites list
```

For complete row workloads and n-body, the native controls work without optional
integrations. To include the Pipeline controls, configure a separate build:

```sh
cmake -S . -B build/performance -DCMAKE_BUILD_TYPE=Release \
  -DSUB0ECS_BUILD_TESTING=ON -DSUB0ECS_BUILD_BENCHMARKS=ON \
  -DSUB0ECS_WITH_PIPELINE=ON
cmake --build build/performance --parallel 2
ctest --test-dir build/performance --output-on-failure
python3 bench/tools/run.py --build-dir build/performance --profile representative
```

`representative` includes quadratic n-body up to 4096 bodies; use `quick` for a
short check. `nbody` selects only that workload. `standard` and `reference` cover
the original suite set; they do not automatically include the newer complete-tick
suites. The exact sizes, lanes and suite membership live in
[`suites.json`](../../bench/tools/suites.json).

For Pub/Pipeline/ECS, enable **both** `SUB0ECS_WITH_PIPELINE=ON` and
`SUB0ECS_STACK_BENCHMARKS=ON`, then use `--profile stack`. Follow the
[full-stack recipe](full-stack.md#reproduction), which also explains how to build
and capture the actual Crucible executable. Optional integrations do not add Pub
or Pipeline to the standalone public ECS target.

For a change comparison, build clean control/candidate revisions with identical
compiler, flags and dependency pins, then rotate processes:

```sh
python3 bench/tools/rotate.py --build control=build/control \
  --build candidate=build/candidate --profile representative --rounds 5 \
  --label representative-change
```

Choose and record affinity appropriate to your machine using `--pin`; run without
competing builds. Preserve output under `bench/results/runs/` and curate accepted
receipts into `bench/results/reference/`. See [the harness guide](../../bench/BENCHMARKING.md)
for profiling, metadata, result interpretation and noise checks.

## Read the evidence before generalizing

- Start with the paired ratio to a handwritten implementation doing the same
  work. Keep numeric order, completed operations, state checks and resources
  comparable; never weaken a reference to improve the ratio.
- Report machine, compiler, flags, pins, independent process count and spread.
  Epochs within one process are not independent build comparisons. Archived
  receipts retain their actual flags, including pre-C++23 captures.
- Compact n-body and row dispatch have measured improvements in their named
  workloads. The dispatch receipt also retains a small-world regression.
- Indexed synthetic staging needs a complete fixed dense-ID population. It
  remains slower than its handwritten floor and is not valid for arbitrary
  sparse/mobile-only sets without additional mapping and lifecycle checks.
- Actual Crucible timing ranges overlap. The current capture establishes a
  reproducible baseline, **not a qualified production speedup**. Checksums do
  not replace per-field bitwise conformance or native rendering/latency tests.

Continue from the [consumer optimization campaign](crucible.md),
[future Crucible workload matrix](full-stack.md#crucible-workload-and-exhaustive-logic-reference)
and [backlog](../BACKLOG.md). Product policy stays in Crucible; neutral upstream
reproductions belong here when profiling attributes a cost to the libraries.
