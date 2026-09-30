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
