# Full-stack workload contract

Reviewed 2026-10-10. PR20 is merged at `185f5c6789b7f5222eddb29c0517f8ee8cd45554`;
its ten push/PR GCC, Clang, macOS, MSVC and sanitizer checks passed. This increment
adds reproducible composition experiments, not a production performance acceptance.

## Sources and ownership

| Source | Frozen revision | Role |
|---|---|---|
| Crucible | `118ce62a75c8e6f1c325cc915cbc539c558a4c11` | Actual headless consumer and future workload requirements |
| ECS control / current | `1b1114ad2a15569ce106da429e94435912144f4f` / `185f5c6789b7f5222eddb29c0517f8ee8cd45554` | Crucible's pin / current merged library |
| Pub control / current | `d566c71c47cc5aeba3ed0b615052dbe6fcd91f23` / `504d772ecd8da6f22a22a26dd78ae3cb61c97868` | Crucible's pin / fixed benchmark dependency |
| Pipeline | `f730c4ec2973a449c45fbf9a74595414b9bf30e1` | Identical execution dependency in both builds |
| HexGrid | `aaae5c2fc5731a23db94fa947bbb0182d0ea69fd` | Actual consumer's unchanged spatial dependency |

At this source, `src/simulation.cpp` gathers mobile rows, sorts by ID, rebuilds
spatial inputs, calls joined `RowPartitions`, then uses a binary search for each
commit. It does not use `eachParallel` for this path. `runtime/IntentDelivery.cpp`
owns scoped Pub admission; `BoundaryPipeline.cpp` owns the inline boundary/capture/
publish graph. Production FP setup, failure handling, gameplay and rendering stay
in Crucible. This increment does not change its dependency pins or those contracts.

Pub owns routing, Pipeline owns jobs/join, ECS owns storage and membership. The
benchmark composes their existing interfaces; no umbrella library, second router,
new scheduler, or dependency on Pub in the public ECS target is introduced.

## Two complementary baselines

`bench/stack` is a **synthetic, fixed-population reproduction**: owned bounded
command admission, Pub receipt, Pipeline boundary graph, ECS gather, staged row
proposals through a joined Pipeline pool, ECS commit and double-buffered owned
publication. Two partitions with reversed insertion order prevent accidental
reliance on traversal order. Every warmup checks all rows against a handwritten
complete-boundary floor. Timed epochs advance every alternative equally.

| Arm | Difference | Contract |
|---|---|---|
| HandWritten | Contiguous loop and owned publication | Optimized complete-work floor; sequential, no routing/scheduler cost |
| DomainSearch | Scoped locked Pub, sorted staging and binary-search commit | Mechanism control, not a copy of all Crucible rules |
| WiringSearch | Fixed Pub wiring | Same synchronous single endpoint, queue and receipts; no dynamic registration/concurrent disconnect claim |
| DomainDense / WiringDense | Rebuild bounded ID-to-proposal index each tick | Same sorted staging; no cached row pointers or stale epochs |
| DomainIndexed / WiringIndexed | Gather and commit directly by ID | Stronger fixed complete permutation of `[0,n)`; invalid for arbitrary sparse/mobile-only sets |

All stack arms use the same worker count and 256-row grain within a group. The
handwritten floor intentionally needs no task dispatch; compare integrated arms
at equal resources to attribute a mechanism. Empty/small/tail/threshold worlds,
1/2/4 lanes, overflow with no partial admission, copied borrowed input, publication
age and scoped session teardown are covered by `tests/test_stack.cpp`. Unsigned
integer updates are bit-exact and deliberately do not claim Crucible's FP receiving.
No failure-atomic whole-tick guarantee is claimed: staged proposals are joined,
but admission is drained before work and arbitrary exceptions are not a rollback.

`bench/tools/crucible.py` captures **the existing actual consumer executable**.
It runs direct and integrated routes in alternating process order for each build,
retains raw JSONL/stderr and exact source/pins/cache/compile commands/binary hashes,
and rejects failed/empty/incomplete receipts or changed state/command traces.
Complete reference and structural missions run by default; `--skip-missions` is
only a short probe. Aggregate checksum equality is weaker than per-field bitwise
replay, so this capture does not replace Crucible's own correctness tests.
The direct route is a production composition comparator, not a handwritten game.

## Reproduction

```sh
cmake -S . -B build/stack -DCMAKE_BUILD_TYPE=Release \
  -DSUB0ECS_BUILD_BENCHMARKS=ON -DSUB0ECS_WITH_PIPELINE=ON \
  -DSUB0ECS_STACK_BENCHMARKS=ON
cmake --build build/stack --parallel 2
ctest --test-dir build/stack --output-on-failure
python3 bench/tools/run.py --build-dir build/stack --profile stack
python3 bench/tools/rotate.py --build current=build/stack --rounds 5 \
  --suites stack --profile stack --pin 0-3 --label stack
```

The stack profile keeps the existing ordered/compact n-body and streaming/churn/
staged-neighbor suites alongside the new boundary. `stack` is optional, so core
C++23 users and the ordinary quick profile do not acquire Pub. CI enables the
optional tests on the Pipeline platforms and runs a small stack smoke on GCC.
Keep an omitted-dependency Clang build as the standalone gate.

Build the frozen Crucible checkout twice with `BUILD_TESTING=OFF`,
`CRUCIBLE_BUILD_BENCHMARKS=ON`, `CMAKE_BUILD_TYPE=Release`, and desktop/GPU OFF.
The control uses its committed dependency pins. For the current-library arm use
CPM's `CPM_Sub0ECS_SOURCE` and `CPM_Sub0Pub_SOURCE` to point at **clean checkouts of
the exact current revisions above**. Pipeline and HexGrid remain pinned. Build
`crucible_runtime_backbone_bench` in each tree. Then, from Sub0ECS:

```sh
python3 bench/tools/crucible.py --source /path/to/Crucible \
  --build control=/path/to/Crucible/build/control \
  --build current=/path/to/Crucible/build/current \
  --rounds 5 --timeout 600 --out bench/results/runs/crucible-stack
```

Output directories must be new. Keep source and builds frozen throughout capture;
source metadata describes the supplied checkout, not a cryptographic proof of
compiler inputs. Each process has a timeout and preserved stderr; this headless
binary starts threads, not external child processes. It is not a general profiler
supervisor. Run without competing builds; record affinity and uncontrolled host
conditions. No native-frame, thermal, hardware-counter or 100K-at-60FPS claim follows.

## Crucible workload and exhaustive logic reference

These requirements come from the frozen `docs/decisions/phase14-scale-contracts.md`,
`docs/phases/phase15.md`, `docs/game-design.md`, and actual Simulation/Runtime code.
Keep product policy in Crucible; add neutral reproductions here when a cost points
upstream. Future entries are requirements, not implemented coverage.

| Workload | Available baseline | Required next logic / lifecycle coverage | Owner |
|---|---|---|---|
| Ordered all-pairs forces | Existing n-body scalar/compact, matched native/Pipeline controls | FP edge distributions, source order and target SIMD without changed reductions | ECS benchmark; kernel experiment |
| Streaming / fragmented state | Existing row workloads and dispatch diagnostics | Many tiny/empty partitions, sparse identity, churn plus destroy/reuse generations | ECS |
| Command boundary / publication | New stack plus actual direct/integrated headless capture | Burst admission, overflow atomicity, reentrant refusal, request exhaustion, restart generations and last-good-frame failures | Pub contracts; Crucible policy |
| Complete spatial steering | Actual consumer's ordinary/reference missions | Complete neighbors under clustered/dense occupancy, mobile subset, stable reduction order; 100K/400x250 and 150K/500x300 kept distinct from 2K dense stress | Crucible Spatial/Swarm; HexGrid |
| Parallel proposals | Synthetic joined pool; production RowPartitions | FP install/restore, rounding/denormals, poisoned workers, partial submission failure and callback/destructor join; bitwise 1/2/N replay | Pipeline/ECS bridge; production Scheduling |
| Structural resources | Actual complete structural mission; synthetic consumed tag churn | Conservation, fuse/split eligibility, starvation, depletion, reclamation/protection and command-before-movement order | Crucible Interactions/Runtime |
| Snapshots and frontend | CPU owned publication reproduction | Publication failure, retained views across restart, coherent picking/camera/run IDs, bounded observation overflow | Crucible Runtime/Presentation |
| Production rendering | No headless substitute | GPU slot ownership, fences, backpressure, resize/device-loss, useful motion, p95/p99 complete frame/input latency on received native hardware | Crucible production renderer |
| Future terrain/growth/factions | Planning reference only | Freeze gameplay/numeric oracle, spatial/lifecycle contract and deterministic input trace before benchmark implementation | Crucible game design |

Dense indexed staging should be reconsidered first at the actual caller, with an
explicit valid-ID/mobile map and complete missing/duplicate/stale ID tests. A
static wiring production arm also needs its real receipt/exception/restart gates.
Neither synthetic win licenses a blanket library storage or broker default change.

## Received results

See the [curated receipt](../../bench/results/reference/work-mode-stack/README.md)
for five library process captures, 20 actual-consumer captures, profiling and
validation. Indexed synthetic staging is a useful direction; actual Crucible
library-version timing ranges overlap. No production pin update is made here.
