# Design review: composing overlapping systems

> Design note: the reasoning behind a part of the library. Measurements are kept in
> [FINDINGS.md](../FINDINGS.md), open work in [BACKLOG.md](../BACKLOG.md).

Scope: the query-partition storage design, and the
proposal to resolve "an entity in two systems" by **composition** (system
tree, parenting, DAG) instead of copies or references.

Inputs:
- [FINDINGS.md](../FINDINGS.md): design and measurements.
- [holographic-storage.md](holographic-storage.md): the storage design.
- [store/world.hpp](../../include/sub0ecs/store/world.hpp): the store.
- The partition-count benchmark, [bench/spans_bench.cpp](../../bench/spans_bench.cpp).

Outcome: a reframing of the "duplicate" problem, a ranked composition
strategy, the experiments that would settle it, and the review findings
that are still open.

---

## 1. Where the design stands

| Area | State | Evidence |
|---|---|---|
| Partition key = matched declared systems ("automatic archetypes") | Validated | Iteration at archetype speed, non-fragmenting churn at sparse-set speed |
| Per-component choice: key bit / carry / side storage | Validated as *necessary* | Side-storing stable components costs memory for nothing; carrying them is close to free |
| Non-fragmenting membership lives only in its pool | Validated | It is what brought AddRemove to sparse-set speed |
| Single-copy contiguity across partitions (PQ-tree ordering) | **Questioned by this review** | §3 below |
| Reference bindings (reference sets, authority refs) | Proposed, unmeasured | [holographic-storage.md](holographic-storage.md) §4.3 |
| Replicas | Fallback, unmeasured | [holographic-storage.md](holographic-storage.md) §4.3.4 |
| Deferred mutation / commit points | Proposed, not implemented (moves are immediate) | — |
| Static constexpr planning (ESP32) | Proposed, unmeasured | — |

## 2. Reframing: in the partition model an entity is never duplicated

The worry behind reference/replica bindings was that an entity used by
two systems needs to "be in two places". The store shows that is not how the model
works:

- An entity lives in exactly **one** partition, the one keyed by the set
  of systems it matches.
- Every system iterates **all partitions it matches**, and each partition is
  dense for that system's components.
- So an entity used by Physics *and* Render is stored once, in the
  partition `{Physics, Render}`, and both systems stream over it directly.

What remains are three narrower problems, none of which is duplication:

| # | Real problem | When it bites |
|---|---|---|
| P1 | **Granularity:** a system's data is split across many partitions. | Only when partitions get small (§3). |
| P2 | **Order conflict:** two systems want the *same* rows in *different* orders (e.g. simulation order vs sort-by-material for rendering, or spatial order). | Order-sensitive systems only. |
| P3 | **Shared data:** many entities read one value (parent transform, material, device calibration). Storing it per entity is a copy; a per-entity reference is an indirection. | Hierarchies, prefabs, config. |

Composition-style solutions should be judged against P1–P3, not against a
duplication problem that does not exist.

## 3. Evidence: how much does contiguity across partitions matter?

`sub0ecs_spans_bench` runs the Update2 kernel over a fixed number of entities
split evenly into K partitions (spans), from one span to thousands. Current
figures are in [FINDINGS.md](../FINDINGS.md), section 3.

**Conclusion.** Above roughly a thousand entities per partition, span count is
free. The per-span cost only shows below a few hundred entities per
partition. A PQ-tree ordering, so that each system gets *one* span, solves a
problem that only exists when partitions are tiny. The cheaper fix is to
**not create tiny partitions** (§5.1). The experiments are scoped accordingly
(§6).

## 4. Composition options for overlapping systems

Running example: the "star" overlap, where C1P fails. Entity kinds:

| Entity | Components | Systems |
|---|---|---|
| E1 | Pos, Vel, Sprite, Brain | Physics{Pos,Vel}, Render{Pos,Sprite}, AI{Vel,Brain} |
| E2 | Pos, Vel | Physics |
| E3 | Pos, Sprite | Render |
| E4 | Vel, Brain | AI |

The partitions are P1(E1), P2(E2), P3(E3), P4(E4). Each system needs P1
plus one other, and P1 can only have two neighbours in a linear order, so
no single order makes all three systems contiguous. In the partition model this
costs one extra span for one system, which §3 shows is free at normal
sizes. The options below matter for P1–P3 generally.

### C1. System tree (nested refinement)

Systems form a tree in which a child's query *refines* its parent's:
child match set ⊆ parent match set. For example, `Physics → {Physics+Render,
Physics+AI}`, or `Movable → {Projectile, Vehicle}`.

- **Property.** The match sets of a tree are a laminar family, so a
  depth-first order of the tree's partitions gives *every* system in the
  tree one contiguous range. This is the consecutive-ones property,
  guaranteed by construction and needing no PQ-tree. It is the same
  constraint EnTT enforces for nested groups
  ([EnTT wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System)).
- **Overlap that is not nesting** (the star): the tree records *priority*.
  The system placed in the tree gets contiguity; a system outside it gets
  k spans. That is explicit and deterministic.
- **Declared or inferred.** The tree can be inferred from the subset
  lattice of query match sets (a Hasse diagram → spanning tree), with an
  optional user priority hint for ties. Inference keeps the "automatic"
  goal; declaration keeps embedded builds predictable.
- **Churn bonus.** Moving an entity between parent and child partitions is
  adjacent in DFS order, so boundary-swap moves are O(1) per column (the
  EnTT group trick).
- **Cost.** Planner complexity is low (tree, not PQ-tree), and it composes
  with static constexpr planning.
- **Addresses:** P1 (ordering and adjacency), not P2 or P3.

### C2. Parent/child composition (data parenting)

Model shared data *structurally*: the shared value lives on a **parent
entity**, and the entities that use it are **children** (`ChildOf(parent)`).
The store keeps each child partition **grouped by parent**, so each parent
has one contiguous run of children:

```
parent table:  [ P0 | P1 | P2 ]                  (e.g. Transform, Material, Calibration)
child table:   [ c c c | c c | c c c c ]         runs aligned to parent order
               run lengths: 3, 2, 4              (Dremel-style repetition info)
iteration:     for each parent p: v = p.value (load once)
                   for child c in run(p): kernel(c, v)
```

- **What it replaces.** It replaces the per-entity *authority reference*
  ([holographic-storage.md](holographic-storage.md) §4.3.3): the relationship becomes run-length structure, not a
  pointer per entity. The parent value is loaded once per run and hoisted
  out of the inner loop. There is no gather, no copy and no reference column
  in the hot loop.
- **Precedents:**
  - flecs `cascade`/`group_by(ParentDepth)` iterates hierarchies breadth-first
    by grouping tables by depth ([flecs Hierarchies](https://www.flecs.dev/flecs/md_docs_2HierarchiesManual.html)).
  - Bevy 0.16 made `ChildOf`/`Children` a first-class relationship
    ([Bevy 0.16](https://bevy.org/news/bevy-0-16/)).
  - Dremel/Parquet store nested data column-wise with repetition levels
    ([Melnik et al., VLDB 2010](https://research.google.com/pubs/archive/36632.pdf)).
  - Unity chunk components store one value per chunk.
- **Multi-level hierarchies** (transform trees) iterate breadth-first by
  depth (flecs-style), so a parent is always resolved before its children.
- **1:1 facets.** Splitting one entity into two linked rows gives run
  length 1: an aligned zip of two tables, with the order maintained by the
  store. It only pays off when the two facets have very different churn,
  otherwise it is just a worse partition. Not recommended as a general tool.
- **Cost.** Keeping child runs grouped by parent turns re-parenting into a
  move (like any key change). Parent destruction needs a policy (Q11:
  cascade, orphan or forbid).
- **Addresses:** P3 fully, and P1 for hierarchies. Not P2.

### C3. DAG phases (pipeline-driven layout)

Use the Sub0Pipeline DAG. Systems in a phase share one row order; when the
next phase needs a different order (P2), the store **permutes rows in place**
at the phase boundary. Example: simulation phase in insertion order, then
a sort by material key before the render-extract phase.

- It is a single copy (a permutation, not a duplicate), with no persistent
  indirection.
- Cost: O(rows × columns) moves per phase change, only for partitions the
  next phase is order-sensitive about. Incremental sorts (nearly sorted
  data from the previous frame) are cheap.
- Handles stay valid (records updated). Spans are only valid between commit
  points anyway ([holographic-storage.md](holographic-storage.md) §4.8).
- Compared with a reference set ([holographic-storage.md](holographic-storage.md) §4.3.2): this pays a permute
  once per frame instead of a gather on every access. It wins when the
  reordered system touches many columns or runs more than once per frame.
  A cost model decides.
- **Addresses:** P2.

### C4. System fusion (compose the systems themselves)

When systems in the same DAG phase visit the same (or nested) partitions and
have no conflicting access, **fuse** them into one pass per partition. The
data is read once for both. This is Halide's `compute_at`-style scheduling
applied to systems ([Halide](https://dl.acm.org/doi/10.1145/2491956.2462176)).

- Frame3 is the obvious case: Physics and RotHealth both visit the Medium
  and Large partitions.
- The gain is memory bandwidth, which matters at 1M+ where every system in
  the benchmark is memory-bound.
- Legality comes straight from declared access sets plus DAG ordering.
- Cost: scheduler complexity and register pressure. It is an optimisation,
  not a storage feature.
- **Addresses:** none of P1–P3 directly. It reduces the cost of many systems
  sharing data.

### Comparison

| | Copies | Hot-loop indirection | Fixes | Churn impact | Declarative burden | Static/embedded fit |
|---|---|---|---|---|---|---|
| **C1 System tree** | none | none | P1 | improves (adjacent moves) | low (inferable) | excellent |
| **C2 Parent/child** | none | none (per-run hoist) | P3, hierarchy P1 | re-parent = move | medium (relations) | good |
| **C3 DAG phases** | none (permute) | none | P2 | permute per phase | low (sort key per system) | good if small N |
| **C4 Fusion** | none | none | bandwidth | none | none (scheduler) | good |
| Reference set (§4.3.2) | none | gather per element | P1, P2 | cheap | none | good |
| Authority ref (§4.3.3) | none | 1 indirection per element | P3 | cheap | low | good |
| Replica (§4.3.4) | yes | none | P2 | refresh per phase | budget | poor |

## 5. Recommendations

1. **Adopt the reframing (§2).** The model has no duplicates, so drop "one
   span per system" as a goal. Keep partition-per-system iteration
   as the foundation.
2. **5.1 Granularity control first.** When a natural partition would fall
   below a threshold (a few hundred entities, tunable; §3 puts the knee
   there), **do not split it**. Merge it into its parent
   partition and let the smaller system filter in the loop with an enable
   bit ([holographic-storage.md](holographic-storage.md) §4.5). This bounds the P1 cost and also caps partition
   explosion (Q3).
3. **C1 system tree as the ordering mechanism.** Infer it from query
   subset relations, and let users annotate priority. Replace the PQ-tree
   plan with it: simpler, constexpr-friendly, deterministic.
4. **C2 parent/child composition replaces per-entity authority
   references** as the default answer to shared data (P3). Keep per-entity
   authority refs only for non-hierarchical, many-to-one links where
   grouping by parent is not worth maintaining.
5. **C3 phases for order conflicts (P2).** Reference sets stay as the
   alternative when the permute cost is higher (few columns touched, large
   partitions). Replicas remain last resort, as before.
6. **C4 fusion** is implemented: a gain on column-sharing systems, a loss on
   disjoint ones. See [fusion.md](fusion.md).
7. The binding abstraction ([holographic-storage.md](holographic-storage.md) §4.3.5, one column-view type) **still
   stands**. C2 adds a `per_run` accessor (the value is constant per run)
   next to `direct`, `indexed`, `referenced` and `uniform`.

## 6. Experiments that would settle it

| Experiment | Question | Build | Pass criterion |
|---|---|---|---|
| **Granularity** | Does merging small partitions (enable-bit filter) beat splitting them? | The store plus a threshold merge; fragmentation stress with 2^k optional-component mixes (k = 4…10) | Update-class systems close to "ideal" (all partitions above the threshold); partition count bounded; churn no worse than today |
| **System tree** | Does DFS order plus boundary swaps give contiguity and O(1) adjacent moves? | Global columns ordered by an inferred system tree; star example plus nested example | Nested systems = 1 span; a parent↔child move clearly faster than a table move |
| **Parent/child** | Does per-run hoisting beat per-entity authority refs and match direct access? | Parent table plus children grouped by parent; material/transform-style kernel; run lengths 1, 8, 64 | At least direct-with-copied-value at run length ≥ 8; clearly faster than a per-entity authority ref |
| Replica | Only if the two above leave an order conflict unsolved | — | — |

Granularity runs first, because it decides whether tiny partitions ever need
ordering at all.

## 7. Review findings still open

| Finding | Action |
|---|---|
| `each<Cs...>` must exactly match a declared query (static_assert). Ad-hoc and tool queries have no path. | Undeclared queries iterate the partitions whose columns ⊇ Cs *plus* a side-storage fallback. Or: declaring is how you get fast access, and everything else is a slow path by design. |
| Moves are immediate; there is no command buffer or commit semantics in the store. | Implement deferred mutation before any parallel scheduling of structural changes ([holographic-storage.md](holographic-storage.md) §4.6); re-measure churn with batched moves. |
| Empty partitions are never freed; per-query partition lists only grow. | Reclaim empty partitions at commit; compact query lists (flecs "empty table" handling). |
| Volatility (carry vs side) needs a hint; "automatic" is only partly true. | Churn counters per component to feed an adaptive mode; `Volatile<T>` stays as the override. |
| The planner runs at runtime even though the query list is known at compile time. | A static constexpr plan; the system tree (C1) makes it simpler. |

## 8. Open decisions

1. **Accept the reframing** (no duplication in the partition model; the
   targets are P1 granularity, P2 order, P3 shared data)?
2. **System tree: inferred with priority hints** (recommended), or
   declared explicitly?
3. **Adopt parent/child composition (C2) as the default for shared data,**
   demoting per-entity authority refs?
4. **Experiment order: granularity → system tree → parent/child?**
