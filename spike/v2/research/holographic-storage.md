# System-driven ("holographic") storage for SubzeroECS v2

Research note: use cases, prior art, design patterns and a proposed
architecture. It follows the storage-model spike in [../FINDINGS.md](../FINDINGS.md).

> Status: research / proposal, not a decision. Every design claim here is meant
> to be tested by the spikes in [§8](#8-validation-plan) against the baseline
> in [../results/](../results/).

## TL;DR

- **Idea.** Classic archetypes are keyed by *which components an entity has*.
  Instead, key physical partitions by **which declared systems an entity
  matches**. The set of systems then decides the data arrangement: components
  no system filters on never cause fragmentation or data moves, and every
  system iterates only whole partitions of tightly packed columns.
- **"Holographic" property.** An entity can appear in the views of several
  systems at once. For each system, its data is in a contiguous arrangement for
  that system. Two ways to get there:
  1. **Arrangement, one copy.** Order the partitions so that each system's
     partitions are adjacent (the consecutive-ones problem).
  2. **Replication.** Where one copy cannot satisfy every system, keep a
     system-shaped replica, synchronised at pipeline commit points.
- **Prior art exists for every part, separately:**
  - EnTT nested groups, Legion packing groups, Bevy tables vs. archetypes,
    flecs non-fragmenting components and Unity enableable components.
  - Workload-driven layouts in databases: Data Morphing, HYRISE, H2O, Peloton,
    Fractured Mirrors, CopyRight, Chestnut.
  - PL/HPC work that separates algorithm from layout: Halide, Taichi, LLAMA,
    Kokkos.

  I found no ECS that derives its physical partitioning *from the declared
  system set*, and none that replicates for competing layouts.
- **Proposal.** Put the storage engine in a new domain-agnostic library, working
  name **Sub0Store**. It takes a schema plus declared access patterns, produces
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
3. **Replicate** (§4.3).

A cost model decides.

**Chunked variant.** Global arrays make growth in a middle partition cost
O(#partitions) in boundary rotations. Fixed-size chunks per partition (Unity's
16 KiB, PAX pages) bound this at the price of "span list" iteration. The
single-span vs chunked trade-off is spike **H2**.

### 4.3 Holographic replication: same data, several shapes

"An entity appears in multiple archetypes" has two readings, and the design
supports both:

- **Logical multi-membership (default, no copies).** Through §4.1–4.2, an
  entity is simultaneously in the projection of every system it matches.
  Each system sees its data in its own contiguous arrangement, while each
  component value is stored exactly once.
- **Physical replication (opt-in).** When systems want *incompatible* layouts,
  the planner may materialise a **replica projection**: a subset of columns,
  possibly with a different order, interleaving (AoSoA) or precision. Two
  cases:
  - C1P is infeasible.
  - A system wants AoS gather while another wants SoA streaming, or a
    render-extract system wants a compacted, sorted-by-material copy.

  Precedents: Fractured Mirrors, H2O, CopyRight.

Coherency protocol (must be simple enough to reason about):

- **One home, N replicas.** Writes go to the home copy. Replicas are
  **read-only** for systems.
- **Refresh at commit points only.** Sub0Pipeline DAG edges are the sync
  points. A replica is refreshed from dirty-range or change-tick tracking (R7)
  before any system that reads it starts. This is a snapshot-consistency
  model, like double buffering.
- **Budgeted.** Replicas need a declared memory budget (R9) and are never used
  in static mode unless requested explicitly.
- Replica cost model: replicate if

  ```
  Σ over reader runs (Δ iteration cost) > refresh cost × refresh frequency + memory penalty
  ```

  This is the same shape as CopyRight's objective.

### 4.4 Inside a partition: column grouping

- **Default: pure SoA** (one array per hot column). This is what vectorised in
  the spike.
- **Co-access grouping (HYRISE, Data Morphing).** Components whose
  *accessing-system set* is identical are always touched together. They may be
  interleaved (AoSoA with lane width = SIMD width) when a cost model predicts
  fewer cache lines.
- **Precision/format per replica** (e.g. `float16` render copies) is a future
  replica property, not a home-copy concern.

### 4.5 Three answers to churn, chosen per component by the planner

| Mechanism | Precedent | Move cost | Iteration cost | When the planner picks it |
|---|---|---|---|---|
| **Side storage** (no key bit) | Bevy SparseSet, flecs DontFragment | none | only if read (random access) | Component is in no system filter |
| **Enable bit** (key bit replaced by a per-row mask) | Unity enableable components | none | mask test; block-skip when all off | Filter component with high toggle rate |
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
 │  Sub0Store  (domain-agnostic "holographic" columnar store)                │
 │  Planner: match signatures → partitions → C1P order → replicas → groups   │
 │  Storage: hot columns per partition │ side storage │ enable bits          │
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

**Naming** (your call):

- **Sub0Store** (recommended, because it says what it is).
- **Sub0Holo** (says the concept).
- **Sub0Layout** (too narrow once replication and mutation are in scope).

**Illustrative API (non-final, for discussion only).**

```cpp
// ---- Sub0Store: no ECS vocabulary -----------------------------------------
namespace sub0store {
    using Schema = sub0store::schema<Position, Velocity, Health, Rotation, Tag, Frozen>;

    constexpr auto physics  = access<Schema>().write<Position, Velocity>();          // required ⇒ filter + hot
    constexpr auto tagSweep = access<Schema>().write<Position, Velocity>().read<Tag>();
    constexpr auto plan     = make_plan<Schema>(physics, tagSweep);                  // static mode: constexpr

    store<plan, fixed_capacity<4096>> s;                                             // no heap after init
    auto row = s.insert(Position{}, Velocity{});        // staged
    s.commit();                                         // batched moves
    for (auto [pos, vel] : s.spans(physics)) { /* std::span<Position>, std::span<Velocity> */ }
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
| Query optimiser / planner | Sub0Store planner | DB physical design, Chestnut |
| Materialised view | Replica projections | Fractured Mirrors, H2O, CopyRight |
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
| Q4 | **Replica write amplification / staleness bugs.** | Read-only replicas, refresh only at barriers, budget, off by default. |
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
| **H3** Replica | Render-extract replica (sorted, compacted) | Refresh cost < saved iteration cost at 1 refresh/frame; staleness impossible by construction (tests) |
| **H4** Static mode on ESP32-P4 | constexpr plan, fixed capacity, FreeRTOS executor | Zero heap after init; code size and SRAM reported; Update2 vs StaticBitmask |
| **H5** Replan / fragmentation stress | 2^k optional-component mixes; add/remove systems at runtime | Partition count bounded as predicted; replan of 100K entities under a frame budget (target to be set) |

Recommended order: **H1 → H2 → H4 → H5 → H3.** H1 is cheap (a variant of an
existing spike) and decides whether the core idea is real.

## References

ECS
- skypjack, *EnTT* — [Entity Component System wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System), [group ownership discussion](https://github.com/skypjack/entt/discussions/829), [tips & tricks](https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/)
- *Legion* — [`World` / `WorldOptions`](https://docs.rs/legion/latest/legion/world/struct.World.html), [storage](https://docs.rs/legion/latest/legion/storage/index.html)
- *Bevy* — [`StorageType`](https://docs.rs/bevy/latest/bevy/ecs/component/enum.StorageType.html), [archetypes](https://docs.rs/bevy_ecs/latest/bevy_ecs/archetype/index.html), [change detection](https://bevy-cheatbook.github.io/programming/change-detection.html), [FilteredAccess PR](https://github.com/bevyengine/bevy/pull/5105)
- S. Mertens, *flecs* — [v4.1 (DontFragment)](https://ajmmertens.medium.com/flecs-4-1-is-out-fab4f32e36f6), [Building an ECS #2](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9), [#3 Storage in pictures](https://ajmmertens.medium.com/building-an-ecs-storage-in-pictures-642b8bfd6e04), [ECS FAQ](https://github.com/SanderMertens/ecs-faq)
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
- S. Sudhir, M. Cafarella, S. Madden, [Replicated Layout for In-Memory Database Systems (CopyRight), PVLDB 2021](https://www.vldb.org/pvldb/vol15/p984-sudhir.pdf)

PL / HPC / theory
- T. Chilimbi, B. Davidson, J. Larus, [Cache-Conscious Structure Definition, PLDI 1999](https://dl.acm.org/doi/10.1145/301618.301635)
- J. Ragan-Kelley et al., [Halide, PLDI 2013](https://dl.acm.org/doi/10.1145/2491956.2462176)
- Y. Hu et al., [Taichi, SIGGRAPH Asia 2019](https://yuanming.taichi.graphics/publication/2019-taichi/)
- B. Gruber et al., [LLAMA: The Low-Level Abstraction for Memory Access](https://arxiv.org/abs/2106.04284), [updates 2023](https://arxiv.org/abs/2302.08251)
- [Kokkos View layouts](https://kokkos.org/kokkos-core-wiki/ProgrammingGuide/View.html)
- K. Booth, G. Lueker, [Testing for the Consecutive Ones Property … Using PQ-Tree Algorithms, JCSS 1976](https://www.ic.unicamp.br/~meidanis/courses/mo640/2015s1/texts/Booth-Lueker-1976.pdf)
