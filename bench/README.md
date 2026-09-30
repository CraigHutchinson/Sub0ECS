# Comparison benchmarks

Keeps the v2 store honest. The v2 store ([`sub0ecs::store`](../include/sub0ecs/store/world.hpp),
*QueryPart* / *QPartHinted* below) is driven through one workload and adapter surface
alongside the alternatives it was chosen over, including v1 itself, unmodified. The
storage model is the only variable.

The comparators are **reference implementations, not library code**:
[`designs/`](designs/) (archetype, sparse set, sorted SoA, static bitmask) and
[`baselines/v1/`](baselines/v1/) (v1, frozen byte-identical). Findings and the
decision record are in [FINDINGS.md](../docs/FINDINGS.md); raw numbers in
[results/](results/).

## Candidates

| Id | Design | Storage | Query | add/remove component | destroy | Registry |
|---|---|---|---|---|---|---|
| **V1** | SubzeroECS v1 (unmodified, [`v1_adapter.hpp`](designs/v1_adapter.hpp)) | per-type sorted id vector + parallel data vector | N-way galloping sorted intersection, `.at()` access | add only (O(n) if not appended) | ✗ | static per-type table, max 32 worlds |
| **C** | Sorted SoA, "v1-evolved" ([`sorted_soa.hpp`](designs/sorted_soa.hpp)) | same as v1 | smallest-set leader + linear-step/gallop followers, raw pointers | staged, merged in one O(n) pass at `commit()` | staged | per-world runtime |
| **A** | Sparse set, EnTT-style ([`sparse_set.hpp`](designs/sparse_set.hpp)) | per-type sparse index + packed dense entities/data | smallest-pool leader, O(1) `contains` on others | O(1) swap-and-pop | O(#pools) | per-world runtime |
| **B** | Archetype tables, flecs/Bevy-style ([`archetype.hpp`](designs/archetype.hpp)) | tables per exact signature, SoA columns | cached archetype match, tight typed-column loop | row move between tables (memcpy), cached edges | swap-remove row | runtime, 64-bit signature |
| **D** | Static bitmask, fixed capacity ([`static_bitmask.hpp`](designs/static_bitmask.hpp)) | compile-time component list, `Capacity` dense arrays indexed by slot, 32-bit signature per slot | linear scan of signatures | set/clear bit O(1) | O(1) | compile-time |
| RawSoA | hand-written `vector<Position>` + `vector<Velocity>` loop | — | — | — | — | roofline for Update2 only |

All generational designs (A, B, D) share the 32-bit handle in
[`common/entity.hpp`](../include/sub0ecs/entity.hpp) (24-bit slot + 8-bit version) so stale
handles are rejected. C keeps v1's monotonic, never-recycled ids.

## Workloads

Entity mix is identical to `benchmarks/update_patterns`: **Coherent** = all
Small (Position, Velocity); **Fragmented** = rotating Small / Medium (+Health,
Rotation, Scale) / Large (+Color, Team, Flags). Kernels are copied verbatim
from v1's benchmark ([`common/components.hpp`](common/components.hpp)).

| Scenario | What it measures | Pattern(s) |
|---|---|---|
| `Create` | populate N entities (manual timer, excludes world construction); also reports heap bytes/entity and allocations/entity | both |
| `Iter1` | single-component read/write loop — pure storage streaming cost | both |
| `Update2` | Position+Velocity physics — **v1's headline benchmark** | both |
| `Frame3` | three systems per frame (Physics, Health+Rotation, Scale+Color) | Fragmented |
| `SparseQuery` | Position+Velocity+Tag where Tag is on 1% of entities — does the rare component drive iteration? | Fragmented |
| `RandomGet` | `find<Velocity>(e)` over a shuffled handle list — random access | Fragmented |
| `AddRemove` | add then remove a component on 10% of entities | Fragmented |
| `DestroyCreate` | destroy 10% of entities and create 10% new | Fragmented |

`items_per_second` is always *world entities per second* (N per iteration),
so rows are comparable across designs within a scenario.

### Fairness guards

- **Conformance first.** [`tests/test_design_conformance.cpp`](../tests/test_design_conformance.cpp) runs
  the same sequence (systems, tagging, churn, destroy/create, stale-handle
  checks) on every design and requires *bit-identical* checksums against the
  sparse-set reference. It also runs clean under ASan/UBSan
  (`cmake --preset sanitize`).
- **Denormals flushed (FTZ/DAZ).** The kernel damps velocity each step, so
  long benchmark runs drift into denormal floats and end up measuring the FPU
  microcode path (10–30× slower, varying with iteration count). v1's own
  benchmark avoids this only implicitly through `-ffast-math`. The benchmarks set
  FTZ/DAZ explicitly instead, and builds without `-ffast-math` so conformance
  stays bit-exact.
- **Same flags for all**: `-O3` (GCC/Clang) or `/O2` (MSVC), plus `-march=native` /
  `/arch:AVX2` with `SUB0ECS_NATIVE=ON` (the `bench-native` preset).
- v1 is compiled from [`baselines/v1/`](baselines/v1/) unmodified; its warnings are
  silenced as SYSTEM headers only.
  v1 does not compile with Clang (it calls members of an incomplete type), so
  Clang builds compare every design except v1 (`SUB0ECS_HAS_V1_BASELINE`).

## Benchmark harness

For reproducible runs (and dedicated hardware) use the harness rather than
invoking binaries by hand: see [BENCHMARKING.md](BENCHMARKING.md)
(`tools/run.py`, `tools/compare.py`, CMake presets).

## Build & run

From the repository root (on Windows, in a VS developer prompt):

```bash
cmake --preset bench-native
cmake --build --preset bench-native
ctest --preset default                                   # or: build/bench-native/tests/sub0ecs_tests
build/bench-native/bench/sub0ecs_bench --benchmark_filter='Update2/.*/(V1|QPartHinted)'
python3 bench/tools/summarize.py <run>.json > <run>.md
```

Handy filters: `--benchmark_filter='Update2/.*/(V1|Archetype)'`,
`BENCH_SIZES=small` (N=1000 only, fast smoke run).

## Adding a candidate

Implement the adapter surface (see the top of
[`common/scenarios.hpp`](common/scenarios.hpp)): `Entity`, `kName`,
`kSupportsRemove`, `kSupportsDestroy`, `reserve`, `create(Cs...)`,
`each<Cs...>(f)`, `find<C>(e)`, `add<C>(e, c)`, `remove<C>(e)`, `destroy(e)`,
`commit()`. Then add it to `registerAll()` in [`scenarios_bench.cpp`](scenarios_bench.cpp)
and to the conformance test in [`tests/test_design_conformance.cpp`](../tests/test_design_conformance.cpp).
