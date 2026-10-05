# SubzeroECS design findings

This is the evidence record for selecting query-partition storage. The design
has since been implemented in the library; exploratory alternatives below
remain useful as historical benchmark comparisons, not undecided product choices.

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
   the v1 storage model into the current library.**

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

## Selected storage direction and follow-up proposals

The implemented library uses **query-partition storage**: queried components
form dense columns, and `Volatile<T>` components use side storage to avoid
partition churn. The original spike also proposed these follow-up directions;
they are not claims about already-shipped functionality:

- **Broader sparse-set side storage** selected per component, beyond the
  current world-level `Volatile<T>` hint, for tags and high-churn components.
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
   components (memcpy moves). The current component contract requires
   trivially copyable values; supporting non-trivial components would require
   type-erased move/destroy operations.
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
neighbour queries. That argues for making spatial queries a
first-class feature (a grid/partition index maintained by the store, with neighbour
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
- **Fusion inlining on MSVC:** `[[msvc::flatten]]` on the whole `fusedLoop` function,
  beside GCC's `flatten`, was tried and withdrawn (2026-10-01). The paired fusion benchmarks showed no consistent gain (MSVC
  fuses at 1.1-1.8x over sequential either way), while it cost ~4 GB and ~60 s for
  every file that includes the Skirmish systems.
- **Compiler memory:** the fixed-capacity comparator's component arrays were members
  initialised at compile time; MSVC needed ~48 KB per slot (50 GB for the 1M world,
  105 GB with Skirmish's), which broke CI. They are now allocated once at
  construction; the heaviest benchmark file compiles in 0.5 GB / 11 s.
- The harness runs on Windows (see BENCHMARKING.md "Windows / MSVC").

## Compilers on one machine

MSVC 19.51, clang-cl 22.1 and GCC 16.2 (MinGW-w64), all on the Core Ultra 9 275HX,
pinned to its P-cores, tuned for the host (`/arch:AVX2`, `-march=native`), with
floating-point contraction off so all three compute the same arithmetic. Three
rounds, the compilers rotated within each round; the table is the median of the
three (µs per pass, 100K entities, fragmented). 2026-10-05.

| Case | MSVC | clang-cl | GCC |
|---|---:|---:|---:|
| Iter1 / store | 20.6 | 14.7 | 15.9 |
| Update2 / hand-written loop | 149.4 | 165.9 | 115.0 |
| Update2 / store | 140.7 | 168.1 | 119.8 |
| Frame3 / store | 226.4 | 206.6 | 170.8 |
| RandomGet / store | 575.7 | 369.9 | 410.5 |
| AddRemove / store | 88.5 | 87.2 | 59.3 |
| TagChurn / store | 612.4 | 459.1 | 434.9 |

Absolute times moved by up to 30% between rounds on this laptop, so read a column
against the hand-written loop in the same run, not against another column to the
last digit. The store's Update2 is 0.94x, 1.01x and 1.01x the hand-written loop on
MSVC, clang-cl and GCC: the library adds nothing on any of them.

An earlier reading of the reference capture put MSVC 3-7x behind GCC and blamed
missing vectorisation. That compared this laptop with a cloud Xeon. On one machine
MSVC is 1.0-1.5x behind GCC, and GCC does not vectorise these kernels either.
Fusion is the same story. Fused against sequential on FusionFrame here (median of
the three rounds): 1.38x on MSVC, 1.26x on GCC and 0.95x on clang-cl at 100K;
1.72x, 1.54x and 1.30x at 1M. That is far from the 3.5-5.3x of the earlier GCC
runs, which were on another machine and had multiply-add fusing on. The planner
and executor results need re-measuring here before they are quoted again.

**What MSVC was losing, and the fixes (all three in the library, none in user code):**

| Cause | Found by | Fix | Effect |
|---|---|---|---|
| The type index was a function-local static. MSVC does not inline a function with one, so every `find`, `add` and `remove` made a call (and every partition of an iteration) | Assembly: `call TypeIndices::of` | Indices of queried and Volatile types are compile-time constants (the component-capacity change) | RandomGet 1.6x faster on MSVC; 35-45% on clang-cl and GCC too |
| The system callable was inlined into the row loop, but the kernel it calls was not: a call per row | Assembly: `call updatePosition` in the loop | `SUB0ECS_FLATTEN_CALLS` (`[[msvc::flatten]]`) on the row call statement of `each`, `eachDyn` and `eachParallel` | Update2 and Frame3 1.2-1.35x faster on MSVC; the store went from 1.24x the hand-written loop to 0.94x |
| A row move copied each column with `memcpy(stride)`, a library call | Reading `moveTo` after the profile showed add/remove time spread over the move path | `copyRow`: fixed-size copies for 4, 8, 12 and 16 bytes | AddRemove 5% faster on MSVC and clang-cl, 11% on GCC (probe) |

Measured as the store's paired ratio to v1 within one run, before against after
the first two fixes (MSVC, 100K): Update2 1.31-1.35x, Frame3 1.20-1.24x,
RandomGet 1.63-1.66x, AddRemove 1.15-1.51x. No case got slower (72 ratios: 34
faster, 38 unchanged). The comparator designs got the same row hint, so they are
not handicapped on MSVC. The statement-level hint did not lengthen the build; the
function-level one tried on `fusedLoop` (see "MSVC validation") is what cost 4 GB.

**Hints that were measured and not adopted** (`store / hand-written` was already
1.0, so these are about the kernel, not the library; ns per entity, Update2):

| Variant | MSVC | clang-cl | GCC |
|---|---:|---:|---:|
| As written (two structs, branches) | 0.99 | 1.29 | 0.81 |
| `__restrict` on the column pointers | 0.90 | 1.13 | 0.95 |
| "Iterations are independent" pragma | 1.02 | 0.73 | 0.95 |
| Branches rewritten as selects | 0.92 | 0.34 | 0.93 |
| One array per field, selects, `__restrict` | 0.93 | 0.21 | 1.01 |

- Only Clang turns any of them into vector code. MSVC reports the two-struct loop
  as a loop-carried dependence (reason 1200) even with `__restrict` and
  `#pragma loop(ivdep)`, and the select form as control flow (reason 1100).
- The independence pragma is not safe for `each`: a callable may carry state from
  row to row. It could be applied to fused systems, whose access is declared.
- A branch-free kernel is the user's choice; the benchmark kernel stays as written
  so the comparison with v1 holds.

**What is left on MSVC** is its code generator, not the library: Iter1 is 1.3-1.4x
behind because MSVC will not vectorise a one-field update of a two-field struct
(reason 1300, "too little computation"). One array per field makes that loop 3-4x
faster on all three compilers (0.044 ns per entity against 0.13-0.17), which is a
storage-layout change, not a hint; it is on the backlog. AddRemove and TagChurn
are 1.4-1.5x behind GCC with the time spread across the side-pool operations.

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
| Component type ids were process-wide | The 64-type limit covered every type in the program, not one World | Numbered per World type; since 2026-10-05 no runtime limit at all (types beyond the 64 layout bits are side-stored) |

Each fix has a test that fails without it. That was verified for the side-pool and
double-destroy fixes by reintroducing the old code: the model-based churn test
reported 43 mismatches, and the stale-handle test crashed.

## Reference capture: MSVC on dedicated hardware

Runs: [bench/results/reference/](../bench/results/reference/README.md). Core Ultra 9
275HX (8 P + 16 E cores, no SMT), MSVC 19.51 `/O2 /arch:AVX2`, nanobench harness,
`reference` profile (52 paired rounds, epochs of at least 5 ms), single-threaded
suites pinned to the P-cores. Ratios are paired within a group, with 95% intervals
in the run summaries.

**Storage scenarios at 100K fragmented entities (time per pass, µs):**

| Scenario | v1 | OOP | SparseSet | Archetype | **Store (hinted)** | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Update2 | 216.6 | 278.6 | 221.5 | 142.6 | **143.5** | 149.5 | 113.2 |
| Frame3 | 364.8 | | 340.9 | 199.6 | **200.7** | 303.7 | |
| SparseQuery | 40.3 | | 4.6 | 1.4 | **1.3** | 38.9 | |
| RandomGet | 6529 | | 426 | 566 | **588** | 207 | |
| Iter1 | 34.8 | | 18.2 | 18.3 | **18.2** | 27.7 | |
| AddRemove (unqueried component) | n/a | | 82.1 | 800.6 | **107.8** | 28.1 | |
| TagChurn (queried component) | n/a | | 76.7 | 834.6 | **567.1** | 31.1 | |
| DestroyCreate | n/a | | 335.4 | 350.7 | **340.6** | 121.7 | |
| Create | 7380 | 2732 | 8197 | 5238 | **4682** | 1076 | |

What the MSVC capture adds to the GCC results:

1. **The ranking holds; the magnitudes do not.** The store matches Archetype on
   iteration and beats v1 everywhere, as in the GCC results. But Update2 is 1.5x v1
   here against 6.9x there. Those GCC results came from another machine (a cloud
   Xeon), so the difference is not a compiler comparison; "Compilers on one
   machine" above is, and it puts MSVC 1.0-1.5x behind GCC with neither vectorising
   this kernel. In this capture the store ran Update2 at 79% of the hand-written
   loop's speed; that gap was the library's and is closed since (same section).
   Claims of "N x faster than v1" must name the machine and the compiler.
2. **The structural-change result is compiler-independent.** AddRemove of an
   unqueried component is 7.4x faster than Archetype (107.8 vs 800.6 µs) and close
   to SparseSet, as designed. RandomGet is 11x v1; SparseQuery 31x.
3. **OOP is the slowest design at iteration** (Update2 0.78x v1) and among the
   fastest at creation: one allocation per entity is cheap next to v1's sorted
   insert.
4. **Fusion pays less here, and the plan matters more.** FusionFrame fused vs
   sequential: 1.24x at 100K, 1.72x at 1M (the earlier GCC runs, on another machine
   and with GCC's default multiply-add fusing: 3.5-5.3x). `AlwaysFuse` is *0.61x*
   on the four-system frame (earlier GCC runs: 3.7x): fusing an unrelated system
   into the loop defeats MSVC's optimiser. `ShareColumns` gets 1.72x and the measuring
   `AutoTuned` 1.77x. This is the case for a measured planner: the best static
   policy differs by compiler.
5. **Skirmish (50K units per team):** the store, Archetype and StaticBitmask are
   within 2% of each other at 1.10-1.13x SparseSet; whole-game time is dominated by
   the spatial grid and neighbour queries, as noted above. The `Parallel` executor
   on the movement group alone gives 1.59x.
6. **Thread scaling on a hybrid CPU** (fused movement, 50K units per team, vs 1
   thread): 2 -> 1.74x, 4 -> 2.89x, 8 -> 3.55x, 16 -> 4.54x, 24 -> 4.38x. Past the 8
   P-cores the E-cores add little, and 24 threads is slower than 16. Static
   "owner computes" affinity is slower than dynamic claiming at every thread count
   here (3.52x vs 4.54x at 16): equal-sized blocks suit equal cores, and these are
   not. At 12.5K units scaling peaks at 8 threads (3.13x).
7. **Spans:** splitting 100K rows into up to 512 partitions is free (1.04-1.11x of
   one span); 4,096 costs 16% and 16,384 costs 25%. Consistent with the GCC
   finding that partition granularity only matters below a few hundred rows each.
8. **H9 dynamic systems (1M entities, 500K promoted):** a stall relayout is one
   40.7 ms frame; 16,384 entities per frame caps the worst frame at 7.8 ms and
   reaches the full path in 30 frames. Degraded iteration costs 10-20x the full
   path, so it remains a transition mechanism.
