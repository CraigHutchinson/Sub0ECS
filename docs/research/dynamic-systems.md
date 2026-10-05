# Dynamic system lifetimes (H9)

> **Design note from the exploration phase.** It records the reasoning behind a
> decision; its numbers were measured on an earlier host (a 4-vCPU cloud VM, GCC 13)
> and its result files are in the repository history, not the tree. Current,
> re-measured figures are in [FINDINGS.md](../FINDINGS.md).

Systems are not all known at startup. A game pages systems in and out as
it moves between levels and phases, or as new world regions and element
types stream in. Because the layout is *derived from the system set*
(research/holographic-storage.md), a new system can change what the
layout should be. This note defines how that change happens:
- **at stall-acceptable points** (level loads), by doing the whole relayout
  at once;
- **without a stall**, by a degraded compatibility path plus bounded
  incremental restructuring, flipping to the full path when done.

- Code: `addQuery` / `eachDyn` / `migrateStep` / `setQueryEnabled` /
  `runFusedOnMasked` in [designs/query_partition.hpp](../../include/sub0ecs/store/world.hpp).
- Tests: [test/dynamic_test.cpp](../../tests/test_dynamic.cpp).
- Timeline: [bench/dynamic_timeline.cpp](../../bench/dynamic_timeline.cpp).
- Results: [results/h9-dynamic-*.json](../../bench/results/).

## 1. When does a new system need a relayout?

In the recommended **hinted** layout, every non-volatile component is
already a dense column, so most new systems need **no relayout**: only an
O(partitions) update of which partitions the new query matches.

| New system requires… | Layout change | Cost |
|---|---|---|
| only components that are already columns | none | match partitions: O(#partitions) |
| a component currently in side storage (`Volatile`, e.g. `Carrying`, `Frozen`) | **promote** that component to a column for every holder | O(#holders) row moves |
| a component that doesn't exist yet | none (created as a column on first add) | — |
| *(removal of a system)* | none required; optional **demotion** back to side storage at a stall point | O(#holders) |

The pure-automatic layout (partitions keyed only by queries) would relayout
far more often, since any new query can split partitions. That is another
reason the hinted layout is the recommended default (FINDINGS § H1).

## 2. Lifecycle of a paged-in system

```
 addQuery<Cs...>()                        migrateStep(budget) each frame             side pools drained
 ───────────────►  DEGRADED ─────────────────────────────────────────────────────►  FULL
                   • fast path over partitions that already match                   • fast path only
                   • + sparse join over not-yet-migrated holders                     (identical to a system
                     (driven by the side pool of a pending component)                 declared at startup)
                   • each holder visited exactly once
        setQueryEnabled(false) → DISABLED (no layout change; re-enable is free)
```

Rules that keep this correct while the game keeps running:

1. **Promotion flips the component's policy first.** From `addQuery` on, new
   adds of the component go straight to columns. Existing holders stay in
   side storage until migrated.
2. **An entity migrates atomically.** All of its pending components move in
   one row move, so it is never half-migrated. That guarantees the sparse
   join (driven by one pending component's pool) plus the fast path cover
   each holder exactly once.
3. **Reads, removals and destruction understand "migrating".** `find`
   falls back to side storage for an unmigrated holder, `remove` removes
   from side storage, and `destroy` scrubs migrating pools.
4. **The budget changes performance, never results.** The test runs 40
   frames with structural churn during migration (the component is added
   and removed, entities are destroyed and created) under budgets of
   0 / 100 / 1000 / ∞. All match a SparseSet reference bit for bit, with
   identical per-frame visit counts. So the budget can be driven by
   **wall-clock time** (a frame-time governor) without breaking lock-step
   determinism.

**Enable/disable** is scheduler-level and layout-neutral:
- `setQueryEnabled` for dynamic systems;
- `runFusedOnMasked(exec, enabledMask, ...)` for members of a fused group,
  where a disabled member drops out of every partition's subset. Verified
  equal to running the remaining systems sequentially.

## 3. Evidence: stall vs incremental

A system over `<Position, Velocity, Frozen>` is added at frame 0; `Frozen`
is in side storage on 50% of entities (fragmented pattern). Median of 3
trials, 60 frames, 4-core VM.

**1M entities (500K holders promoted)**

| Budget (entities/frame) | Degraded system µs/frame | Full-path system µs/frame | Worst frame | Frames to full path | Total migration |
|---|---:|---:|---:|---:|---:|
| stall (all at once) | — | 438 | **149 ms** | 0 | 139 ms |
| 65 536 | 6 478 | 417 | 28.9 ms | 7 | 127 ms |
| 16 384 | 5 831 | 421 | **15.8 ms** | 30 | 118 ms |
| 4 096 | 7 509 | — | 13.1 ms | not within 60 | — |
| never | 9 551 | — | 12.3 ms | never | 0 |

**100K entities (50K holders)**

| Budget | Degraded | Full | Worst frame | Frames to full path |
|---|---:|---:|---:|---:|
| stall | — | 19 µs | 8.9 ms | 0 |
| 16 384 | 608 µs | 19 µs | 3.4 ms | 3 |
| 4 096 | 469 µs | 19 µs | 1.5 ms | 12 |
| never | 833 µs | — | 1.0 ms | never |

What the numbers say:

- **Incremental migration bounds the hitch.** At 1M, a stall costs one
  149 ms frame; 16 384 entities per frame caps frames at ~16 ms for about
  half a second of game time.
- **The degraded path is a real cost:** 14–22× the full path, because it is
  a sparse join with random access to records. That is fine for a
  transition but not as a steady state. Budgets must be large enough to
  finish in a bounded time, or the system should be declared ahead of the
  level where it is needed.
- **Migration throughput is ~250 ns per entity** (one general row move per
  entity). Bulk migration (moving runs of rows from the same source
  partition with column slice copies) should be several times faster, and
  it is the obvious next optimisation.

## 4. Design decisions

| # | Decision |
|---|---|
| D5 | System sets are **mutable at runtime**. The layout follows the set via promotion/demotion, never by rebuilding the world. |
| D6 | **Three transition modes:** *stall* (level loads, explicit key points), *incremental* (budgeted per frame; the default for streaming), and *degraded-only* (short-lived or rarely run systems). |
| D7 | The migration budget is **time-based** (µs per frame), owned by the scheduler. It runs as a low-priority Sub0Pipeline job in frame slack, with no determinism impact (§2, rule 4). |
| D8 | **Demotion** (returning a component to side storage when no remaining system needs it as a column) only at stall points. |
| D9 | **Fusion plans are recomputed** when the system set changes. A paged-in system joins fused groups only after it reaches the full path. Until then it runs as its own degraded pass, and the AutoTuner re-measures after the flip. |
| D10 | **Static mode (embedded):** no runtime promotion. Per-level system sets are compile-time "world profiles", and changing level is a stall-point relayout between profiles. |

## 5. Next steps

| Step | Goal |
|---|---|
| H9b | Time-budgeted `migrateFor(µs)` driven by a frame-time governor; bulk row-slice migration per source partition |
| H9c | Dynamic systems inside fused groups (re-plan on set change, AutoTuner re-measure after the flip) |
| H9d | Skirmish scenario: a "phase 2" system paged in mid-game (e.g. supply-line telemetry over `Carrying`) playing the identical game across budgets |
| H9e | Demotion at stall points; system set profiles for static/embedded mode |
