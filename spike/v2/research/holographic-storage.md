# System-driven ("holographic") storage for SubzeroECS v2

Research note: use cases, prior art, design patterns and a proposed
architecture. It follows the storage-model spike in [../FINDINGS.md](../FINDINGS.md).

> Status: research / proposal, not a decision. Every design claim here is meant
> to be tested by the spikes in [§8](#8-validation-plan) against the baseline
> in [../results/](../results/).

## Decisions so far

| # | Decision | Date |
|---|---|---|
| D1 | Archetype-class iteration is the target; build on system-derived ("automatic") partitions. | 2026-09-30 |
| D2 | Working title for the storage library: **Sub0DataStore** (`sub0datastore` namespace). | 2026-09-30 |
| D3 | **Prefer no copies.** Where one arrangement cannot serve every system, bind the column **by reference** first. That means either a partitioned reference set into the home columns, or a per-entity reference to a central authority. Both sit behind one container/accessor type, so system code sees the direct case normally (§4.3). | 2026-09-30 |
| D4 | Replication stays a **viable fallback**. Only systems with declared write access can mutate a component, so each component has a known single writer per phase, which makes replica coherency tractable (§4.3.4). | 2026-09-30 |

## TL;DR

- **Idea.** Classic archetypes are keyed by *which components an entity has*.
  Instead, key physical partitions by **which declared systems an entity
  matches**. The set of systems then decides the data arrangement: components
  no system filters on never cause fragmentation or data moves, and every
  system iterates only whole partitions of tightly packed columns.
- **"Holographic" property.** An entity can appear in the views of several
  systems at once. For each system, its data is in a contiguous arrangement for
  that system. In order of preference:
  1. **Arrangement, one copy.** Order the partitions so that each system's
     partitions are adjacent (the consecutive-ones problem).
  2. **Reference binding, still one copy.** Where the arrangement cannot serve
     a system, bind the column by reference. Either a partitioned *reference
     set* of row indices into the home columns, or a *per-entity reference*
     to a central authority (shared/prefab/parent data). One container type
     hides which binding is in use, and the direct case has zero overhead.
  3. **Replication (fallback).** Keep a system-shaped copy, synchronised at
     pipeline commit points. It is tractable because only systems with
     declared write access can mutate a component.
- **Prior art exists for every part, separately:**
  - EnTT nested groups, Legion packing groups, Bevy tables vs. archetypes,
    flecs non-fragmenting components and Unity enableable components.
  - Workload-driven layouts in databases: Data Morphing, HYRISE, H2O, Peloton,
    Fractured Mirrors, CopyRight, Chestnut.
  - PL/HPC work that separates algorithm from layout: Halide, Taichi, LLAMA,
    Kokkos.

  I found no ECS that derives its physical partitioning *from the declared
  system set*, or that picks direct, reference or replica binding per system
  from that set.
- **Proposal.** Put the storage engine in a new domain-agnostic library, working
  name **Sub0DataStore**. It takes a schema plus declared access patterns, produces
  a *plan*, and serves spans. SubzeroECS v2 becomes a thin ECS vocabulary on
  top: entities→rows, components→columns, systems→access declarations.
  Sub0Pipeline supplies scheduling and commit points, and Sub0Pub supplies
  change events.
- **Planning modes.** Planning can happen at compile time (a static system list
  gives a constexpr plan, zero-allocation, for ESP32) or at startup and
  adaptively (desktop). It is the same model with different policies.

---

## 1. Problem statement and vocabulary

The spike ([FINDINGS.md](../FINDINGS.md)) showed:

- Archetype tables are the only design that gets near the SoA roofline on
  iteration (6× v1 on Update2 at 100K, 85–95% of hand-written code).
- They are about 18× slower than a sparse set when components are added and
  removed. That cost comes from the *keying*: every component in an entity's
  signature is a partition key, so any add/remove moves the entire row.
  This happens even when no system cares about that component.

The user requirement: *archetypes should be automatic, so that the systems
that are present impose the data arrangement by design.*

| Term | Meaning here |
|---|---|
| **Component signature** | The set of component types an entity has. Classic archetype key (flecs, Bevy, Unity). |
| **Access declaration** | What a system touches: `required` (filter + column), `excluded` (filter), `optional` (read if present), each marked read or write. Derived from the system's type. |
| **Match signature** | Bit vector over the declared systems: bit *i* is set iff the entity satisfies system *i*'s filter. |
| **Partition** ("automatic archetype") | The set of entities sharing a match signature. The physical unit of storage. |
| **Hot column** | A component that is `required` by at least one system matching the partition. Stored densely inside the partition. |
| **Side storage** | Where every other component of an entity lives (sparse set). Costs nothing for iteration. |
| **Projection** | The view a system sees: the union of its partitions, restricted to its hot columns. |
| **Binding** | How a system's projection reaches a column's values in a partition: **direct** (contiguous span), **reference set** (row indices into home columns), **authority reference** (value lives on another row, the central authority), or **replica** (copy). |
| **Home column** | The single authoritative storage of a component's values. |
| **Replica** | An optional physical copy of a projection in a different order/layout, kept coherent with the home copy. |
| **Plan** | Output of the planner: partitions, their order, hot columns per partition, replicas and column grouping. |

## 2. Use cases → requirements

Sub0 libraries target embedded (ESP32-P4 via Sub0Pipeline's FreeRTOS
executor), desktop and games, so the use cases span both ends.

| # | Use case | Access pattern | Churn | N | Pressure on the storage |
|---|---|---|---|---|---|
| U1 | **Game frame loop** (physics, AI, animation, render extract) | Many systems, heavily overlapping queries, mostly streaming | Medium: status effects, targeting, selection tags change every frame | 10K–1M | Streaming speed on overlapping queries; cheap tag churn |
| U2 | **Embedded sensor fusion / device model** (ESP32, Sub0Pipeline device init) | Fixed, known system set; small N; periodic ticks | Low after init | 10–4K | Zero allocation, fixed capacity, compile-time plan, small code size |
| U3 | **Robotics / control loops** | Hard-periodic reads of a few components; deterministic order | Low | 100–10K | Deterministic iteration order; bounded worst-case structural cost |
| U4 | **Particles / crowd / agent simulation** | 1–3 very hot kernels over nearly all entities | High create/destroy | 100K–10M | Roofline streaming, SIMD-friendly SoA, cheap bulk spawn/despawn |
| U5 | **Editor / tooling / inspection / hot reload** | Ad-hoc queries, random access, systems added at runtime | Arbitrary | any | Replanning without restart; random access; debuggability |
| U6 | **Networking, replication, rollback** (cf. Overwatch GDC 2017) | Snapshot/serialise subsets; diff against the previous frame | Per frame | 1K–100K | Change detection, cheap snapshot of a projection, deterministic layout |
| U7 | **Data-processing batches** (Sub0Pipeline's existing domain) | Staged transforms, each stage its own projection | Batch | 1K–10M | Stage-shaped layouts, possibly replicated per stage |
| U8 | **Hierarchies / UI / scene graph** | Parent/child traversal, relationships | Medium | 100–100K | Non-fragmenting relationships (flecs lesson), traversal order |

Requirements derived from these (used by the rest of the doc):

- **R1 Streaming.** Queries iterate contiguous typed spans that
  auto-vectorise (U1, U4, U7).
- **R2 Churn isolation.** Changing a component no hot system filters on must
  not move data (U1, U8).
- **R3 Static mode.** Compile-time plan, fixed capacity, no heap after init
  (U2, U3).
- **R4 Determinism.** The same system set and the same operations produce the
  same layout and iteration order (U3, U6).
- **R5 Replanning.** Systems can be added or removed at runtime, with the
  layout migrated incrementally (U5).
- **R6 Deferred mutation.** Structural changes are staged and applied at
  explicit commit points, which also enables parallel systems (U1, U7).
- **R7 Change visibility.** Cheap per-column change tracking and events on
  add/remove (U6, observers).
- **R8 Random access.** O(1) lookup from entity handle to data (U5, U6).
- **R9 Memory honesty.** Bounded, reportable overhead; replicas are opt-in and
  budgeted (U2).

## 3. Prior art

### 3.1 ECS engines: partial answers already shipping

| System | Relevant mechanism | What we take | What we avoid / improve |
|---|---|---|---|
| **EnTT groups** ([wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System), [ownership discussion](https://github.com/skypjack/entt/discussions/829)) | Full-, partial- and non-owning groups *reorder sparse-set pools* so entities matching a group are packed at the front, giving perfect SoA iteration for that query. A component may be in several groups **only if they are nested**. | Query-declared layout. The swap-to-boundary trick makes membership changes O(1) per column. The nesting constraint is the one-copy limit (see §4.2). | Groups are declared by hand and a separate concept from systems. Non-nested overlap falls back to slow paths silently. |
| **Legion** ([`WorldOptions` / `GroupDef`](https://docs.rs/legion/latest/legion/world/struct.World.html), [storage docs](https://docs.rs/legion/latest/legion/storage/index.html)) | Archetypes plus user-declared *component groups*. `World::pack` lays archetype slices out contiguously so a group's queries iterate one slice. Packing only happens once slices are "stable" and an estimate of saved cache misses passes a threshold. | Packing across archetypes, and a cost-model gate on reorganising. | Each component can be in exactly one group; groups are hand-declared. |
| **Bevy** ([`StorageType`](https://docs.rs/bevy/latest/bevy/ecs/component/enum.StorageType.html), [`archetype` module](https://docs.rs/bevy_ecs/latest/bevy_ecs/archetype/index.html)) | Per-component Table vs SparseSet storage. **Archetype ≠ table**: archetypes that differ only by sparse components share one table. Tick-based change detection, and access sets (`FilteredAccess`) for parallel scheduling. | The logical/physical split, which is our partition vs side storage. Change ticks. Access sets as the input to scheduling. | Adding a sparse component *still changes the archetype* (bookkeeping moves); the choice is manual per component. |
| **flecs** ([v4.1 release](https://ajmmertens.medium.com/flecs-4-1-is-out-fab4f32e36f6), [Building an ECS #2](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9)) | Archetype graph with add/remove edges; cached queries store matching tables. v4.1 **`DontFragment`**: adding or removing never changes the table (the first open-source ECS with both storage kinds). Relationships. | Edge caching, query caches, and the non-fragmenting trait. Our planner *infers* that trait: a component no filter uses is non-fragmenting automatically. | The trait is manual and not tied to what systems actually filter on. |
| **Unity Entities** ([enableable components](https://docs.unity3d.com/Packages/com.unity.entities@6.5/manual/components-enableable-intro.html), [chunk allocations](https://docs.unity3d.com/Packages/com.unity.entities@1.3/manual/performance-chunk-allocations.html), [shared components](https://docs.unity3d.com/Packages/com.unity.entities@1.0/manual/components-shared-optimize.html)) | 16 KiB chunks per archetype. **Enableable components** toggle a per-entity bit instead of moving data; chunks are skipped when all bits are off. Shared components partition chunks by value, and misuse causes fragmentation. | Enable bits as a third answer to churn (besides moving and side storage). Chunking bounds move cost. Fragmentation warnings. | Manual choice per component; value-partitioning footguns. |
| **Our Machinery** ([syncing a data-oriented ECS](https://ourmachinery.com/post/syncing-a-data-oriented-ecs/)) | Bitmask entity types; strategies for change tracking against external systems. | Change-propagation patterns for replicas and observers. | — |

**Gap:** every engine offers the knobs (groups, sparse, don't-fragment,
enableable, packing), but *the user sets them by hand*. None derives them from
the set of systems that actually runs. A recent formal treatment of archetype
ECS ([Tasnim & Zhao, SAC '26](https://arxiv.org/abs/2606.14919)) and the Core
ECS concurrency model ([Redmond et al., OOPSLA 2025](https://arxiv.org/abs/2508.15264))
both model archetypes as component-signature keyed. Neither covers
query-keyed partitions.

### 3.2 Databases: workload-driven physical design (decades of evidence)

The ECS problem is the classic *physical design* problem. The table is the
world, rows are entities, columns are components and queries are systems.
Databases have studied layouts derived from the query workload since the
1990s.

| Work | Idea | Relevance |
|---|---|---|
| **NSM / DSM / PAX** ([Ailamaki et al., VLDB 2001](https://research.cs.wisc.edu/multifacet/papers/vldb01_pax.pdf)) | Row vs column storage; PAX keeps columns *within* pages (minipages): 50–75% fewer L2 misses than NSM. | Partitions with SoA columns are PAX pages. Unity chunks are PAX. |
| **Data Morphing** ([Hankins & Patel, VLDB 2003](https://www.vldb.org/conf/2003/papers/S13P03.pdf)) | Derive a cache-efficient attribute grouping *from the query workload* and use it as the storage template. | The closest ancestor of "systems decide the layout". |
| **HYRISE** ([Grund et al., PVLDB 2010](https://www.vldb.org/pvldb/vol4/p105-grund.pdf)) | Automatic vertical partitioning into column groups of varying width, chosen with a cache-miss cost model; 20–400% gains over all-row or all-column. | Column-grouping decisions (§4.4) and a cost model we can borrow. |
| **H2O** ([Alagiannis, Idreos & Ailamaki, SIGMOD 2014](https://stratos.seas.harvard.edu/publications/h2o-hands-free-adaptive-store)) | No fixed layout: data is materialised in several layouts *simultaneously*, chosen per query class on the fly, and adapts continuously. | Direct precedent for "holographic" multi-layout storage and adaptive replanning. |
| **Peloton "Bridging the Archipelago"** ([Arulraj, Pavlo & Menon, SIGMOD 2016](https://www.researchgate.net/publication/304021389_Bridging_the_Archipelago_between_Row-Stores_and_Column-Stores_for_Hybrid_Workloads)) | Different layouts for different segments of the same table, evolved continuously from observed access patterns; up to 3× over static layouts. | Per-partition layout choice, online evolution (R5). |
| **Fractured Mirrors** ([Ramamurthy, DeWitt & Su, VLDB 2002](https://www.vldb.org/conf/2002/S12P03.pdf)) | Keep two replicas, one NSM and one DSM, and route each query to the better one. | The original "same data, two shapes" design; the replica pattern of §4.3. |
| **CopyRight — Replicated layout** ([Sudhir, Cafarella & Madden, PVLDB 15, 2021](https://www.vldb.org/pvldb/vol15/p984-sudhir.pdf)) | *Partial* replication: replicate only some columns and rows, each replica laid out differently; 1.1–7.9× over the best single layout with only 25% space overhead. | Evidence that budgeted partial replicas pay off. Informs the replica cost model and budget (R9). |
| **Database Cracking** ([Idreos, Kersten & Manegold, CIDR 2007](https://stratos.seas.harvard.edu/publications/database-cracking)) | Reorganise physically *as a side effect of queries* ("the data is organised the way users ask for it"). | Incremental adaptive reorganisation, e.g. idle-time repacking as Sub0Pipeline jobs. |
| **Chestnut** ([Yan & Cheung, PVLDB 12, 2019](http://www.vldb.org/pvldb/vol12/p1513-yan.pdf)) | Synthesises application-specific in-memory layouts *and* query plans from the application's known queries; 3.6× average, up to 42×. | Precedent for **compile-time layout synthesis from a known query set**, i.e. our static mode (R3). |
| **Materialised views / index advisors** | Store data redundantly in query-shaped form; automated physical-design tuning. | The vocabulary for replicas, and the advisor → planner analogy. |

### 3.3 Programming languages and HPC: separate the algorithm from the layout

| Work | Idea | Relevance |
|---|---|---|
| **Halide** ([Ragan-Kelley et al., PLDI 2013](https://dl.acm.org/doi/10.1145/2491956.2462176)) | Separate *what* is computed (the algorithm) from *how* (the schedule: storage and order), plus autoschedulers. | The systems are the algorithm and the plan is the schedule. User code never names layouts, though it may *hint* at them. |
| **Taichi SNodes** ([Hu et al., SIGGRAPH Asia 2019](https://yuanming.taichi.graphics/publication/2019-taichi/)) | Data-structure hierarchy declared separately from the kernels; kernels are written as if dense. | Systems are written against spans/projections, whatever the physical form. |
| **LLAMA** ([Gruber et al., SPE 2022](https://arxiv.org/abs/2106.04284)) | C++ header-only library: swappable mappings (AoS, SoA, AoSoA, split, instrumented "trace/heatmap" mappings) with zero overhead, generating code identical to handwritten layouts. | The strongest C++ precedent for a separate layout-provider library. Its **instrumented mappings** are a way to collect the access statistics the planner needs. |
| **Kokkos Views** ([docs](https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/View.html)) | The layout is a template policy, with a default chosen by execution space. | Policy-based layout selection; the default depends on the target (embedded vs desktop). |
| **Structure splitting / field reordering** ([Chilimbi, Davidson & Larus, PLDI 1999](https://dl.acm.org/doi/10.1145/301618.301635)) | Profile-driven hot/cold splitting of structs. | Hot columns vs side storage is the same hot/cold split, driven by declared access instead of profiles. |

### 3.4 Theory

- **Consecutive-ones property** (C1P). Given a 0/1 matrix, is there a
  column order in which every row's ones are contiguous? Booth and Lueker's
  **PQ-trees** decide it in linear time and represent *all* valid orders
  ([Booth & Lueker 1976](https://www.ic.unicamp.br/~meidanis/courses/mo640/2015s1/texts/Booth-Lueker-1976.pdf)).
  With partitions as columns and systems as rows, C1P answers exactly
  whether a single copy can give every system one contiguous span (§4.2).
- **Laminar families.** If the systems' partition sets are pairwise nested or
  disjoint, C1P always holds. This is why EnTT restricts ownership to nested
  groups.

## 4. Design

### 4.1 Query-signature partitioning ("automatic archetypes")

Let the world declare systems `S1..Sq`, each with an access declaration.
For an entity *e*, `match(e)` is a *q*-bit vector. **Partitions are the
equivalence classes of `match`.**

**Invariant (columns follow queries).** In partition *P*, the hot columns are
the union of the `required` components of every system matching *P*. Every
entity in *P* has all of them by construction, so each hot column is dense.
Every other component of *e* lives in side storage (sparse set).

Consequences:

1. **Every system iterates only whole partitions of dense columns.** No
   per-entity membership test, which gives R1 and archetype-class speed.
2. **Churn isolation is automatic (R2).** An add/remove moves data only if it
   flips a bit of `match(e)`. Components that appear in no filter never move
   anything. This is flecs' `DontFragment`, *inferred* instead of annotated.
3. **Fragmentation is bounded by systems, not components.** The number of
   partitions is at most `min(2^q, #distinct component signatures)`. In
   practice it is far smaller, because filters correlate.
4. **The layout is a pure function of the system set.** The same systems
   always give the same partitioning, which gives R4 determinism.
5. **Adding a system *refines* partitions and removing one *coarsens* them.**
   Replanning is a split or merge of existing partitions, never a full
   reshuffle, which keeps R5 tractable.

**Worked example (the spike workload).**

Systems:
- Physics{Pos, Vel}
- RotHealth{Health, Rot}
- Pulse{Scale, Color}
- TagSweep{Pos, Vel, Tag}

| Partition (match bits P/R/U/T) | Hot columns | Side storage | Classic archetypes it absorbs |
|---|---|---|---|
| Small `1000` | Pos, Vel | — | {Pos,Vel}, {Pos,Vel,Frozen}, … |
| Medium `1100` | Pos, Vel, Health, Rot | Scale | {…,Scale}, {…,Scale,Frozen} |
| Large `1110` | Pos, Vel, Health, Rot, Scale, Color | Team, Flags | {…,Team,Flags}, {…,Frozen} |
| Small+tag `1001`, Medium+tag `1101`, Large+tag `1111` | as above + Tag | as above | … |

`Frozen`, `Team` and `Flags` appear in no filter, so the spike's AddRemove
scenario (18× slower on archetypes) moves **no** data under this scheme and
should cost about the same as the sparse set. That is hypothesis **H1** in §8.
Classic archetypes would have 12 or more tables here; the system-derived
scheme has 6.

**Optional reads.** `optional<T>` components are read from side storage by
default. When a system's optional read is hot (by declaration or measured),
the planner may *promote* it into a partition key bit. That is a refinement,
so it is always safe.

### 4.2 Ordering partitions: one copy, every system contiguous

With per-column global arrays, where a partition is a contiguous row range of
every hot column (Legion-style packing), a system's projection is the union of
its partitions' ranges. **If each system's partitions are adjacent, it
iterates one span per column.** This is the C1P problem on the
partition × (system ∪ column) matrix.

In the example, the order `[S, M, L, L+t, M+t, S+t]` makes every system and
every column contiguous:

- Physics is all six partitions.
- RotHealth is `M…M+t`.
- Pulse is `L…L+t`.
- TagSweep is `L+t…S+t`.
- The Health column is `M…M+t`.

A PQ-tree finds such an order or proves none exists, and when one exists it
represents *all* the valid orders. That freedom is spent on a second
objective: **minimise expected migration distance.** The cost of moving an
entity between partitions *i* and *j* with boundary swaps (the EnTT group
trick generalised) is `|i − j| × #hot columns`. The planner orders partitions
so that frequent transitions (e.g. tag on/off) are adjacent. In the example,
the order above puts the tag toggles `S↔S+t`, `M↔M+t` and `L↔L+t` at
distances 5, 3 and 1. The order `[S, S+t, M+t, L+t, L, M]` is also fully
contiguous for every system and column (TagSweep = positions 1–3, RotHealth
and Health = 2–5, Pulse = 3–4) and cuts the distances to 1, 3 and 1. If tag
churn dominates, the planner should pick it.

When C1P fails (overlapping, non-nested systems), there are three options:

1. Accept *k* spans for some systems (a few range jumps are cheap).
2. Split the offending system's projection across partitions (the Legion/flecs
   default).
3. **Bind by reference** (a reference set for that system), or, as a last
   resort, **replicate** (§4.3).

A cost model decides.

**Chunked variant.** Global arrays make growth in a middle partition cost
O(#partitions) in boundary rotations. Fixed-size chunks per partition (Unity's
16 KiB, PAX pages) bound this at the price of "span list" iteration. The
single-span vs chunked trade-off is spike **H2**.

### 4.3 Holographic access without copies: bindings

"An entity appears in multiple archetypes" is met *logically* by §4.1–4.2.
The entity is in the projection of every system it matches, and each
component value is stored once. The remaining question is what a system gets
when the single arrangement cannot give it a direct span. The answer is
**bindings**: the planner chooses, per (system, component, partition), how
the system reaches the value. **Direct** is the default; **reference** comes
next; **replica** is the last resort.

#### 4.3.1 Direct binding (normal case)

The column is a contiguous `T*` range for the partition, as in §4.1–4.2. The
kernel compiles to the RawSoA loop that the spike measured at the roofline.

#### 4.3.2 Partitioned reference set (indirection into the home copy)

For a system whose partitions cannot be made contiguous, or which needs a
different *order* (e.g. render extract sorted by material, or a spatial-hash
order), the store keeps for that system a **reference set**: per partition, an
array of `uint32_t` row indices into the home columns, in the system's
preferred order. This is "late materialisation" with position lists from
column stores ([Abadi et al., ICDE 2007](http://www.cs.umd.edu/~abadi/papers/abadiicde2007.pdf)).
It is also EnTT's *partial-owning group*, which is direct for owned
components and indirect for the rest.

- **Cost.** The system does a gather per element, not a copy of the data.
  Maintenance on migration touches 4 bytes per reference set, not every
  column.
- **Mixed binding.** A system can be direct for some components and
  reference-bound for others. The planner makes the components the system
  *writes* direct where possible.
- **Order preservation.** Keeping indices sorted ascending (unless the system
  asked for a different order) keeps gathers prefetch-friendly.

#### 4.3.3 Per-entity reference to a central authority

Some data is conceptually *one value, many readers*: prefab/archetype
defaults, material or config blocks, a parent transform, a shared
calibration table on an embedded device. Here the entity carries a
**reference** (a generational handle) to the row that owns the value, the
*authority*, instead of a copy. This is the Flyweight pattern, with direct
precedents:
- flecs `IsA` inheritance, where queries match components through
  `Self|Up` traversal ([flecs Queries](https://www.flecs.dev/flecs/md_docs_2Queries.html),
  [Prefabs](https://www.flecs.dev/flecs/md_docs_2PrefabsManual.html)).
- Unity shared components (value stored once, chunks partitioned by value).

Two storage forms:

- **Per-entity reference column.** Each entity stores `Ref<T>`, and access is
  one indirection. Any entity can point at any authority.
- **Partitioned by reference.** The reference is a partition key (Unity
  shared-component style), so every entity in the partition shares one
  authority. The accessor returns *the same* `T&` for the whole span, which
  is constant per partition, and the kernel can hoist it out of the loop.
  The planner chooses this only when the number of distinct authorities is
  small, because each one creates a partition (the Unity shared-component
  fragmentation caveat).

**Writes go to the authority only**, and only from the system that holds
declared write access to that component. Readers never see a torn value,
because writes land at commit points (§4.6).

#### 4.3.4 Replica (fallback)

When a reader is hot enough that gathers or indirections cost more than a
copy (measured, or declared with a hint), the planner may materialise a
replica projection. It can be a subset of columns, possibly in a different
order, interleaving or precision. Precedents: Fractured Mirrors, H2O,
CopyRight.

Controlled write access makes this tractable:

- **Single writer per component per phase.** Access declarations mean the
  scheduler already knows the one system (or the ordered set of systems)
  that writes each component in a frame. The **home copy lives in that
  writer's preferred layout**; every other layout is a read-only replica.
- **Refresh after the writer, before the readers.** The refresh is a job
  inserted on the Sub0Pipeline edge between the writer and the first replica
  reader, driven by dirty ranges or change ticks (R7). It is snapshot
  consistent by construction; no locks.
- **Ownership can move.** If the dominant writer changes (e.g. a new system
  is registered), the replan swaps which copy is home. No other semantics
  change.
- **Budgeted.** Replicas need a declared memory budget (R9) and are off in
  static mode unless requested explicitly.
- **Cost model.** Replicate only if

  ```
  Σ over reader runs (reference-binding cost − direct cost) > refresh cost × refresh rate + memory penalty
  ```

  The comparison baseline is the *reference* binding, not the direct one,
  so replication has to beat indirection rather than an ideal.

#### 4.3.5 One container type for all bindings

Systems never name a binding. They receive a **column view** whose element
access is a policy, like `std::mdspan`'s `AccessorPolicy`
([cppreference](https://en.cppreference.com/cpp/container/mdspan)), LLAMA
mappings or Kokkos layouts:

| Binding | Accessor policy | Element access | Notes |
|---|---|---|---|
| Direct | `direct<T>` | `base[i]` | Normal case; vectorises |
| Reference set | `indexed<T>` | `base[idx[i]]` | Gather; indices sorted unless ordered |
| Authority (per-entity) | `referenced<T>` | `resolve(ref[i])` | One indirection; read-only unless the system owns writes |
| Authority (per-partition) | `uniform<T>` | `*value` | Hoistable constant |
| Replica | `direct<const T>` | `replica[i]` | Read-only by type |

**Dispatch is per partition batch, not per element.** The store iterates a
system's partitions, and for each batch it invokes the kernel with a
concretely typed view: a variant resolved once per batch, or a compile-time
instantiation in static mode. The inner loop therefore has no branches on
binding kind, and the direct case compiles to exactly the RawSoA loop.
Constness is part of the type: a system without write access to `T` only
ever receives `const T&`. That is how "only systems with write access can
write" is enforced at compile time rather than by convention.

### 4.4 Inside a partition: column grouping

- **Default: pure SoA** (one array per hot column). This is what vectorised in
  the spike.
- **Co-access grouping (HYRISE, Data Morphing).** Components whose
  *accessing-system set* is identical are always touched together. They may be
  interleaved (AoSoA with lane width = SIMD width) when a cost model predicts
  fewer cache lines.
- **Precision/format per replica** (e.g. `float16` render copies) is a future
  replica property, not a home-copy concern.

### 4.5 Answers to churn and sharing, chosen per component by the planner

| Mechanism | Precedent | Move cost | Iteration cost | When the planner picks it |
|---|---|---|---|---|
| **Side storage** (no key bit) | Bevy SparseSet, flecs DontFragment | none | only if read (random access) | Component is in no system filter |
| **Enable bit** (key bit replaced by a per-row mask) | Unity enableable components | none | mask test; block-skip when all off | Filter component with high toggle rate |
| **Authority reference** (value shared, entity holds a ref) | flecs `IsA`, Unity shared components | re-point a 4-byte ref | one indirection or hoisted constant | Many entities read one value (defaults, config, parent) |
| **Partition key bit** (move) | classic archetypes | row move / boundary swaps | none | Filter component with low toggle rate |

The toggle rate is declared (a `churn::high` hint) or measured (adaptive mode).
The decision can change at a replan.

### 4.6 Deferred mutation and commit points

- All structural changes (create, destroy, and any add/remove that flips a key
  bit) go into **command buffers**. Buffers are per worker, with no locks
  while systems run.
- Buffers are applied at **commit points**, which the scheduler places on
  Sub0Pipeline DAG edges. Batching lets the store sort moves by
  (source, destination) partition and do each boundary rotation once
  (cracking-style bulk reorganisation).
- Change events (`on_add`, `on_remove`, `on_set`) are emitted **after commit**
  through Sub0Pub. Observers see a consistent world and cannot re-enter
  mid-iteration.

### 4.7 Planning modes

| Mode | When | How | Targets |
|---|---|---|---|
| **Static** | System list known at compile time | `constexpr` planner: bitmask algebra over type lists gives partition types, column arrays and a fixed capacity per partition or a pool. No heap, no RTTI, no virtuals. The spike's StaticBitmask shows the memory/allocation profile. | U2, U3 (ESP32-P4) |
| **Startup** | Systems registered at runtime before the first tick | Runtime planner (PQ-tree, cost model), then a frozen plan | U1, U4, U7 |
| **Adaptive** | Tools, long-running servers | Startup plan plus statistics (per-column toggle rates, per-system run cost, optionally LLAMA-style instrumented access). Replans are applied incrementally as low-priority Sub0Pipeline jobs, H2O/cracking-style. | U5 |

Determinism (R4): given the same system set, the same hints and the same
seedless statistics snapshot, the planner must produce the same plan.
Adaptive mode records its replans so runs can be replayed (needed for U6
rollback).

### 4.8 Handles, lookup and stability

- A 32-bit generational handle (as in the spike's `common/entity.hpp`) maps to
  a location record `{partition, row}`. That is O(1) random access (R8),
  updated on moves.
- References and spans are valid **between commit points only**. The API
  should make this structural, e.g. spans are obtained inside a system
  invocation and cannot outlive it.
- **Static-mode typing constraint.** Components must be *trivially
  relocatable* (the spike's archetype design required trivially copyable).
  The dynamic mode may carry type-erased move/destroy function tables. This
  decision is still open (Q5).

## 5. Proposed architecture

```
 ┌──────────────────────────────── application ───────────────────────────────┐
 │  components (POD structs)      systems (callables + declared access)       │
 └──────────────┬─────────────────────────────────────────────┬───────────────┘
                │                                             │
 ┌──────────────▼──────────────┐   access sets / DAG   ┌──────▼──────────────┐
 │  SubzeroECS v2 (sub0ecs)    │──────────────────────▶│  Sub0Pipeline       │
 │  entities, components,      │◀──── commit points ───│  jobs = systems,    │
 │  systems → AccessDecl,      │                       │  edges = conflicts  │
 │  world facade, observers    │── on_add/on_remove ──▶│                     │
 └──────────────┬──────────────┘        Sub0Pub        └─────────────────────┘
                │ schema + AccessDecls
 ┌──────────────▼────────────────────────────────────────────────────────────┐
 │  Sub0DataStore (domain-agnostic "holographic" columnar store)             │
 │  Planner: match signatures → partitions → C1P order → bindings → groups   │
 │  Storage: hot columns per partition │ side storage │ enable bits          │
 │  Bindings: direct │ reference sets │ authority refs │ replicas (fallback) │
 │  Mutation: command buffers, batched commit │ Sync: replica refresh        │
 │  Policies: planning mode, capacity/allocator, chunking, replica budget    │
 └───────────────────────────────────────────────────────────────────────────┘
```

**Why a separate library.**

- The planner and store have no ECS concepts: they know rows, typed columns
  and access declarations, so they are testable in isolation.
- The same store can back other Sub0 uses: device registries, telemetry
  tables, and Sub0Pipeline batch stages (U7).
- It keeps SubzeroECS v2 small: vocabulary, ergonomics and scheduling glue.
- It follows the existing family pattern of narrow, composable libraries
  (Sub0Pub for messaging, Sub0Pipeline for scheduling).

**Naming:** working title **Sub0DataStore** (D2), namespace `sub0datastore`.

**Illustrative API (non-final, for discussion only).**

```cpp
// ---- Sub0DataStore: no ECS vocabulary -------------------------------------
namespace sub0datastore {
    using Schema = sub0datastore::schema<Position, Velocity, Health, Rotation, Tag, Frozen>;

    constexpr auto physics  = access<Schema>().write<Position, Velocity>();          // required ⇒ filter + hot
    constexpr auto tagSweep = access<Schema>().write<Position, Velocity>().read<Tag>();
    constexpr auto plan     = make_plan<Schema>(physics, tagSweep);                  // static mode: constexpr

    store<plan, fixed_capacity<4096>> s;                                             // no heap after init
    auto row = s.insert(Position{}, Velocity{});        // staged
    s.commit();                                         // batched moves
    for (auto [pos, vel] : s.spans(physics)) { /* std::span<Position>, std::span<Velocity> */ }

    // A binding is a property of the plan, never of system code: `mat` below may be
    // direct, indexed, referenced or uniform depending on the plan; kernel unchanged.
    s.for_each(renderExtract, [](const Position& p, const Material& mat) { /* ... */ });
    auto ent = s.insert(Position{}, authority_ref<Material>{sharedMaterialRow});   // per-entity authority ref
}

// ---- SubzeroECS v2: ECS vocabulary on top ----------------------------------
struct Physics {
    void operator()(sub0ecs::Write<Position> p, sub0ecs::Write<Velocity> v) const;  // access inferred from signature
};
auto world = sub0ecs::world<Physics, RotHealth, Pulse, TagSweep>(sub0ecs::static_plan, sub0ecs::capacity<4096>);
world.schedule(pipeline);   // Sub0Pipeline jobs + commit edges derived from access conflicts
```

## 6. Design pattern catalogue

| Pattern | Where | Source / precedent |
|---|---|---|
| Algorithm/schedule separation | Systems vs Plan | Halide, Taichi, LLAMA |
| Query optimiser / planner | Sub0DataStore planner | DB physical design, Chestnut |
| Accessor / layout policy | One column-view type over direct, indexed, referenced, uniform and replica bindings | `std::mdspan` AccessorPolicy, LLAMA, Kokkos |
| Late materialisation (position lists) | Partitioned reference sets | Abadi et al. ICDE 2007; EnTT partial-owning groups |
| Flyweight / central authority | Per-entity authority references | flecs `IsA` prefabs, Unity shared components |
| Single-writer ownership | Home copy follows the declared writer | Access declarations → Sub0Pipeline DAG |
| Materialised view | Replica projections (fallback) | Fractured Mirrors, H2O, CopyRight |
| Policy-based design | Planning mode, capacity, allocator, chunking | Kokkos layouts, Alexandrescu policies |
| Hot/cold splitting | Hot columns vs side storage | Chilimbi et al. |
| Unit of work / command buffer | Deferred structural changes | Unity ECB, flecs deferred mode |
| Snapshot isolation at barriers | Replica refresh at commit points | Double buffering, MVCC-lite |
| Observer (post-commit) | Change events | Sub0Pub |
| Generational index / slot map | Entity handles | Spike `common/entity.hpp` |
| Swap-to-boundary | O(1) membership moves | EnTT groups |
| Graph edge caching | Partition transition table | flecs archetype graph |
| Adaptive reorganisation | Adaptive mode | Database cracking, H2O, Legion packing |
| Access sets → scheduling | Systems → Sub0Pipeline DAG | Bevy `FilteredAccess`, Core ECS |

## 7. Risks and open questions

| # | Risk / question | Mitigation / how to resolve |
|---|---|---|
| Q1 | **Planner complexity and debuggability.** The layout is no longer obvious from the code. | Plan is a printable artifact (`dump_plan()`); deterministic; explainable ("why is X a key bit?"). |
| Q2 | **Replan cost** when systems are added at runtime (U5). | Refinement is splits only; incremental; background Sub0Pipeline jobs. Measure (H5). |
| Q3 | **Combinatorial blow-up** of match signatures with many optional-heavy systems. | Bounded by entity diversity; enable bits instead of key bits for high-cardinality filters; warn above a threshold. |
| Q4 | **Replica write amplification / staleness bugs.** | Reference bindings first (D3); replicas read-only by type, refreshed on the writer→reader edge, budgeted, off by default. |
| Q11 | **Authority reference lifetime.** What happens when the authority row is destroyed while entities still reference it? | Generational handles detect staleness. Policy choice: forbid (debug assert), cascade, or re-point to a default. Decide with U8 hierarchies. |
| Q12 | **Gather cost of reference sets** vs direct on real kernels. | Measure in H6; the planner prefers making written components direct. |
| Q5 | **Component type constraints** (trivially relocatable?). | Required in static mode; type-erased vtables in dynamic mode. Decide after H1. |
| Q6 | **Systems that write filter components** (a system removes its own filter tag). | Always deferred to commit, so iteration never sees its own moves. |
| Q7 | **Relationships / hierarchies** (U8). | Out of scope for the first cut; relationships default to non-fragmenting side storage (flecs lesson). |
| Q8 | **Is C1P usually satisfiable in real system sets?** | Measure on real system sets (a game sample plus an embedded sample) before building PQ-tree machinery; the fallback of k spans may be enough. |
| Q9 | **Embedded code size** of a constexpr planner and templates on ESP32. | Measure in H4; keep a runtime-light static path. |
| Q10 | **Does compile-time planning compose across translation units / modules?** | The world type is defined in one TU (the system list); the store is instantiated there. |

## 8. Validation plan

These are spikes on the existing harness, compared to
[baseline-linux-gcc13](../results/baseline-linux-gcc13.md) numbers.

| Spike | Build | Success criterion |
|---|---|---|
| **H1** Query-signature partitions | Archetype spike keyed by match signature + side storage | AddRemove (Frozen, unfiltered) ≤ 1.2× SparseSet (~62 µs @100K); Update2/Frame3 within 10% of Archetype; memory ≤ Archetype +10% |
| **H2** Partition ordering | Global columns + boundary swaps vs chunked partitions | Each system iterates 1 span (C1P case); tag-churn cost ≤ SparseSet ×2; Frame3 ≥ Archetype |
| **H6** Reference bindings | Frame with a system needing a non-C1P or reordered projection: (a) reference set, (b) per-entity authority ref, (c) per-partition uniform authority; all through one accessor type | Direct binding identical to the H1 loop (no abstraction cost, verify asm); reference set within 2× direct on Update2-class kernels; uniform authority ≈ direct |
| **H3** Replica (fallback) | Render-extract replica (sorted, compacted) | Refresh cost < saved iteration cost at 1 refresh/frame; staleness impossible by construction (tests) |
| **H4** Static mode on ESP32-P4 | constexpr plan, fixed capacity, FreeRTOS executor | Zero heap after init; code size and SRAM reported; Update2 vs StaticBitmask |
| **H5** Replan / fragmentation stress | 2^k optional-component mixes; add/remove systems at runtime | Partition count bounded as predicted; replan of 100K entities under a frame budget (target to be set) |

Recommended order: **H1 → H2 → H6 → H4 → H5 → H3.** H3 only runs if H6 shows a
reader where references lose badly. H1 is cheap (a variant of an
existing spike) and decides whether the core idea is real.

## References

ECS
- skypjack, *EnTT* — [Entity Component System wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System), [group ownership discussion](https://github.com/skypjack/entt/discussions/829), [tips & tricks](https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/)
- *Legion* — [`World` / `WorldOptions`](https://docs.rs/legion/latest/legion/world/struct.World.html), [storage](https://docs.rs/legion/latest/legion/storage/index.html)
- *Bevy* — [`StorageType`](https://docs.rs/bevy/latest/bevy/ecs/component/enum.StorageType.html), [archetypes](https://docs.rs/bevy_ecs/latest/bevy_ecs/archetype/index.html), [change detection](https://bevy-cheatbook.github.io/programming/change-detection.html), [FilteredAccess PR](https://github.com/bevyengine/bevy/pull/5105)
- S. Mertens, *flecs* — [v4.1 (DontFragment)](https://ajmmertens.medium.com/flecs-4-1-is-out-fab4f32e36f6), [Building an ECS #2](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9), [#3 Storage in pictures](https://ajmmertens.medium.com/building-an-ecs-storage-in-pictures-642b8bfd6e04), [ECS FAQ](https://github.com/SanderMertens/ecs-faq)
- flecs — [Queries (Self/Up traversal)](https://www.flecs.dev/flecs/md_docs_2Queries.html), [Prefabs / IsA inheritance](https://www.flecs.dev/flecs/md_docs_2PrefabsManual.html)
- Unity — [Enableable components](https://docs.unity3d.com/Packages/com.unity.entities@6.5/manual/components-enableable-intro.html), [chunk allocations](https://docs.unity3d.com/Packages/com.unity.entities@1.3/manual/performance-chunk-allocations.html), [shared components](https://docs.unity3d.com/Packages/com.unity.entities@1.0/manual/components-shared-optimize.html)
- Our Machinery — [Syncing a data-oriented ECS with a stateful external system](https://ourmachinery.com/post/syncing-a-data-oriented-ecs/)
- T. Ford, [Overwatch Gameplay Architecture and Netcode, GDC 2017](https://www.gdcvault.com/play/1024001/-Overwatch-Gameplay-Architecture-and)
- A. Tasnim, T. Zhao, [The Essence of Entity Component System, SAC '26](https://arxiv.org/abs/2606.14919)
- P. Redmond et al., [Exploring the Theory and Practice of Concurrency in the ECS Pattern, OOPSLA 2025](https://arxiv.org/abs/2508.15264)

Databases
- A. Ailamaki et al., [Weaving Relations for Cache Performance (PAX), VLDB 2001](https://research.cs.wisc.edu/multifacet/papers/vldb01_pax.pdf)
- R. Ramamurthy, D. DeWitt, Q. Su, [A Case for Fractured Mirrors, VLDB 2002](https://www.vldb.org/conf/2002/S12P03.pdf)
- R. Hankins, J. Patel, [Data Morphing, VLDB 2003](https://www.vldb.org/conf/2003/papers/S13P03.pdf)
- S. Idreos, M. Kersten, S. Manegold, [Database Cracking, CIDR 2007](https://stratos.seas.harvard.edu/publications/database-cracking)
- M. Grund et al., [HYRISE, PVLDB 2010](https://www.vldb.org/pvldb/vol4/p105-grund.pdf)
- I. Alagiannis, S. Idreos, A. Ailamaki, [H2O: A Hands-free Adaptive Store, SIGMOD 2014](https://stratos.seas.harvard.edu/publications/h2o-hands-free-adaptive-store)
- J. Arulraj, A. Pavlo, P. Menon, [Bridging the Archipelago between Row-Stores and Column-Stores, SIGMOD 2016](https://www.researchgate.net/publication/304021389_Bridging_the_Archipelago_between_Row-Stores_and_Column-Stores_for_Hybrid_Workloads)
- C. Yan, A. Cheung, [Generating Application-Specific Data Layouts for In-memory Databases (Chestnut), PVLDB 2019](http://www.vldb.org/pvldb/vol12/p1513-yan.pdf)
- D. Abadi, D. Myers, D. DeWitt, S. Madden, [Materialization Strategies in a Column-Oriented DBMS, ICDE 2007](http://www.cs.umd.edu/~abadi/papers/abadiicde2007.pdf)
- S. Sudhir, M. Cafarella, S. Madden, [Replicated Layout for In-Memory Database Systems (CopyRight), PVLDB 2021](https://www.vldb.org/pvldb/vol15/p984-sudhir.pdf)

PL / HPC / theory
- T. Chilimbi, B. Davidson, J. Larus, [Cache-Conscious Structure Definition, PLDI 1999](https://dl.acm.org/doi/10.1145/301618.301635)
- J. Ragan-Kelley et al., [Halide, PLDI 2013](https://dl.acm.org/doi/10.1145/2491956.2462176)
- Y. Hu et al., [Taichi, SIGGRAPH Asia 2019](https://yuanming.taichi.graphics/publication/2019-taichi/)
- B. Gruber et al., [LLAMA: The Low-Level Abstraction for Memory Access](https://arxiv.org/abs/2106.04284), [updates 2023](https://arxiv.org/abs/2302.08251)
- [`std::mdspan` and AccessorPolicy](https://en.cppreference.com/cpp/container/mdspan), [P2604 (data_handle rename)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p2604r0.html)
- [Kokkos View layouts](https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/View.html)
- K. Booth, G. Lueker, [Testing for the Consecutive Ones Property … Using PQ-Tree Algorithms, JCSS 1976](https://www.ic.unicamp.br/~meidanis/courses/mo640/2015s1/texts/Booth-Lueker-1976.pdf)
