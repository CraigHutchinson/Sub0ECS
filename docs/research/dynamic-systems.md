# Systems added and removed at runtime

> Design note: the reasoning behind a part of the library. Measurements are kept in
> [FINDINGS.md](../FINDINGS.md), open work in [BACKLOG.md](../BACKLOG.md).

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
  `runFusedOnMasked` in [store/world.hpp](../../include/sub0ecs/store/world.hpp).
- Tests: [tests/test_dynamic.cpp](../../tests/test_dynamic.cpp).
- Timeline benchmark: [bench/dynamic_timeline.cpp](../../bench/dynamic_timeline.cpp).

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
reason the hinted layout is the default ([FINDINGS.md](../FINDINGS.md), section 1).

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

## 3. What the timeline benchmark shows

`sub0ecs_dynamic_timeline` adds a system over `<Position, Velocity, Frozen>` at
frame 0, with `Frozen` in side storage on half the entities, and records every
frame for a range of migration budgets. Current figures are in
[FINDINGS.md](../FINDINGS.md), section 5. The shape of the result is what the
design relies on:

- **Incremental migration bounds the hitch.** Migrating everything at once is one
  long frame; a per-frame budget caps the worst frame at a fraction of it and
  finishes within a bounded number of frames.
- **The degraded path is a real cost**, an order of magnitude above the full
  path, because it is a sparse join with random access to records. That is fine
  for a transition but not as a steady state. Budgets must be large enough to
  finish in a bounded time, or the system should be declared ahead of the level
  where it is needed.
- **Migration is one general row move per entity.** Bulk migration (moving runs
  of rows from the same source partition with column slice copies) is the obvious
  next optimisation.

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

| Goal |
|---|
| Time-budgeted `migrateFor(µs)` driven by a frame-time governor; bulk row-slice migration per source partition |
| Dynamic systems inside fused groups (re-plan on set change, AutoTuner re-measure after the flip) |
| Skirmish scenario: a "phase 2" system paged in mid-game (e.g. supply-line telemetry over `Carrying`) playing the identical game across budgets |
| Demotion at stall points; system set profiles for static/embedded mode |
