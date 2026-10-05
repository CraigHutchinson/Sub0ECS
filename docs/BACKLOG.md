# SubzeroECS — landing checklist and follow-up backlog

The selected core is the archetype-class **query-partition store** (H1 hinted
model, [store/world.hpp](../include/sub0ecs/store/world.hpp)). The historical v1
API is preserved by the `v1.0.0` tag and as a frozen benchmark comparator; the
alternative designs remain comparators so every claim stays honest against them.

Status key: ☐ open · ◐ in progress · ☑ done.

## 1. Promote the spike into the library layout ☑ (2026-09-30)

Done: header-only `Sub0ECS::Sub0ECS` in `include/sub0ecs/`, split into single-
responsibility headers (`store/`, `fusion/executors/`, `fusion/planners/`);
comparators, the frozen v1 baseline and harness in `bench/`; doctest suites in
`tests/` (57 cases, 741,669 assertions, versus v1's 114 cases and 339 assertions);
findings and research in `docs/`; one `ci.yml`; README, CLAUDE.md and STYLE_GUIDE.md. The
old v1 implementation, samples, benchmarks, workflows and build scaffolding are
preserved at the `v1.0.0` tag. Promoting the code surfaced and fixed three edge-case
defects, and an edge-cache bug that dropped entities holding 64 column types out
of every query.
Component type indices are now per World type, not process-wide.

## 1b. Component-type capacity ◐

**Done (tiered numbering):** there is no runtime limit on component types any more.
A World type numbers the types its queries and `Volatile` list name at compile time
(a `static_assert` if they exceed the 64 layout bits), then other types on first use
without limit. A type with a layout bit can be a dense column; a type beyond them
is kept in side storage, so overflow costs `find` locality, never correctness. In
pure mode unqueried types never need a bit, so they do not count at all.

**Open:**

| | Item |
|---|---|
| ☐ | Wider layout masks as a World option (`MaxTypes<128>`), for worlds with more than 64 queried or carried types: `WideMask<Words>` (array of words; `unsigned __int128` is not on MSVC, `std::bitset` lacks bit iteration and a cheap hash). Default stays one 64-bit word. Needs a sparse form of the per-partition tables sized by the bit count (`base`, `columnOf`, edge caches) |
| ☐ | Option tags as the configuration style (`World<Queries, Volatile<...>, MaxTypes<N>>`), order-independent so options can be added without breaking signatures |
| ☐ | A runtime `addQuery` naming a type without a layout bit terminates; make it report instead |
| ☐ | Let carried types be chosen deliberately (a `Carried<...>` hint) rather than by first use, for worlds near the limit |

Rejected: a macro (`SUB0ECS_MAX_TYPES`), a process-wide ODR hazard.

## 2. Examples: one per feature and alternative ☑ (2026-10-05)

Plan and conventions: [EXAMPLES.md](EXAMPLES.md). Each source opens with **Use
when / Demonstrates / Story / Keep in mind / Run**, is built and run by CTest
(`Sub0ECS_Example_*`), and returns a failure code if its checks fail. The examples
document, justify, and verify when each feature is useful.

| | Example | Shows |
|---|---|---|
| ☑ | `minimal_world` | Declare queries, create entities, run one system |
| ☑ | `hinted_partitions` | `Volatile<T>` for churn-heavy components vs carried columns (the H1 memory/churn trade) |
| ☑ | `structural_changes` | add/remove/destroy, deferred commit, stale-handle detection |
| ☑ | `random_access` | `find<T>(e)` and why column-first lookup is cheap |
| ☑ | `fusion_planners` | NeverFuse / AlwaysFuse / ShareColumns / DeviceAware on one schedule: when each wins (Frame3 vs FusionFrame) |
| ☑ | `auto_tuner` | Measured plan choice; switching plans is safe (bit-identical) |
| ☑ | `executors` | Inline / Tiled / Parallel / Offload: row-locality contract, written-column write-back |
| ☑ | `parallel_systems` | `eachParallel` / `runFusedParallel`, per-worker command buffers |
| ☑ | `dynamic_systems` | H9: paging a system in, degraded path, bounded migration budget |
| ☑ | `determinism` | Lockstep check across plans and executors (what `kBitExact` protects) |
| ☑ | `skirmish` | Pointer to the full RTS testbed as the "everything together" sample |
| ☑ | `rocket` | The classic first game loop: movement and rendering systems over different components (clean-room successor to v1's sample) |

All are built and run by CTest; CI verifies them on GCC, Clang, MSVC, macOS and under sanitizers.
v1's other sample, the SFML `balls_simulation` (ECS vs SoA vs AoS vs OOP, interactive), is not
rewritten: the Skirmish testbed and `sub0ecs_skirmish_demo` are its successor as the interactive
showcase, and the OOP/SoA comparison lives in the benchmarks (`OOP`, `RawSoA`).

## 3. Executors for accelerators (H10)

Design: [research/executor-async.md](research/executor-async.md).

| | Item |
|---|---|
| ☐ | `Completion` handle + optional `prepare / submit / retire` phases; `run()` stays as the synchronous composite (no change to existing executors) |
| ☐ | Store residency per partition column (`HostValid / DeviceValid / BothValid`) behind a compile-time `Coherence` policy (default `HostOnly`, zero cost); host access paths and structural changes sync first |
| ☐ | `HostAllocTraits` from executors → pluggable column allocator (pinned / DMA-capable) |
| ☐ | H10 spike: async emulated device (engine + copy-engine threads, latency model). Gates: bit-identical, bytes per frame with residency, overlap, zero cost when unused |
| ☐ | `kBitExact` capability plus a non-exact emulated device |
| ☐ | Later: CUDA executor behind an off-by-default option (device-compiled TU, `SUB0ECS_HD`); ESP32-P4 async-memcpy `Offload` |

## 4. Carried forward from FINDINGS

| | Item |
|---|---|
| ☐ | Reclaim empty partitions (F4); must also invalidate the edge caches |
| ☐ | Bulk row-slice promotion for H9 (sort pending holders by source partition) |
| ☐ | Batch TagChurn moves at commit (research §4.6) |
| ☐ | H2a partition granularity, H2b system tree, H6′ parent/child runs (design-review-pre-h2) |
| ☐ | H7b: auto-grouping from Access over a whole schedule |
| ☐ | Sub0Pipeline-backed `Parallel`; Sub0DataStore extraction once the store API settles |
| ☐ | ESP32-P4 run (H4): static capacity, code size, SRAM |

## 5. Validation

| | Item |
|---|---|
| ☑ | MSVC 19.51: all spike targets clean at `/W4 /permissive-`; four suites pass (2026-09-30) |
| ☑ | Benchmark harness runs on Windows (fingerprint, hybrid P/E map, `--pin P`) |
| ☑ | Reference capture on the Core Ultra 9 275HX (MSVC, P-cores pinned + thread ladder to 24): [results](../bench/results/reference/README.md), FINDINGS "Reference capture" |
| ☑ | Promoted layout: `default`, `ci-msvc` and `sanitize` (MSVC ASan) presets pass; tests converted to doctest |
| ☑ | CI green on Linux GCC/Clang, macOS arm64 and ASan/UBSan; gate/exhaustive test tiers; nightly exhaustive workflow |
| ☑ | Benchmarks on nanobench 4.6 (paired, interleaved design comparisons with CIs); tests on doctest 2.5.3; both fetched, matching Sub0Log |
| ☐ | A TSan job for the executors and the parallel store paths |
| ☑ | MSVC optimisation pass (2026-10-05): compile-time type indices, row-call inlining hint, fixed-size row copies; store Update2 at 0.94x the hand-written loop, RandomGet 1.6x faster. Same-machine comparison of MSVC, clang-cl and GCC in FINDINGS |
| ☐ | Re-measure fusion (fused vs sequential, planners) on one machine with all three compilers and contraction off: the 3.5-5.3x came from GCC with multiply-add fusing on another machine; FusionFrame here shows 0.95-1.38x at 100K and 1.30-1.72x at 1M |
| ☐ | Field-split columns (one array per field of a component): the one-field Iter1 loop is 3-4x faster on MSVC, clang-cl and GCC alike, and it is the only route to vector code on MSVC |
| ☐ | Side-pool add/remove on MSVC: AddRemove and TagChurn are 1.4-1.5x behind GCC, time spread over `SidePool::emplace`/`remove`/`find` (profile first) |
| ☐ | `bench/tools/profile.py`: wrap Linux `perf` for hosts without VTune; verify the `uarch` collection from an elevated prompt |
| ☐ | MSVC fusion inlining: a same-binary A/B (two instantiations, one with `[[msvc::forceinline_calls]]` on the kernel body only) instead of cross-build runs; `[[msvc::flatten]]` was withdrawn as too costly to compile for no consistent gain |
