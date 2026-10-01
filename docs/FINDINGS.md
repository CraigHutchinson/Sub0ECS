# SubzeroECS v2 spike — findings

Baseline: [results/baseline-linux-gcc13.md](../bench/results/baseline-linux-gcc13.md)
(raw: [JSON](../bench/results/baseline-linux-gcc13.json)). Host: 4 vCPU Xeon @ 2.1 GHz
(shared cloud VM), GCC 13.3, `-O3 -march=native`, FTZ/DAZ, median of 5.

**Noise caveat:** memory-bound rows at N = 1M drift about ±30% between runs on
this host. For example, a rerun of Frame3/1M gave V1 6.0 ms against 4.5 ms in the
baseline, and SortedSoA 5.9 ms against 8.2 ms. Treat gaps under ~1.5× at 1M as
noise. The 1K and 100K rows are stable (CV < 5% except rows marked ⚠). Re-run
on quiet reference hardware (the i7-11800H/MSVC box from the v1 README, and an
ESP32-P4) before making final decisions.

## Headline numbers (N = 100K, speed-up vs v1)

| Scenario | SortedSoA | SparseSet | Archetype | StaticBitmask |
|---|---:|---:|---:|---:|
| Update2 (v1 headline) | 1.1× | 1.2× | **6–6.6×** (RawSoA roofline: 7.4×) | 1.5× |
| Frame3 (3 systems) | 0.9× | 1.15× | **4.4×** | 1.5× |
| SparseQuery (1% tag) | 3.9× | 20× | **260×** | 1.9× |
| RandomGet | 1.0× | 18× | 10× | **76×** |
| Create (fragmented) | 1.7× | 1.1× | 1.4× | **9×** |
| AddRemove 10% (v1: unsupported) | 137 µs | 62 µs | 1.13 ms | **30 µs** |
| DestroyCreate 10% (v1: unsupported) | 1.27 ms | 442 µs | 462 µs | **120 µs** |
| Heap bytes/entity (fragmented) | 86 | 148 | 91 | 90 (capacity-bound) |

## What we learned

1. **v1's performance is capped by its data model, not its code.**
   SortedSoA removes every known v1 cost: `.at()` checks, the tuple-of-iterators
   view, the static registry, and the missing remove/destroy. That buys only
   1.1–1.9×, and random access stays O(log n) (1.0× at 100K).
   Sorted-id intersection cannot give the compiler a contiguous, branch-free
   loop, so it never gets near the roofline. **Recommendation: do not carry
   the v1 storage model into v2.**

2. **Only archetype tables reach the roofline on iteration.** Every matched
   archetype becomes plain typed column pointers, so GCC auto-vectorises the
   kernel. Update2 runs at 85–95% of hand-written SoA at every size. Every
   design that looks up per entity (V1, SortedSoA, SparseSet, StaticBitmask)
   lands at 1.1–1.6× of v1, i.e. 4–6× off the roofline. The rare-tag query is
   also near-free because tagged entities live in their own archetypes.

3. **Archetypes pay for this on structural change.** Adding and removing
   one component on 10% of entities is 18× slower than SparseSet at 100K,
   because every column of the row is memcpy'd between tables. Churn-heavy
   components (status flags, "frozen", "selected") need a separate path.

4. **Sparse set is the best all-rounder but not the fastest at anything
   iteration-bound.** It has O(1) add/remove/destroy and 18× faster random
   access. Its iteration is limited by the per-entity sparse lookup, which
   EnTT addresses with owning *groups* (not spiked). It also uses the most
   memory: flat sparse arrays cost 4 B × max-index per component. Paging
   would fix that.

5. **Static fixed-capacity wins wherever N is known.** It has zero heap
   allocations after construction, 9× faster create, the fastest random access
   and structural change, and a predictable footprint. Its weakness is that
   queries scan all slots (0.7× v1 on Iter1, linear-in-N sparse queries). That
   suits ESP32-class targets with bounded N and mostly-dense components.

## Recommendation for v2 (to validate, not yet decided)

The core should be **archetype tables** with two escape hatches:

- **Sparse-set side storage per component** (the flecs "sparse" trait, Bevy
  `SparseSet` storage), opt-in via a component trait. It serves tags and
  high-churn components so they don't trigger table moves.
- **Deferred command buffer** for structural changes, applied at sync points.
  Batching amortises the table moves. It is also the prerequisite for running
  systems in parallel under Sub0Pipeline, where systems become jobs and
  commit points become DAG edges.

For embedded, a **fixed-capacity allocator/policy** should back the same
archetype columns. Option D's measurements show that zero-allocation,
capacity-bounded storage is valuable in its own right; the policy keeps that
without forking the query model.

## Open questions / next spikes

1. **Hybrid spike:** archetype plus sparse-side-storage in one design. Re-run
   AddRemove with `Frozen` marked sparse: does it approach SparseSet's 62 µs
   while Update2 stays near the roofline?
2. **EnTT owning groups:** does SparseSet with groups close the iteration gap?
   If so, it becomes a genuine alternative core with cheaper structural change.
3. **Embedded run:** cross-compile the harness for ESP32-P4 (no `-march=native`,
   small N ≤ 4K, SRAM budget). The desktop ranking may not hold on in-order
   cores without SIMD auto-vectorisation.
4. **MSVC / reference host:** reproduce on the v1 README machine for continuity
   with published v1 numbers. *Update 2026-09-30:* MSVC 19.51 builds every
   target clean and passes all four suites; the harness now runs on Windows
   (see "MSVC validation" below). The original i7-11800H box was not available;
   the reference capture runs on a Core Ultra 9 275HX.
5. **Archetype fragmentation stress:** many archetypes with few entities each,
   e.g. 2^k combinations of k optional components, plus query-cache
   invalidation cost.
6. **Component constraints:** the archetype spike requires trivially copyable
   components (memcpy moves). Decide whether v2 accepts that or needs
   type-erased move/destroy vtables.
7. **v1 benchmark hygiene:** v1's published numbers depend on `-ffast-math`
   implicitly setting FTZ/DAZ. Without it, the kernel runs on denormals after
   ~9K iterations and results depend on iteration count. v1's Linux configure
   is also broken (`find_package(fmt)` vs `FindFmt.cmake` case).

## Follow-up research

[research/holographic-storage.md](research/holographic-storage.md) covers
system-driven ("holographic") storage. Partitions are keyed by which systems
an entity matches rather than by which components it has; the document covers
use cases, prior art, the design, a proposed Sub0DataStore library split, reference bindings (no copies by default) and
validation spikes H1–H5.

[research/executor-async.md](research/executor-async.md) designs the executor
contract for accelerators: split-phase `prepare / submit / retire` with a
completion handle, and column residency tracked by the store (validation spike H10).

## H1 results: query-signature partitions ("automatic archetypes")

Design: [designs/query_partition.hpp](../include/sub0ecs/store/world.hpp). Numbers:
[results/h1-query-partition-linux-gcc13.md](../bench/results/h1-query-partition-linux-gcc13.md)
(speed-up column is vs Archetype; same host, 5 repetitions, random interleaving).

**Verdict: H1 passes with the hinted variant; the pure automatic variant
fails on memory.** The core idea works. A partition key derived from the
declared system set gives archetype-class iteration *and* sparse-set-class
churn for components no system requires.

| Criterion (100K; 1M in brackets) | Target | QueryPart (pure auto) | QPartHinted (unqueried carried, `Volatile<Frozen>`) |
|---|---|---|---|
| Update2 / Frame3 / SparseQuery vs Archetype | within 10% | 1.08× / 1.01× / 1.19× faster ✅ | 1.19× / 0.98× / 1.15× ✅ |
| AddRemove of an unqueried component vs SparseSet | ≤ 1.2× | 0.99× (1.06×) ✅ | 1.09× (1.10×) ✅ |
| Heap bytes/entity vs Archetype (fragmented) | ≤ +10% | +36% (+33%) ❌ | +1.4% (+0.3%) ✅ |
| Physical tables after Frozen churn | fewer | 3 vs 6 ✅ | 3 vs 6 ✅ |

Against Archetype, AddRemove is **15× faster** (80 µs vs 1.19 ms at 100K).

What the numbers say:

1. **Pure side storage is the wrong default for stable unqueried data.**
   Team, Flags, and Scale on Medium entities are present on a third or more
   of entities and never churn. Sparse sets cost them 4 B × max-index plus a
   dense entity id each (the SparseSet memory problem again). Carrying them
   as dense columns (extra key bits) costs nothing, because they don't
   change. The planner's per-component choice (research §4.5) is therefore
   necessary, not optional:
   - **carry** stable unqueried components;
   - **side-store** volatile ones;
   - **key-bit** the queried ones.

   The volatility input has to come from a hint (`Volatile<T>`) or, in
   adaptive mode, measurement.
2. **Churn isolation needs one more invariant than planned.** The first cut
   was still 4.4× SparseSet on AddRemove. Profiling showed the cause:
   updating the per-entity record on every non-fragmenting add/remove,
   which touches an extra cache line callgrind doesn't model. Fix: the
   record tracks only *fragmenting* components. Membership of
   non-fragmenting components lives solely in their pool, which makes the
   op literally a sparse-set op.
3. **Churn on a queried component still moves data.** TagChurn is 0.98×
   Archetype, as expected: H1 removes needless moves, not necessary ones.
   Carrying more columns (hinted) makes those moves 21% dearer (0.79×).
   Mitigations to test next: enable bits for high-toggle queried
   components (research §4.5) and batched moves at commit (§4.6).
4. **Regressions to address in the real implementation:**
   - RandomGet is 0.71× Archetype: `find()` goes record → partition table →
     column; caching column base pointers per partition would help.
   - Create is 0.75–0.88×.
   - DestroyCreate (pure) is 0.84×: destroy also has to scrub the
     non-fragmenting pools.

   None of these is a model limit.
   *Update (optimisation pass, below):* RandomGet is now 0.86–1.12×, Create
   0.83–1.30×, DestroyCreate 1.14–1.23× faster than before.

**Recommendation:** proceed on the hinted model (carry by default,
side-store volatile). Next spikes: H2 (partition ordering, single span) and
H6 (reference bindings), then feed volatility automatically in adaptive
mode.

## Design review before H2

[research/design-review-pre-h2.md](research/design-review-pre-h2.md).
Key evidence: iterating one system over K partitions is free above ~1K
entities per partition and 1.3–3.8× slower below ~200
(`sub0ecs_spans_bench`). So single-span ordering is not worth a PQ-tree;
controlling partition granularity is. The review ranks composition options
ahead of references and replicas, re-scopes H2 into H2a (granularity), H2b
(system tree) and H6′ (parent/child runs), and lists findings F1–F9 to carry
into implementation.

## H7 results: system fusion

Design and prototype: [research/fusion.md](research/fusion.md). Numbers:
[results/h7-fusion-linux-gcc13.md](../bench/results/h7-fusion-linux-gcc13.md).

**Verdict: fusion works and is worth designing in now.** Three small systems
that share Position/Velocity (the Update2 kernel split into Integrate,
Forces and Wrap) plus one unrelated system run **3.5–5.3× faster fused than
as sequential passes**, at every size from 1K to 1M. They match or beat the
hand-merged kernel, and the output is bit-identical to sequential
execution. Writing many small systems therefore costs nothing once fused.

Two constraints came out of the prototype:

1. **Only fuse systems that share data.** Fusing Frame3's three systems,
   which touch disjoint columns, is 0.64–0.86× (slower). Planner rule: fuse
   connected components of the "shares a column" graph; an unrelated
   member inside a sharing group is neutral.
2. **Fusion depends on full inlining.** Without `flatten`, GCC stopped
   inlining in the large benchmark translation unit and fused ran at
   sequential speed. Production code needs forced inlining per compiler
   plus a benchmark guard that fused ≈ hand-merged.

## Skirmish RTS testbed: first results

Testbed: [testbed/skirmish/](../bench/skirmish/README.md). Numbers:
[results/skirmish-linux-gcc13.md](../bench/results/skirmish-linux-gcc13.md).
All seven storage/execution variants play the bit-identical game.

| ms per tick | 1K units | 10K units | 50K units |
|---|---:|---:|---:|
| SparseSet (reference) | 0.144 | 2.40 | 14.4 |
| Archetype | 1.03× | 1.08× | 1.18× |
| QPartHinted | 1.04× | 1.06× | 1.16× |
| **QPartHinted + fused movement** | **1.10×** | 1.07× | **1.23×** |
| StaticBitmask | 1.02× | 1.09× | 1.25× |

**The real game compresses the micro-benchmark gaps.** Update2 differs 6×
between designs, but a whole RTS tick differs only 1.0–1.25×. At 50K units
about 80% of the tick is **spatial neighbour work**: Separation inside
movement (5.4–6.7 ms) and target acquisition (≈4.2 ms). Both are bound by
the grid scans, not by the storage layout. The storage choice still
matters at the edges:
- combat, projectiles, arrive and death are 1.5–4× cheaper on table-based
  designs than on SparseSet;
- fusion trims movement by ~9%.

The big remaining lever, though, is spatial indexing: the grid build and
neighbour queries. That argues for making spatial queries a first-class
v2 citizen (a grid/partition index maintained by the store, with neighbour
iteration that can itself be fused and offloaded), and it is exactly the
kind of priority a real testbed exposes and micro-benchmarks hide.

## Fusion extension points (planners × executors)

[research/fusion-extension-points.md](research/fusion-extension-points.md).
Fusion is pluggable along two axes: `constexpr` planners and runtime
auto-tuning, and executors (inline, tiled, thread pool, emulated offload).

- Every combination is bit-identical to sequential execution, including an
  auto-tuner that switches plans mid-run and the real game with four
  runners.
- `ShareColumns` fuses FusionFrame (3.6×) and declines Frame3 (where
  `AlwaysFuse` costs 17–20%). `AutoTuned` lands on or near the best plan
  everywhere.
- Fusion + 4 threads = 6.0× at 1M rows.
- Offload copies back only the columns a group writes.

## H8 threading

[research/threading.md](research/threading.md). The real game on 4 cores
stays bit-identical at 2 and 4 threads (ThreadSanitizer clean).

- **First cut:** lock-step fork-join per system with a sleeping pool.
  0.84× at 10K units, 1.50× at 50K.
- **Improved:** spin-then-park pool, grain control, and fused chains run
  chunk by chunk across all partitions. **1.45–1.77× at 10K, 2.05–2.18× at 50K** (two runs; VM variance):
  movement 3.50×, target acquisition 3.16×.
- **Remaining limit:** the serial fraction (grid build, commit, small
  systems) of ~2.4 of 5.5 ms.
- **Affinity:** static "owner computes" scheduling lost to dynamic
  balancing (chunk costs vary by an order of magnitude).

Recommendation: lock-step phases with independent chunk-level work inside
them. Then:
- batch small systems into one task-parallel dispatch;
- make the commit and spatial-index build parallel;
- pipeline read-only consumers across frames from snapshots.

Reject free-running threads (they break determinism).

## H9 dynamic system lifetimes

[research/dynamic-systems.md](research/dynamic-systems.md). Systems can be
added at runtime.
- **No relayout** in the hinted layout when the new system uses components
  that are already columns (O(#partitions) matching only).
- **Promotion** when it needs a side-stored component. The new system runs
  immediately in a **degraded** mode (fast path plus a sparse join over
  unmigrated holders) while `migrateStep(budget)` restructures boundedly
  per frame, then **flips to the full path**.

Results are bit-identical for every budget (0 / 100 / 1000 / stall), with
structural churn during migration and each holder visited exactly once. So
the budget can be time-driven without affecting determinism.

At 1M entities (500K promoted): a stall is one **149 ms** frame, while
16 384 entities/frame caps frames at **~16 ms** and reaches the full path
in 30 frames. The degraded path costs 14–22× the full path, so it is a
transition mechanism, not a steady state. Next: time-budgeted and bulk
row-slice migration. (After the optimisation pass: stall 92 ms, 16 384/frame
worst frame 11 ms, ~170 ns per promoted entity.)

## Benchmark harness

[BENCHMARKING.md](../bench/BENCHMARKING.md) codifies how to reproduce every result
above on other hardware:
- CMake presets (`bench-native` / `bench-portable` / `sanitize`);
- a suite catalogue with quick / standard / reference profiles;
- `run.py`: builds, fingerprints the machine (CPU, caches, SIMD, governor,
  turbo, SMT, ASLR, load, compiler flags, git state), runs the suites with
  pinning or no-ASLR options, and writes a self-describing result
  directory;
- `compare.py`: A/B verdicts that account for measured noise and list
  environment differences first.

Sizes and thread ladders scale automatically to the machine
(`BENCH_SIZES`, `SKIRMISH_UPT`, `SKIRMISH_THREADS`).

## Code review & optimisation pass

Numbers: [results/opt-pass-linux-gcc13.md](../bench/results/opt-pass-linux-gcc13.md).
The scope was the recommended design ([designs/query_partition.hpp](../include/sub0ecs/store/world.hpp)),
the executors and the H9 paths.

**Correctness: no defects found.** Checked explicitly:
- `destroy` scrubs side bits, non-fragmenting pools and migrating pools
  correctly in both carry modes.
- H9 promotion is all-or-nothing per entity; `find`, `remove` and
  `compatJoin` agree on where an unmigrated value lives.
- `Parallel` dispatch: the release/acquire pairs on `gen_`/`remaining_` plus
  the empty lock handshake rule out lost wake-ups.

All suites pass (conformance, fusion, dynamic, skirmish incl. lock-step x2/x4)
and are clean under ASan/UBSan and TSan.

**Optimisations** (all in `query_partition.hpp`):

| Change | Why | Effect (vs Archetype as drift control) |
|---|---|---|
| Raw 64-byte-aligned column buffers, owned by the partition, geometric growth, no zero-fill | `vector<byte>::resize` per `pushRow`/`swapRemove` did bookkeeping and zero-fill on every structural move | TagChurn **1.46–1.91×**, DestroyCreate **1.14–1.23×**, Create QPartHinted 0.83→**1.12×** Arch at 1M |
| Per-partition column base pointer per type | `find`/`each`/fusion bind with one load instead of `columnOf → columns[] → data` | Simpler code; not measured in isolation (RandomGet did not move until the reorder below) |
| `find()`: column lookup before the `has` test (columns ⊆ has, so it is safe) | The early `has` branch cost ~20% on a latency-bound loop | RandomGet 0.67–0.75× → **0.86–1.12×** Arch |
| Add/remove transition edge cache, keyed by destination column mask | Hash lookup per structural move. The key includes the mask because carried components make the destination depend on more than (partition, type) | Folded into the TagChurn and H9 gains |
| Same edge cache for H9 promotion | Hash lookup per promoted entity | Migration 100–113 ms → **72–84 ms** at 1M (~170 ns/entity); stall worst frame 109→92 ms |

Unchanged, within noise: Update2, Iter1, and AddRemove of an unqueried
component (a pure sparse-set op; code untouched, 0.9× in one run tracked
Archetype's own 0.9× drift).

**Carried forward (not done):**
- Reclaiming empty partitions (F4). It must also invalidate edge caches,
  which hold partition indices.
- Bulk row-slice promotion. It would sort pending holders by source
  partition so the moves stream instead of chasing random rows (migration
  is now latency-bound).
- Batching TagChurn moves at commit (§4.6).

## MSVC validation

MSVC 19.51 (Visual Studio 18), Release, `/W4 /permissive-`, `/arch:AVX2`, Ninja.

- **Builds clean:** every target compiles with zero warnings once `/bigobj` is set.
  The benchmark TUs instantiate every design × scenario and exceed COFF's section
  limit without it. `_CRT_SECURE_NO_WARNINGS` covers `getenv`/`fopen`.
- **All four suites pass:** storage conformance (bit-identical checksums across all
  designs, including v1), fusion (planners × executors), H9 dynamic, and Skirmish
  lockstep.
- **Fusion inlining on MSVC:** `[[msvc::flatten]]` beside GCC's `flatten` was tried and
  withdrawn (2026-10-01). The paired fusion benchmarks showed no consistent gain (MSVC
  fuses at 1.1-1.8x over sequential either way), while it cost ~4 GB and ~60 s for
  every file that includes the Skirmish systems.
- **Compiler memory:** the fixed-capacity comparator's component arrays were members
  initialised at compile time; MSVC needed ~48 KB per slot (50 GB for the 1M world,
  105 GB with Skirmish's), which broke CI. They are now allocated once at
  construction; the heaviest benchmark file compiles in 0.5 GB / 11 s.
- The harness runs on Windows (see BENCHMARKING.md "Windows / MSVC").

## Promotion to the library

The query-partition store became `sub0ecs::store` (header-only, `include/sub0ecs/`).
Writing library-grade tests surfaced defects that the benchmark-driven conformance
suite never exercised, because it never repeats an operation on the same entity:

| Defect | Effect | Now |
|---|---|---|
| `add` of a component already held in side storage | A second dense entry, orphaning the first; the pool corrupts on later swap-removes | Overwrites in place |
| `remove` of an absent component | UB (indexing with the null slot), or a null-pool dereference for a never-stored type | No-op |
| `destroy` / `add` / `remove` on a stale handle | `destroy` twice released the slot twice, so two live entities later shared it | No-op |
| Edge cache sentinel was the all-ones mask | An entity holding 64 column types matched the "empty" sentinel and was moved to the empty partition, dropping out of every query | Separate `kNoEdge` index |
| Component type ids were process-wide | The 64-type limit covered every type in the program, not one World | Numbered per World type |

Each fix has a test that fails without it. That was verified for the side-pool and
double-destroy fixes by reintroducing the old code: the model-based churn test
reported 43 mismatches, and the stale-handle test crashed.
