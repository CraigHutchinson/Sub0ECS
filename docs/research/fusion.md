# System fusion: design and H7 prototype

System fusion (C4 in [design-review-pre-h2.md](design-review-pre-h2.md)),
brought forward by request. Several systems that visit the same entities run
as **one pass per partition**, applying each system to a row in schedule
order before moving to the next row. The data is loaded once and stays in
registers for the next system, instead of making one full memory pass per
system.

- Prototype: `runFused(...)` in [designs/query_partition.hpp](../designs/query_partition.hpp).
- Systems: [common/systems.hpp](../common/systems.hpp).
- Results: [results/h7-fusion-linux-gcc13.md](../results/h7-fusion-linux-gcc13.md).

## 1. Result

| FusionFrame (3 systems sharing Position/Velocity + 1 unrelated) | Sequential | Fused | vs hand-merged kernel |
|---|---:|---:|---:|
| Coherent, 100K | 195.9 µs | **44.0 µs (4.46×)** | 1.09× *faster* than hand-merged |
| Fragmented, 100K | 235.7 µs | **63.9 µs (3.69×)** | 1.02× |
| Coherent, 1M | 2.89 ms | **0.75 ms (3.84×)** | 0.88× |
| Fragmented, 1M | 3.16 ms | **0.90 ms (3.52×)** | 1.07× |
| Coherent, 1K | 1.77 µs | **0.34 µs (5.14×)** | 1.09× |

| Frame3 (3 systems, **no shared columns**) | Sequential | Fused |
|---|---:|---:|
| Fragmented, 1K / 100K / 1M | 0.75 µs / 119.5 µs / 1.37 ms | 0.64× / 0.81× / 0.86× (**slower**) |

- Fused output is **bit-identical** to sequential passes, verified by
  conformance for both H1 variants and both patterns. That includes a
  structural change between frames that alters which systems apply to
  which partition. It is also clean under ASan/UBSan.
- Automatic fusion reaches the hand-merged kernel. Writing systems as
  small, single-purpose kernels therefore costs nothing once they are fused.
  That removes the usual ECS tension between small, composable systems and
  fast code.
- Fusion is not free: fusing systems that share no data is a net loss
  (§4).

## 2. Model

- **Fusion group:** an ordered list of systems `S1..Sk` run by one executor
  call. Order is the schedule (DAG) order.
- **Per-partition subset:** each H1 partition's signature says which
  declared queries it matches, so for each partition the set of group
  members that apply is known *before* the loop:
  `subset(P) = { j : Sj.query ∈ signature(P) }`.
- **Specialised loop per subset.** The runtime subset is dispatched to a
  compile-time instantiation (`2^k` variants). The inner loop therefore
  contains exactly the kernels that apply, with no per-row "does Sj
  apply?" branches. Partitions that match none are skipped.
- **One pointer per component type** across the whole group. Systems that
  share a component read and write through *the same* pointer, so after
  inlining the compiler sees one merged kernel: values stay in registers
  from one system to the next, and no false aliasing is assumed between
  per-system copies of the same column.

```
for each partition P with subset(P) ≠ ∅:            // e.g. Medium: {Integrate, Forces, Wrap, RotHealth}
    cols = { Position*, Velocity*, Health*, Rotation* }   // one pointer per type in the group
    for row i:                                       // one pass
        Integrate(pos[i], vel[i]); Forces(pos[i], vel[i]); Wrap(pos[i], vel[i]); RotHealth(h[i], r[i]);
```

## 3. Legality

A group may be fused when all of these hold. The rules come from loop fusion
theory ([Kennedy & McKinley, LCPC 1993](https://link.springer.com/chapter/10.1007/3-540-57659-2_18)),
specialised to ECS systems:

| # | Rule | Why | How the design knows |
|---|---|---|---|
| L1 | Every member is **row-local**: it reads and writes only the current entity's components. | Interleaving rows equals running the passes back to back only if row *i* never observes row *j*. | Default for query callables. Systems that use `find()` on other entities, spatial queries or relationships are marked non-local and are never fused. |
| L2 | **No structural change inside the group** (add/remove/destroy take effect at commit). | A move mid-pass would change the partition subsets. | Deferred mutation (research §4.6): commit points split groups. |
| L3 | **No cross-row reduction consumed inside the group.** A system that sums over all rows cannot feed a later member in the same pass. | The sum is only complete after the pass. | Reductions are declared (`Reduce<T>`) and end a group. |
| L4 | **Schedule order is preserved per row.** | Keeps the sequential semantics for write→read chains (Integrate writes Position, Wrap reads it). | The fold applies members in group order. |
| L5 | **No external side effects that depend on global order** (I/O, events that must fire in row order across systems). | Fusion reorders side effects across systems. | Such systems are non-local (L1). Sub0Pub events are emitted after commit anyway. |
| L6 | **All members run under the same execution context** (same Sub0Pipeline job, same thread partitioning). | One loop, one job. | The planner forms groups only from consecutive DAG nodes with no other dependency between them. |

L1 + L4 are why the prototype is bit-identical: each row sees exactly the
same sequence of operations as in the sequential schedule.

## 4. Grouping (what the planner should fuse)

Optimal fusion for locality is NP-hard in general (Kennedy & McKinley), so
we use a rule plus a cost check. H7 gives the rule:

- **G1. Fuse systems that share columns.** Take connected components of
  the "shares a component" graph over consecutive, legally fusable systems.
  Shared columns are loaded once instead of once per system. Evidence:
  FusionFrame, 3.5–5.3×.
- **G2. Do not fuse groups that share nothing.** Frame3 (Physics,
  RotHealth, Pulse touch disjoint columns) got 0.64–0.86× when fused.
  There is no reuse to gain, and the combined loop streams six arrays of
  mixed element sizes (8 B, 4 B, 16 B) at once. The vector width is then
  set by the smallest element, the other streams pay for shuffles, and
  register and prefetch pressure rise. The loops still vectorise, as the
  compiler reports confirmed, so this is efficiency lost, not a failure to
  vectorise.
- **G3. An unrelated system inside a sharing group is neutral.**
  FusionFrame fused *with* RotHealth (Fused) and without it (FusedGrouped)
  are within noise of each other. So G2 is about groups with *no* reuse at
  all, not about every member having to share.
- **G4. Cost check (next step).** Estimated bytes saved (shared-column
  footprint × (members − 1)) against the added number of streams. Only fuse
  when the saving is positive. The spike leaves grouping to the caller;
  the planner will derive it from declared access (§6).

## 5. Implementation lessons from the prototype

1. **Fusion depends on full inlining.** The first working version was *no
   faster* than sequential in the benchmark binary, yet 4× faster in a small
   test file. In a large translation unit GCC's heuristics stopped inlining
   partway through the fused call chain, so the merged kernel never formed.
   Fix: `__attribute__((flatten))` on the fused loop. Production code needs
   the equivalent per compiler (GCC/Clang `flatten`; MSVC equivalent to be
   verified), plus a regression benchmark that fails if fused ≠ hand-merged.
2. **Bind columns by type, not per system.** Per-system copies of the same
   column pointer defeat store-to-load forwarding and force alias versioning.
3. **Subset dispatch costs code size.** `2^k` loop instantiations per group:
   fine at k ≤ 4–5. Larger groups should dispatch only the subsets that
   actually occur (known from the partition list at plan time), or split.
   This matters for ESP32 flash (H4).

## 6. Integration design

- **Declared access.** Fusion legality (L1–L3) and grouping (G1) need
  per-system `Read<T>`/`Write<T>` and locality/reduction markers, not just a
  query. The spike's systems declare only `Query<Cs...>` (all treated as
  write). Action: introduce the `Access<Read<A>, Write<B>, ...>` declaration
  from the research note; it also gives `const T&` for read-only members.
- **Sub0Pipeline.** A fusion group is **one job** in the DAG. The planner
  collapses consecutive fusable nodes into one node and keeps the external
  edges (the union of the members' edges). Commit points and reductions are
  group boundaries. Data parallelism is unchanged: a fused job splits by
  partition or row ranges exactly as an unfused one would.
- **Partitions (H1).** Subsets come straight from partition signatures, so
  partitioning and fusion reinforce each other. A system tree (C1) makes
  subsets nested, which reduces the number of distinct subsets and so the
  instantiations.
- **Change detection / ticks.** A member that writes a column marks it
  changed once per row. In a fused pass, the marks of several writers of
  the same column can merge into one store.
- **Static / embedded.** In static mode, groups and subsets are known at
  compile time, so only occurring subsets are instantiated. On ESP32-class
  cores without wide SIMD, the gain comes from fewer memory passes and less
  loop overhead. To be measured in H4.
- **Debuggability.** A `SUB0DATASTORE_NO_FUSION` build switch runs groups
  sequentially (same results, by L1–L4), so per-system profiling and
  stepping stay possible. Timing is reported per group when fused.

## 7. Prior art

- **Halide:** schedules decide how stages are fused (`compute_at` /
  `compute_inline`) separately from the algorithm, as here: systems stay
  small and the plan fuses them ([Ragan-Kelley et al., PLDI 2013](https://dl.acm.org/doi/10.1145/2491956.2462176)).
- **Data-centric query compilation (HyPer):** fuses relational operators
  into pipelines so tuples stay in registers across operators, which is the
  database analogue of fusing systems ([Neumann, PVLDB 2011](https://dl.acm.org/doi/10.14778/2002938.2002940)).
- **Loop fusion theory:** legality and the locality objective (NP-hard in
  general) ([Kennedy & McKinley, LCPC 1993](https://link.springer.com/chapter/10.1007/3-540-57659-2_18)).
- **ECS engines:** none of the surveyed engines (EnTT, flecs, Bevy, Unity
  Entities) document automatic fusion of separately written systems; they
  run each system as its own pass or job.

## 8. Next steps

| Step | What | Pass criterion |
|---|---|---|
| H7b | Planner forms groups automatically from declared `Read/Write` access + DAG order (G1–G4) | Picks FusionFrame's grouping, declines Frame3; equals hand grouping |
| H7c | Fusion as one Sub0Pipeline job; data-parallel split | Parallel fused ≥ parallel sequential × (single-thread speed-up) |
| H4+ | Static mode on ESP32-P4 with occurring-subset instantiation | Speed-up without SIMD; flash growth reported |
| — | Regression guard: fused ≡ hand-merged within 10% in CI benchmark | Catches inlining regressions (lesson 1) |
