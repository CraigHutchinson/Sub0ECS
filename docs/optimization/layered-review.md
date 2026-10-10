# Layered optimization review — 10 October 2026

Source: Sub0ECS master `459a0d0f2eceeca97c0d10ce365f023a173a7548`, including
PR18's optional Pipeline adapter. This review works from architecture down to
row-loop code. The implementation in this increment removes single-partition
chunk materialization; it does not claim whole-application optimality.

## Performance contract and architecture

The target is optimized handwritten code doing the **same work**. Use two bars:
(1) an equivalent storage/kernel loop to isolate abstraction cost and (2) the
best admissible layout/algorithm to expose remaining opportunity. Preserve
numerical order, ownership, mutation/failure semantics and execution resources.
A parallel ECS result versus a sequential bar is thread scaling, not proof of
zero-cost abstraction. A shared scalar n-body kernel is an isolation control,
not the fastest achievable n-body implementation.

| Layer | Inspection and decision |
|---|---|
| Architecture | Retain query-signature partitions: declared queries iterate dense columns without per-entity membership checks. Keep carry/volatile policy available. Replacing this with a sparse join would sacrifice the central streaming advantage. |
| Integration | Keep the optional ECS-owned Pipeline bridge. It borrows scheduling resources; it does not own storage or introduce Pipeline into the C++20 umbrella. Exclusive dispatch/join, disjoint lane scratch and joined failure remain necessary. |
| Work decomposition | `eachParallel` already batches across partitions, but rebuilt one descriptor per chunk each call. A single partition has arithmetic boundaries and needs no descriptor storage. Implement this specialization without a new API or persistent invalidation cache. |
| Data layout | Component columns remain arrays of structs inside each component. Field-only work may still trail the existing field-split `HandTuned` bar. An opt-in field-layout/column-view design is the next substantial architectural experiment, requiring a proxy/reference compatibility decision. Do not claim loop tuning closes that gap. |
| Class/storage | `Partition` caches column bases and transition edges; growth updates bases. `find` retains generation validation and record/partition indirection. `SidePool` still grows its sparse table to the largest entity index; paged indexing trades memory for another lookup and needs its own workload. |
| Structural functions | `destroy` scans non-fragmenting types; `moveTo` copies columns and updates swapped-row records. Those costs need churn workloads, not fixed-population n-body evidence. Do not enlarge common-case per-entity metadata to optimize unmeasured churn. |
| Fusion/planning | Keep subset specialization and row-local fusion. `Access<Read<T>>` remains trusted rather than enforced; it cannot justify automatic concurrent system scheduling. `AutoTuner` keeps minima from five trials and never retunes; evolving workload evidence is needed before choosing adaptive scheduling. |
| Functions | Hoist typed column resolution out of the per-chunk callback on the single-partition path. Keep callback ordinals, lane values, tail coverage and the four-chunk inline threshold unchanged. Do not bypass pool dispatch merely because it reports one lane: that could change custom-pool behavior. |
| Lines/code generation | Compute count with `1 + (n - 1) / Rows` after checking zero, and length with `min(Rows, n - begin)`. Offset typed column pointers to the chunk start and use a zero-based inner loop. This avoids rounded-up overflow and descriptor loads. No `restrict`, unsafe alias promise, unchecked handle path, fast-math or global force-inline switch is introduced. |

## Implemented refinement

For one matching partition, resolve its columns once, calculate the number of
chunks, and pass an arithmetic range callback to the existing pool. Under four
chunks, stream rows directly. The single-partition path does not touch `chunks_`
and allocates no descriptor storage even on its first invocation. It still pays
whatever dispatch/exception bookkeeping the caller's pool requires.

Multiple matching partitions, including retained empty partitions, continue to
use the existing implementation. This deliberately avoids a structural epoch
cache or an extra full partition-count pass. The same mechanism in
`runFusedParallel` remains a separately measurable candidate: fusion has subset
resolution and generated-code costs not represented by this patch.

## Evidence and receiving

`bench/rows/bench.cpp` registers `rows` in the existing paired harness. It compares
handwritten direct column loops, sequential `each`, and `eachParallel` with
1/64/1024-row chunks at 64/4096/100000 rows, both one and two partitions.
Unsigned arithmetic avoids floating-point reassociation concerns; all arms
validate three untimed ticks against the handwritten checksum. Regression tests
check individual rows and lane assignment, threshold edges, tails, mutation into
a second partition and retention of an empty partition, in both storage modes.
The inline diagnostic pool isolates bookkeeping; it is not a parallel scheduler
performance test. N-body now also has matched handwritten parallel controls for both real pools,
with bitwise conformance against the ordered scalar oracle. MatchedNative and
MatchedPipeline groups compare identical worker counts and 64-row grains.

GCC 13.3 gprof attributes the selected 100000-row grain-1 control workload to the
inlined row-dispatch operation (63 samples). It does not distinguish individual
inlined instructions. GCC's vectorizer report and linked disassembly show the
separate descriptor-building loop and per-chunk traversal; the inner arithmetic
can already vectorize at wider grains. VTune/perf hardware sampling is unavailable
on this host. Instrumented profile time is excluded from speed measurements.

The [curated receipt](../../bench/results/reference/work-mode-layered/README.md) records paired A/B runs, compiler flags, binary hashes,
source header hashes, raw results, vectorizer diagnostics, profile and disassembly.
These shared-host measurements provide advisory evidence for this library specialization, not
Crucible's frame pacing or production receiving. Power/thermal conditions and
external host load are not controlled. Cross-platform CI and consumer attribution
remain separate gates.

## Next experiments, in order

1. Receive this dispatch specialization on GCC/Clang/MSVC and the real consumer.
   Recheck code size and compile cost as template instantiations multiply.
2. Extend the matched parallel n-body controls with a field-split/target-SIMD
   handwritten bar that retains ascending source reductions and joined completion.
   The new direct-range controls isolate ECS overhead; sharing the scalar physics
   kernel does not establish the best possible kernel or algorithm.
3. Measure field-split storage against `HandTuned` using useful bytes and generated
   SIMD, then choose an opt-in layout contract. Preserve the ordinary `T&` path.
4. Profile many tiny partitions before adding cached dispatch plans; account for
   invalidation on create/destroy/migration and retained empty partitions.
5. Receive declared access enforcement before hazard-derived parallel DAGs.
   Evaluate paged side indices and batched structural moves on churn workloads.

No new common library, second router, thread pool or duplicated Pipeline scheduler
is needed for these changes. Keep costs opt-in at the layer that owns them.

## Refinement record

Three implementations were measured, and the rejected variants are retained.
The first used 64-bit absolute row indices and removed descriptor construction,
but regressed some wider-grain loops. The second used the existing 32-bit row
index shape; it recovered the default grain but lost ground at 64 rows. The third
offsets the typed column pointers per chunk and runs a zero-based loop. This
exposes the contiguous range more directly to the optimizer without adding aliasing
assertions or new compiler-specific attributes. It is the retained implementation.

The third pass and final measurement series use a resumed host; each comparison reruns
both control and candidate on that same host. Never compare raw times across the
two host identities. Changes in unchanged `each` control timings also reveal
layout/noise sensitivity; they are not attributed to an `each` optimization.

## Final result

Five A/B pairs on GCC 13.3 put the 100K-row grain-1 diagnostic at 231.7 → 20.6 µs
(11.3×), and the 4096-row grain-64 case at 1117 → 706 ns (1.6×). Those retained
hot loops reach approximately handwritten throughput. The 64-row/default-grain
case pays a measured 4.8 ns setup regression; this is a scoped optimization, not
a promise of improvement for every call. Full ranges and all samples are in the
receipt. Matched n-body results establish no clear complete-tick improvement.

All 81 local tests (including exhaustive), 12 examples, focused standalone C++20
and fully instrumented focused Pipeline ASan/UBSan receiving pass. Cross-platform
CI and refreshed reference-host performance remain required before broad claims.
