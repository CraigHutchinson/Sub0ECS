# SubzeroECS v2 — design spike & performance baseline

Throwaway-quality code whose only job is to answer: **which storage/query model
should SubzeroECS v2 be built on?** Five candidate designs (including v1 itself,
unmodified) are driven through one identical workload and adapter surface, so
the storage model is the only variable.

> Status: spike. Nothing in `spike/` is public API. Findings and the
> recommendation are in [FINDINGS.md](FINDINGS.md), follow-up design research in
> [research/holographic-storage.md](research/holographic-storage.md); raw numbers in
> [results/](results/).

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
[`common/entity.hpp`](common/entity.hpp) (24-bit slot + 8-bit version) so stale
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

- **Conformance first.** [`test/conformance.cpp`](test/conformance.cpp) runs
  the same sequence (systems, tagging, churn, destroy/create, stale-handle
  checks) on every design and requires *bit-identical* checksums against the
  sparse-set reference. It also runs clean under ASan/UBSan
  (`-DSPIKE_SANITIZE=ON`).
- **Denormals flushed (FTZ/DAZ).** The kernel damps velocity each step, so
  long benchmark runs drift into denormal floats and end up measuring the FPU
  microcode path (10–30× slower, varying with iteration count). v1's own
  benchmark avoids this only implicitly through `-ffast-math`. The spike sets
  FTZ/DAZ explicitly instead, and builds without `-ffast-math` so conformance
  stays bit-exact.
- **Same flags for all**: `-O3 -march=native` (toggle `-DSPIKE_NATIVE=OFF`).
- v1 is compiled from `source/` unmodified; its warnings are silenced as
  SYSTEM headers only.

## Benchmark harness

For reproducible runs (and dedicated hardware) use the harness rather than
invoking binaries by hand: see [BENCHMARKING.md](BENCHMARKING.md)
(`tools/bench/run.py`, `tools/bench/compare.py`, CMake presets).

## Build & run

```bash
cmake -S spike/v2 -B build/spike -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/spike
ctest --test-dir build/spike --output-on-failure        # conformance
./build/spike/spike_bench --benchmark_repetitions=5 --benchmark_report_aggregates_only=true \
    --benchmark_out=spike/v2/results/<host>.json --benchmark_out_format=json
python3 spike/v2/results/summarize.py spike/v2/results/<host>.json > spike/v2/results/<host>.md
```

Handy filters: `--benchmark_filter='Update2/.*/(V1|Archetype)'`,
`SPIKE_SIZES=small` (N=1000 only, fast smoke run).

The spike is a standalone CMake project (it reuses `cmake/CPM.cmake` for
Google Benchmark) because the v1 top-level configure currently fails on Linux:
`find_package(fmt)` does not match the `FindFmt.cmake` module name on
case-sensitive filesystems.

## Adding a candidate

Implement the adapter surface (see the top of
[`common/scenarios.hpp`](common/scenarios.hpp)): `Entity`, `kName`,
`kSupportsRemove`, `kSupportsDestroy`, `reserve`, `create(Cs...)`,
`each<Cs...>(f)`, `find<C>(e)`, `add<C>(e, c)`, `remove<C>(e)`, `destroy(e)`,
`commit()`. Then add it to `registerAll()` in `bench/main.cpp` and to
`main()` in `test/conformance.cpp`.
