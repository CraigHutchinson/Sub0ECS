# SubzeroECS v2 spike — findings

Baseline: [results/baseline-linux-gcc13.md](results/baseline-linux-gcc13.md)
(raw: [JSON](results/baseline-linux-gcc13.json)). Host: 4 vCPU Xeon @ 2.1 GHz
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
   with published v1 numbers.
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

## H1 results: query-signature partitions ("automatic archetypes")

Design: [designs/query_partition.hpp](designs/query_partition.hpp). Numbers:
[results/h1-query-partition-linux-gcc13.md](results/h1-query-partition-linux-gcc13.md)
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

**Recommendation:** proceed on the hinted model (carry by default,
side-store volatile). Next spikes: H2 (partition ordering, single span) and
H6 (reference bindings), then feed volatility automatically in adaptive
mode.
