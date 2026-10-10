# Comparison benchmarks

Start with the [performance guide](../docs/optimization/README.md) for workload selection,
reproduction recipes, current optimization results and their limits.

These keep the library honest. The store ([`sub0ecs::store`](../include/sub0ecs/store/world.hpp),
*QueryPart* / *QPartHinted* below) runs one fixed workload beside reference
implementations, and every figure the project quotes is a ratio to one of them,
measured in the same run.

The references are **not library code**. They live in [`designs/`](designs/).
Findings are in [FINDINGS.md](../docs/FINDINGS.md); curated runs in
[results/reference/](results/reference/README.md).

## What the store is measured against

**The bars: no ECS at all** ([`handwritten.hpp`](designs/handwritten.hpp)). They
know the workload in advance and support nothing else: no queries, no adding or
removing components, no destroying entities.

| Id | What it is | Role |
|---|---|---|
| **HandWritten** | One array per component per entity shape, and a plain loop calling the shared kernel | What a programmer writes first. The paired baseline of the iteration and lookup scenarios: a ratio of 1.0 means the library costs nothing over it |
| **HandTuned** | One array per field, branch-free kernels, explicit AVX2 where the build targets it | The ceiling: what the hardware allows for this arithmetic. The distance from the store to this is what layout and SIMD could still buy |

**What an ECS replaces.**

| Id | What it is | Scope |
|---|---|---|
| **OOP** | Class hierarchy ([`oop.hpp`](designs/oop.hpp)): one heap object per entity, Small ← Medium ← Large, one virtual call per object per update | Create, Iter1, Update2, Frame3, RandomGet. A class is fixed at creation, so no queries or structural change |
| **NaiveObjects** | Game objects owning a list of separately allocated components ([`naive_components.hpp`](designs/naive_components.hpp)); a system visits every object and looks its components up | Every scenario, up to 1M entities |

**The store's own way of working.** Besides one system per scenario, the store
also runs the same work as several small single-purpose systems, which is how the
library is meant to be used. These are not like-for-like with the bars (the bars
have one hand-merged loop), but they compute bit-identical results and sit in the
same groups, so the cost of small systems and what fusion recovers are both
visible against hand-written code.

| Id | What it is |
|---|---|
| **QPartHinted3Seq** / **QPartHinted3Fused** | Update2 as three systems (Integrate, Forces, Wrap): one pass each, or fused into one pass |
| **QPartHinted5Seq** / **QPartHinted5Fused** | Frame3 as five systems (those three, RotHealth, Pulse): one pass each, or fused by the `ShareColumns` plan |

**Other storage designs.** Reference implementations of the established ECS
models, driven through the same adapter as the store.

| Id | Design | Storage | Query | Add/remove component |
|---|---|---|---|---|
| **SparseSet** | Sparse set per component, EnTT-style ([`sparse_set.hpp`](designs/sparse_set.hpp)) | sparse index + packed dense entities and data | smallest pool leads, O(1) membership test on the others | O(1) swap-and-pop |
| **Archetype** | Archetype tables, flecs/Bevy-style ([`archetype.hpp`](designs/archetype.hpp)) | one table per exact component set, a column per component | cached table match, typed-column loop | row moves between tables |
| **SortedSoA** | Sorted id vectors ([`sorted_soa.hpp`](designs/sorted_soa.hpp)) | per component a sorted id vector + parallel data | smallest set leads, the others gallop | staged, merged at `commit()` |
| **StaticBitmask** | Fixed capacity ([`static_bitmask.hpp`](designs/static_bitmask.hpp)) | compile-time component list, dense arrays by slot, a signature per slot | linear scan of signatures | set or clear a bit |

These are our own implementations of each model, not the libraries they are named
after. Benchmarks against EnTT and flecs themselves are on the
[backlog](../docs/BACKLOG.md).

The designs with recycled ids share the 32-bit generational handle in
[`sub0ecs/entity.hpp`](../include/sub0ecs/entity.hpp) (24-bit slot, 8-bit version), so
stale handles are rejected. SortedSoA uses monotonic ids that are never recycled.

## Workload

**Coherent** = every entity is Small (Position, Velocity). **Fragmented** =
rotating Small / Medium (+Health, Rotation, Scale) / Large (+Color, Team, Flags).
The kernels are fixed ([`common/components.hpp`](common/components.hpp)).

| Scenario | What it measures | Pattern(s) |
|---|---|---|
| `Iter1` | one field of one component: the cost of streaming storage | both |
| `Update2` | Position + Velocity physics with wrap-around | both |
| `Frame3` | three systems per frame (physics, health + rotation, scale + colour) | Fragmented |
| `RandomGet` | `find<Velocity>(e)` over a shuffled handle list | Fragmented |
| `SparseQuery` | Position + Velocity + Tag where 1% of entities have Tag: does the rare component drive iteration? | Fragmented |
| `AddRemove` | add then remove a component no system queries, on 10% of entities | Fragmented |
| `TagChurn` | add then remove a queried component on 10% of entities | Fragmented |
| `DestroyCreate` | destroy 10% of entities and create as many | Fragmented |
| `Create` | populate N entities; also heap bytes and allocations per entity (glibc) | both |

The first four have hand-written bars. The rest are about flexibility, which the
bars do not have, so their baseline is SparseSet.

`items_per_second` is always *world entities per second* (N per pass), so rows
compare across designs within a scenario.

## Fairness guards

- **Conformance first.** [`tests/test_design_conformance.cpp`](../tests/test_design_conformance.cpp)
  runs the same sequence on every design and requires *bit-identical* state against
  the sparse-set reference. That includes the bars: HandTuned's SIMD kernels must
  give exactly the results of the scalar ones, or they would be measuring different
  arithmetic.
- **No fused multiply-add** (`-ffp-contract=off`; MSVC's default) and no
  `-ffast-math`, so every compiler and every design computes the same values.
- **Denormals flushed (FTZ/DAZ).** The kernel damps velocity each step, so long
  runs drift into denormal floats and would time the FPU's microcode path.
- **Same flags for all**: `-O3` (GCC/Clang) or `/O2` (MSVC), plus `-march=native` /
  `/arch:AVX2` with `SUB0ECS_NATIVE=ON` (the `bench-native` preset).
- **The same inlining hint** on every design's row loop, so MSVC's inliner does not
  favour one of them.
- **Name the machine and the compiler** with any figure. Ratios move between
  compilers; compare them on one machine, in rotated runs (BENCHMARKING.md).

## Running

Use the harness, not the binaries by hand: [BENCHMARKING.md](BENCHMARKING.md)
(`tools/run.py`, `tools/compare.py`, `tools/profile.py`, CMake presets).

```bash
cmake --preset bench-native
cmake --build --preset bench-native
ctest --preset default                                   # conformance
python3 bench/tools/run.py --profile standard --pin P    # results + summary.md
build/bench-native/bench/sub0ecs_bench --filter='^Update2/Fragmented/' --epochs=22
```

Binary options: `--filter=REGEX` (on case names; `--list` shows them), `--epochs=N`,
`--min-epoch-ms=X`, `--out=FILE`; environment: `BENCH_SIZES=small` (N = 1000 only).

## Adding a design

Implement the adapter surface (see the top of
[`common/scenarios.hpp`](common/scenarios.hpp)): `Entity`, `kName`,
`kSupportsRemove`, `kSupportsDestroy`, `reserve`, `create(Cs...)`,
`each<Cs...>(f)`, `find<C>(e)`, `add<C>(e, c)`, `remove<C>(e)`, `destroy(e)`,
`commit()`. Then add it to `registerAll()` in [`scenarios_bench.cpp`](scenarios_bench.cpp)
and to the conformance test.
