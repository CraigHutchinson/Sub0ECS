# SubzeroECS v2 — backlog to landing

Branch: `v2` (holding branch; `claude/sub0ecs-v2-design-spike-ndbal6` is its
history). Decided: the v2 core is the archetype-class **query-partition store**
(H1 hinted model, [designs/query_partition.hpp](designs/query_partition.hpp)). The
alternative designs and v1 stay **only as benchmark comparators**, so every claim
stays honest against them.

Status key: ☐ open · ◐ in progress · ☑ done.

## 1. Promote the spike into the library layout (blocking everything below)

Target: the Sub0Pipeline shape. Audit of every file outside `spike/`, 2026-09-30:
everything v1-specific outside `source/` is stale, and the top-level build no longer
configures.

| | Item |
|---|---|
| ☐ | `include/sub0ecs/`: header-only `INTERFACE` library `Sub0ECS::Sub0ECS`. `store.hpp` (from `designs/query_partition.hpp`, namespace `sub0ecs::store`, which maps 1:1 onto a later Sub0DataStore extraction, D2), `entity.hpp`, `query.hpp`, `fusion/{access,executors/*,planners/*}.hpp`. The spike-workload `World`/`HintedWorld` aliases move to a bench adapter |
| ☐ | `bench/`: other designs (`archetype`, `sparse_set`, `sorted_soa`, `static_bitmask`, `v1_adapter`), benchmark components/scenarios/systems (namespace `bench`), Skirmish testbed, `tools/` (run/compare/summarize/suites), `results/` |
| ☐ | `bench/baselines/v1/SubzeroECS/`: `source/SubzeroECS/` moved whole and **byte-identical** (the adapter needs transitive headers such as `Utility/StructOfVector.hpp`) |
| ☐ | `tests/`: storage conformance, fusion, dynamic, Skirmish lockstep; `docs/`: FINDINGS, research |
| ☐ | Root `CMakeLists.txt` + `CMakePresets.json` rewritten (default / bench-native / bench-portable / sanitize, plus MSVC). Keep `cmake/CPM.cmake` only |
| ☐ | Remove v1-only material: `test/` (v1 gtest), `samples/`, `benchmarks/` (superseded by `spike_bench` Update2: same kernels and entity mix, v1 run unmodified), `all/`, `.github/workflows/*` (all target deleted dirs), `cmake/{Version,tools,FindFmt,FindGTest,FindBenchmark}.cmake`, `.cmake-format`, `codecov.yaml`. `master` keeps v1 |
| ☐ | Keep: `LICENSE`, `NOTICE`, `COMMERCIAL-LICENSE.md`, `.github/ISSUE_TEMPLATE/*` (the licence-request process cross-references them) |
| ☐ | One `.github/workflows/ci.yml` (GCC, Clang, MSVC + ASan/UBSan job), triggered on `v2` |
| ☐ | `README.md` rewritten for v2; `CLAUDE.md` (build/test/API-change rules); `STYLE_GUIDE.md` (`CONTRIBUTING.md` already promises one); `.gitignore` additions (`build-*/`, `CMakeUserPresets.json`, `.cache/`) |
| ☐ | Reword provenance comments that cite `benchmarks/update_patterns` to "v1's published benchmark (see master)" |

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
| ◐ | Reference capture on the i9 275HX (MSVC, P-cores pinned + thread ladder to 24) |
| ☐ | Linux GCC/Clang re-run on the promoted layout (CI), plus ASan/UBSan/TSan |
| ☐ | Convert hand-rolled test mains to doctest (Sub0Pipeline convention) during promotion |
