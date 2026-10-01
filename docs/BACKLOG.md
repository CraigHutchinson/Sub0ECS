# SubzeroECS v2 — backlog to landing

Branch: `v2` (holding branch; `claude/sub0ecs-v2-design-spike-ndbal6` is its
history). Decided: the v2 core is the archetype-class **query-partition store**
(H1 hinted model, [store/world.hpp](../include/sub0ecs/store/world.hpp)). The
alternative designs and v1 stay **only as benchmark comparators**, so every claim
stays honest against them.

Status key: ☐ open · ◐ in progress · ☑ done.

## 1. Promote the spike into the library layout ☑ (2026-09-30)

Done: header-only `Sub0ECS::Sub0ECS` in `include/sub0ecs/`, split into single-
responsibility headers (`store/`, `fusion/executors/`, `fusion/planners/`); comparators,
v1 (byte-identical) and the harness in `bench/`; doctest suites in `tests/`
(57 cases, 741,669 assertions, versus v1's 114 cases and 339 assertions); findings
and research in `docs/`; one `ci.yml`; README, CLAUDE.md and STYLE_GUIDE.md. The v1
tests, samples, benchmarks, workflows and build scaffolding were removed (`master`
keeps v1). Promoting the code surfaced and fixed three edge-case defects, and an
edge-cache bug that dropped entities holding 64 column types out of every query.
Component type indices are now per World type, not process-wide.

## 1b. Store capability configuration (component-type capacity) ☐

Today: `Mask = std::uint64_t`, `kMaxTypes = 64`, per World type; a 65th type terminates.
Bits are needed only by **fragmenting** components (queried ones, plus carried ones in
carry mode). Volatile components and, in pure mode, every unqueried component never
enter a mask. Options:

| Option | Mechanism | Cost | Notes |
|---|---|---|---|
| A | Config as a World template parameter: option tags (`World<Queries, Volatile<...>, MaxTypes<128>>`) or a traits struct | None at runtime | Per World type, like `Volatile`. Order-independent tags extend without breaking signatures |
| B | Mask type from the requested width: `uint32_t` / `uint64_t` / `WideMask<Words>` (array of words: and/or/test/iterate/hash) | Wider masks cost more per structural move and partition lookup; iteration is unaffected | `unsigned __int128` is not available on MSVC; `std::bitset` lacks bit iteration and a cheap hash |
| C | Tiered indices: queried types first (compile time known, so a `static_assert` on the query set), carried types next while bits last, overflow types fall back to side storage automatically | None when within budget | Removes the runtime hard limit: overflow degrades locality, never correctness. Runtime `addQuery` of a type without a bit fails cleanly |
| D | Non-fragmenting types get unbounded indices (growable side-pool table) | None | Pure-mode and Volatile types stop counting against the limit at all |
| E | Macro (`SUB0ECS_MAX_TYPES`) | None | Process-wide ODR hazard; rejected |

Recommended: **A + C + D**, with B for opt-in widths (default stays one 64-bit word).
The limit then applies only where it must, to components that decide the layout, and
it is checked at compile time for declared queries. Per-partition fixed tables sized
by `kMaxTypes` (`base`, `columnOf`, edge caches) need a sparse form before widths
beyond 64 are practical. Gate: benchmarks unchanged at the default width.

## 2. Examples: one per feature and alternative (required for landing)

Model: Sub0Pub's `examples/`. Each source opens with **Use when / Demonstrates /
Story / Keep in mind / Run**, is built and run by ctest (`Sub0ECS_Example_*`), and
returns a failure code if its checks fail. The examples are how each alternative
gets **documented, justified and pinned down**: what it is for, when to choose it,
and how to use it well.

| | Example | Shows |
|---|---|---|
| ☐ | `minimal_world` | Declare queries, create entities, run one system |
| ☐ | `hinted_partitions` | `Volatile<T>` for churn-heavy components vs carried columns (the H1 memory/churn trade) |
| ☐ | `structural_changes` | add/remove/destroy, deferred commit, stale-handle detection |
| ☐ | `random_access` | `find<T>(e)` and why column-first lookup is cheap |
| ☐ | `fusion_planners` | NeverFuse / AlwaysFuse / ShareColumns / DeviceAware on one schedule: when each wins (Frame3 vs FusionFrame) |
| ☐ | `auto_tuner` | Measured plan choice; switching plans is safe (bit-identical) |
| ☐ | `executors` | Inline / Tiled / Parallel / Offload: row-locality contract, written-column write-back |
| ☐ | `parallel_systems` | `eachParallel` / `runFusedParallel`, per-worker command buffers |
| ☐ | `dynamic_systems` | H9: paging a system in, degraded path, bounded migration budget |
| ☐ | `determinism` | Lockstep check across plans and executors (what `kBitExact` protects) |
| ☐ | `skirmish` | Pointer to the full RTS testbed as the "everything together" sample |

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
| ◐ | Reference capture on the Core Ultra 9 275HX (MSVC, P-cores pinned + thread ladder to 24) |
| ☑ | Promoted layout: `default`, `ci-msvc` and `sanitize` (MSVC ASan) presets pass; tests converted to doctest |
| ☑ | CI green on Linux GCC/Clang, macOS arm64 and ASan/UBSan; gate/exhaustive test tiers; nightly exhaustive workflow (active once on the default branch) |
| ☑ | Benchmarks on nanobench 4.6 (paired, interleaved design comparisons with CIs); tests on doctest 2.5.3; both fetched, matching Sub0Log |
| ☐ | A TSan job for the executors and the parallel store paths |
| ☐ | MSVC fusion inlining: a same-binary A/B (two instantiations, one with `[[msvc::forceinline_calls]]` on the kernel body only) instead of cross-build runs; `[[msvc::flatten]]` was withdrawn as too costly to compile for no consistent gain |
