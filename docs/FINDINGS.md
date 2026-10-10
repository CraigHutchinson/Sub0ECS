# SubzeroECS: design and evidence

What the library does, why it is built that way, and the measurements behind each
claim. Open work is in [BACKLOG.md](BACKLOG.md); longer design notes are under
[research/](research/).

Unless a section says otherwise, numbers come from one machine (Core Ultra 9
275HX, 8 P-cores + 16 E-cores, AVX2, Windows 11), pinned to the P-cores, with MSVC
19.51, clang-cl 22.1 and GCC 16.2, floating-point contraction off. A figure
written `a–b×` is the range across those three compilers.

## 1. The design in one page

**Partitions are keyed by what systems ask for.** A World is declared with its
systems' queries. Entities that match the same set of queries share a partition:
dense, typed columns, one per component. A query iterates whole partitions, so a
system is a plain loop over arrays.

**Components fall into three groups**, decided per World type:

| Group | Stored as | Add / remove costs |
|---|---|---|
| Queried by some system | A column; part of the partition key | A row move between partitions |
| Not queried, stable (the default, "carried") | A column; part of the key | A row move |
| Declared `Volatile`, or beyond the 64 layout bits | A sparse-set side pool | A sparse-set operation; no data moves |

The carried default exists because of a measurement: storing stable unqueried
components in side pools costs noticeably more memory per entity than columns,
for nothing. Volatility has to be declared, because only the program knows which
components churn.

**Churn isolation rests on one invariant:** an entity's record tracks only the
components that can change its partition. Membership of the others lives solely in
their pool, so adding or removing one touches the pool and nothing else. Updating
the record as well costs an extra cache line per operation, several times a
sparse set's cost.

**Systems can be fused.** Systems that share columns run as one loop over each
partition; planners decide what fuses, executors where it runs. Results are
bit-identical to running the systems one after another.

**Queries can be added at runtime.** If the new query's components are already
columns, only partition matching changes. If it needs a side-stored component, the
query runs at once in a degraded mode while a bounded number of entities migrate
per frame.

## 2. Performance against references

The comparison suite is described in [bench/README.md](../bench/README.md). The
references:

- **HandWritten**: the workload as a plain loop over per-shape arrays, no ECS. The
  bar for iteration and lookup; ratios below are relative to it.
- **HandTuned**: the same with one array per field, branch-free kernels and
  explicit AVX2. The ceiling.
- **OOP**: a class hierarchy with a virtual update per object.
- **NaiveObjects**: game objects owning separately allocated components.
- **SparseSet**, **Archetype**, **SortedSoA**, **StaticBitmask**: our own
  implementations of the established ECS storage models.

Every design must reproduce the reference state bit for bit before it is
measured. Capture:
[results/reference/CrogLegion/20261006-000543-rotation-bars](../bench/results/reference/CrogLegion/20261006-000543-rotation-bars/tables.md):
five interleaved samples per compiler, all taken with the machine at 2–6% load.
Spreads over the five samples are in that file; the tables here give medians.

### Iteration and lookup (100K entities of three shapes; HandWritten = 1.00)

| Scenario | Design | MSVC | clang-cl | GCC |
|---|---|---:|---:|---:|
| Iter1 (one field) | HandTuned | 3.95× | 2.77× | 2.90× |
| | **Store** | **1.02×** | **0.96×** | **1.00×** |
| | Archetype | 1.02× | 0.98× | 1.00× |
| | SparseSet | 1.03× | 1.00× | 1.01× |
| | OOP | 0.14× | 0.10× | 0.10× |
| | NaiveObjects | 0.030× | 0.024× | 0.024× |
| Update2 (two-component physics) | HandTuned | 4.38× | 5.67× | 3.68× |
| | **Store** | **1.00×** | **1.07×** | **1.04×** |
| | Store, 3 small systems, separate passes | 0.80× | 1.32× | 0.96× |
| | Store, 3 small systems, fused | 1.03× | 1.02× | 0.99× |
| | Archetype | 1.01× | 1.06× | 0.97× |
| | SparseSet | 0.61× | 0.61× | 0.64× |
| | OOP | 0.42× | 0.60× | 0.50× |
| | NaiveObjects | 0.066× | 0.087× | 0.068× |
| Frame3 (three systems) | HandTuned | 3.55× | 4.82× | 2.67× |
| | **Store** | **1.04×** | **1.35×** | **1.02×** |
| | Store, 5 small systems, separate passes | 0.84× | 1.59× | 0.94× |
| | Store, 5 small systems, fused by plan | 0.95× | 1.39× | 1.00× |
| | Archetype | 1.02× | 1.40× | 0.86× |
| | SparseSet | 0.52× | 0.66× | 0.54× |
| | OOP | 0.45× | 0.81× | 0.55× |
| | NaiveObjects | 0.035× | 0.053× | 0.028× |
| RandomGet (lookup by handle) | HandTuned | 1.22× | 1.28× | 1.30× |
| | **Store** | **0.24×** | **0.23×** | **0.28×** |
| | Archetype | 0.17× | 0.33× | 0.20× |
| | SparseSet | 0.23× | 0.44× | 0.32× |
| | StaticBitmask | 0.47× | 1.57× | 0.58× |
| | OOP | 0.44× | 0.50× | 0.46× |
| | NaiveObjects | 0.036× | 0.057× | 0.038× |

What it shows:

1. **Iterating through the store costs nothing over a hand-written loop** (0.96–1.07×
   on Iter1 and Update2, on every compiler). Archetype tables reach the same
   speed; a sparse set does only where one component is read (Iter1), and drops to
   0.5–0.65× as soon as a system needs two.
2. **The ceiling is 2.7–5.7× above both.** HandTuned gets there by layout (one
   array per field) and by SIMD. The store shares the plain loop's limits: no
   compiler vectorises the Update2 kernel over two arrays of structs, and only
   Clang vectorises any hinted variant of it (section 6). Closing this gap means
   field-split columns, which is a storage change and is on the backlog.
3. **A class hierarchy is 1.7–10× slower, and objects owning components 12–40×
   slower,** than the store at iteration.
4. **Lookup by handle is the store's weak figure against hand-written code:**
   0.23–0.28× of indexing an array, and half the speed of following an object
   pointer. `find` checks the handle's generation, loads the entity's record, then
   the partition's column. The other table design is in the same range; the
   sparse set and the fixed-capacity design, which index dense arrays more
   directly, do better on some compilers (up to 0.44× and 1.57× on clang-cl).
5. **Clang's results differ in kind, not degree.** It leaves the hand-written
   Update2 and Frame3 loops slower than the other two compilers do (156 µs against
   98–116 µs), and it vectorises the small split systems, so on clang-cl the store's
   separate small passes beat the hand-merged loop (1.3–1.6×). Name the compiler
   with any figure.

### Small systems and fusion

The library is meant to be used with small single-purpose systems. Update2 was
split into three (Integrate, Forces, Wrap) and Frame3 into five (those, RotHealth,
Pulse); a conformance test requires both forms to equal the whole kernels bit for
bit. Relative to the hand-written merged loop:

| | MSVC | clang-cl | GCC |
|---|---:|---:|---:|
| Update2 as 3 systems, separate passes | 0.80× | 1.32× | 0.96× |
| Update2 as 3 systems, fused | 1.03× | 1.02× | 0.99× |
| Frame3 as 5 systems, separate passes | 0.84× | 1.59× | 0.94× |
| Frame3 as 5 systems, fused by `ShareColumns` | 0.95× | 1.39× | 1.00× |

- **Fused small systems run at the speed of the hand-merged loop** (0.95–1.03× on
  MSVC and GCC). That is the claim fusion has to meet, and it does.
- **What fusion recovers depends on the compiler.** Separate passes cost 16–20% on
  MSVC and 4–6% on GCC, and fusion wins that back. On Clang the separate passes are
  the faster form, because each simple pass vectorises and the merged one does
  not; fusing them gives that up. A fixed "always fuse" rule is therefore wrong
  for some compiler, which is the case for a planner that measures (`AutoTuner`).
- At 1M entities each pass is bound by memory rather than arithmetic, and fusion
  pays on every compiler: the four-system frame fused against separate passes is
  1.71× (MSVC), 1.33× (clang-cl) and 1.55× (GCC).

Two rules came out of the prototype and still hold:

1. **Fuse systems that share data.** Fusing systems on disjoint columns only
   makes the loop body bigger. `ShareColumns` fuses connected components of the
   "shares a column" graph.
2. **Fusion depends on inlining.** If a system body is not inlined into the fused
   loop, nothing is gained. GCC needs `flatten` on the fused loop; MSVC needs the
   row-call hint (section 6).

### Flexibility (100K entities; SparseSet = 1.00)

Hand-written code cannot do any of this, so the reference is the sparse set.

| Scenario | Store | Archetype | StaticBitmask | NaiveObjects |
|---|---:|---:|---:|---:|
| SparseQuery: a component 1% of entities have | 3.2–4.0× | 3.1–3.9× | 0.12–0.15× | 0.003–0.004× |
| Create | 1.8–2.6× | 1.3–1.4× | 7.7–11.5× | 0.31–0.34× |
| DestroyCreate: 10% of entities | 1.1–1.5× | 0.95–1.2× | 2.8–3.3× | 0.12–0.16× |
| AddRemove: a component no system queries | 0.84–1.23× | 0.075–0.11× | 1.7–3.2× | 0.065–0.14× |
| TagChurn: a component a system queries | 0.12–0.21× | 0.074–0.11× | 1.7–3.0× | 0.064–0.13× |

1. **Unqueried components churn at sparse-set speed** (0.84–1.23×), 8–12× faster
   than archetype tables, which move the whole row for any component. This is what
   keying partitions by queries buys.
2. **Churn on a queried component still moves data**: 0.12–0.21× of a sparse set,
   about twice as fast as archetype tables. The store removes needless moves, not
   necessary ones. `Volatile` is the tool when a queried component toggles often;
   batching moves at commit is on the backlog.
3. **A rare component drives its query.** Tagged entities live in their own
   partitions, so the query visits only them.
4. **Fixed capacity wins every structural scenario** and loses every query one
   (it scans all slots). It is the right design when the entity count is bounded
   and known, which is the embedded case; the store does not replace it there yet.

## 3. Why this storage model

The four alternatives above were built and measured before the store was. What
decided it:

- **Only table-based storage reaches a hand-written loop when a system reads
  more than one component.** A sparse set pays a lookup per entity per extra
  component (0.5–0.65× on Update2 and Frame3); sorted ids pay a merge.
- **Archetype tables pay for it on every structural change**, because every
  component is part of the table key (AddRemove 0.075–0.11× of a sparse set).
- **Keying by queries keeps the first and drops most of the second.** A component
  no system asks for does not need to partition anything.

Partition count is not a concern at normal sizes: 100K rows split over up to 512
partitions iterate within 10% of one partition on every compiler; 4,096
partitions cost 9–21% and 16,384 cost 12–26%. Granularity matters only below a
few hundred rows per partition.

## 4. Decisions inside the store, with their evidence

| Decision | Why | Effect |
|---|---|---|
| Raw 64-byte-aligned column buffers owned by the partition, grown geometrically, never zero-filled | `vector<byte>::resize` on every row push and swap-remove did bookkeeping and zero-fill | Faster structural change (TagChurn, DestroyCreate) |
| A column base pointer per component type in each partition | `find`, `each` and fusion bind a column with one load | Simpler hot paths |
| `find`: test for the column before testing membership (a column implies membership) | The membership branch sat in front of a dependent load | Faster `find` |
| Add/remove transition cache per partition, keyed by destination column mask | A hash lookup per structural move; the key includes the mask because carried components make the destination depend on more than (partition, component) | Faster queried-component churn and runtime-query migration |
| Type indices of queried and `Volatile` types are compile-time constants | A function-local static is a call on MSVC and a guard check elsewhere | RandomGet 1.6× faster on MSVC, 35–45% on clang-cl and GCC |
| `[[msvc::flatten]]` on the row call of each iteration loop | MSVC inlined the system callable but not the kernel it calls: a call per row | Update2 and Frame3 1.2–1.35× faster on MSVC |
| Fixed-size copies for 4/8/12/16-byte columns in row moves | `memcpy(stride)` is a library call per column per moved row | AddRemove 5–11% faster on all three compilers |
| No runtime limit on component types | A 64-type cap per World is a wall users would hit; types beyond the 64 layout bits are side-stored | 200 types in one World tested in both modes |

Carried forward: reclaiming empty partitions (must invalidate the transition
caches), bulk promotion sorted by source partition, batching queried-component
moves at commit.

## 5. Execution: planners, executors, threads, runtime queries

Captures: [rotation-exec](../bench/results/reference/CrogLegion/20261006-135259-rotation-exec/rotation.md)
(pinned to the P-cores, three interleaved samples per compiler) and
[rotation-threads](../bench/results/reference/CrogLegion/20261006-140047-rotation-threads/rotation.md)
(unpinned, five). Figures are MSVC / clang-cl / GCC.

**Planners and executors.** Fusion is pluggable on two axes: planners (`NeverFuse`,
`AlwaysFuse`, `ShareColumns`, `DeviceAware`, the measuring `AutoTuner`) and
executors (`Inline`, `Tiled`, `Parallel`, `Offload`). Every combination is
bit-identical to sequential execution, including an auto-tuner that switches plans
mid-run. Speed relative to `NeverFuse` (separate passes), on a frame of three
systems that share columns plus one that does not:

| Plan + executor | 100K entities | 1M entities |
|---|---:|---:|
| `ShareColumns` | 1.29× / 0.78× / 1.01× | 1.46× / 1.13× / 1.41× |
| `AlwaysFuse` | 1.32× / 0.77× / 0.96× | 1.59× / 1.26× / 1.41× |
| `AutoTuner` | 1.30× / 1.01× / 1.05× | 1.55× / 1.00× / 1.39× |
| `ShareColumns` + `Tiled<4096>` | 1.28× / 0.77× / 1.00× | 1.47× / 1.16× / 1.38× |
| `DeviceAware` + emulated `Offload` | 1.01× / 0.64× / 0.76× | 1.17× / 0.90× / 1.05× |
| `ShareColumns` + `Parallel` (8 threads) | 0.59× / 0.47× / 0.43× | 4.18× / 2.94× / 2.96× |

- **A measuring planner is the only one that never loses.** At 100K the static
  plans gain on MSVC, break even on GCC and lose over 20% on clang-cl, where the
  separate passes vectorise. `AutoTuner` takes the gain where it finds one and
  otherwise declines; on clang-cl at 1M it declined a gain the static plans did
  get, so it is safe rather than optimal.
- On a frame whose three systems share no columns, `ShareColumns` declines to fuse
  (0.99–1.04×) and `AlwaysFuse` is within 0.93–1.15×: no gain to speak of, which
  is why sharing is the rule.
- Offload through the emulated device pays for staging every tile in and out; it
  only breaks even where fusion's gain is large.
- **`Parallel` is worth 3–4× at 1M entities and is a loss at 100K.** It splits a
  partition into chunks of at least 4,096 rows whatever the kernel costs. For
  this kernel that is about 5 µs of work per chunk, less than waking a parked
  worker, and the 100K figure swings between 0.4× and 3× from sample to sample
  depending on whether the workers were still spinning. The width should follow
  the work per chunk, not the row count; that is on the backlog.

**The thread pool.** `Parallel` sizes itself to the machine: by default one thread
per performance core the process may use, which here is 8 of 24 logical CPUs. It
creates no thread until a dispatch needs one, so work that is only four ways
parallel starts three workers, and a dispatch wakes only the workers it uses.
Pinning the workers to the performance cores is an option, off by default.

The Skirmish game stays bit-identical at every thread count. Fused movement at
12,500 units per team (50K units), against one thread:

| Threads | 2 | 4 | 8 | 16 | 24 | Default (8) | Default, pinned |
|---|---:|---:|---:|---:|---:|---:|---:|
| Dynamic chunk claiming | 1.71–1.76× | 2.66–2.92× | 3.44–3.52× | 3.22–3.39× | 2.97–3.38× | 3.37–3.77× | 2.93–3.45× |
| Static "owner computes" | 1.29–1.31× | 1.91–2.04× | 2.43–2.67× | 2.72–2.93× | 2.69–2.92× | | |

- **Scaling peaks at the performance-core count** and is no better with 16 or 24
  threads, so the default lands on the best rung without being told.
- **Pinning to the performance cores is slower here** (2.93–3.45× against
  3.37–3.77×, and 1.6–1.9× against 2.2–2.3× at 10K units). With the right thread
  count Windows already puts the workers on performance cores; a mask only takes
  away its room to move them. It stays an option for platforms where the
  scheduler does worse.
- Dynamic claiming beats static "owner computes" at every count, because equal
  blocks suit equal cores and these are not.
- At 10K units the gain is 2.2–2.3× from 4 threads up. The limit is the serial
  fraction: grid build, commit and the small systems. Free-running threads were
  rejected: they break determinism.

**Runtime queries.** A query added at runtime that needs a side-stored component
runs immediately in a degraded mode (the fast path plus a sparse join over
unmigrated holders) while `migrateStep(budget)` moves a bounded number of entities
per frame. Results are bit-identical for every budget. At 1M entities with 500K to
promote ([timeline](../bench/results/reference/CrogLegion/20261006-135259-rotation-exec/dynamic.md)):

| Budget | Worst frame | Frames to finish |
|---|---:|---:|
| Everything at once | 27–35 ms | 0 |
| 65,536 entities per frame | 8.0–9.1 ms | 7 |
| 16,384 entities per frame | 5.6–6.3 ms | 30 |

Migration costs 49–64 ns per promoted entity. The degraded pass costs 5–10× the
full pass (1.1–2.6 ms against 0.22–0.32 ms), so it is a transition, not a steady
state.

**A whole game compresses the differences.** In the
[Skirmish testbed](../bench/skirmish/README.md) (an RTS with spatial queries, combat,
projectiles and deaths), time per tick relative to a sparse set at 50K units:

| Design | vs SparseSet |
|---|---:|
| Store | 1.15–1.26× |
| Store, movement fused | 1.11–1.20× |
| Store, movement on the thread pool | 1.44–1.56× |
| Archetype | 1.12–1.26× |
| StaticBitmask | 1.13–1.17× |

Single-threaded, the table-based designs are 12–26% ahead of the sparse set and
indistinguishable from one another. Most of a tick is spatial neighbour work,
which no storage layout speeds up, and fusing the movement chain does not help
here. That argues for a spatial index maintained by the store, which is on the
backlog.

## 6. Compilers

Median of five interleaved samples, µs per pass, 100K entities:

| Case | MSVC | clang-cl | GCC |
|---|---:|---:|---:|
| Iter1, hand-written | 21.5 | 15.5 | 14.7 |
| Iter1, store | 21.2 | 16.8 | 15.1 |
| Update2, hand-written | 116 | 156 | 98.2 |
| Update2, store | 119 | 145 | 97.6 |
| Update2, hand-tuned | 26.8 | 28.1 | 26.0 |
| RandomGet, store | 314 | 322 | 271 |
| AddRemove, store | 79.7 | 101 | 54.6 |
| TagChurn, store | 486 | 464 | 361 |

- Sample-to-sample spread on this laptop is 5–15%, so compare a column with the
  hand-written row in the same column rather than across columns to the last digit.
- **Hand-tuned code is compiler-independent** (26–28 µs): with explicit SIMD the
  three compilers produce the same thing. The differences elsewhere are in what
  each optimiser does with plain loops.
- MSVC is 1.2–1.5× behind GCC on plain loops and on the structural scenarios. It
  does not vectorise a one-field update of a two-field struct (vectoriser reason
  1300), which is the whole Iter1 gap.

**Source-level hints that were measured and not adopted** (Update2 as a
stand-alone loop, ns per entity):

| Variant | MSVC | clang-cl | GCC |
|---|---:|---:|---:|
| As written (two arrays of structs, branches) | 0.99 | 1.29 | 0.81 |
| `__restrict` on the column pointers | 0.90 | 1.13 | 0.95 |
| "Iterations are independent" pragma | 1.02 | 0.73 | 0.95 |
| Branches rewritten as selects | 0.92 | 0.34 | 0.93 |
| One array per field, selects, `__restrict` | 0.93 | 0.21 | 1.01 |

Only Clang turns any of them into vector code. MSVC reports the two-struct loop as
a loop-carried dependence (reason 1200) even with `__restrict` and
`#pragma loop(ivdep)`, and the select form as control flow (reason 1100). The
independence pragma is also unsafe for `each`, whose callable may carry state from
row to row. So the route to the ceiling on every compiler is the one HandTuned
takes: one array per field and explicit SIMD.

## 7. Defects found when the store became a library

Writing library-grade tests surfaced defects the benchmark conformance suite never
exercised, because it never repeats an operation on the same entity:

| Defect | Effect | Now |
|---|---|---|
| `add` of a component already held in side storage | A second dense entry, orphaning the first; the pool corrupts on later swap-removes | Overwrites in place |
| `remove` of an absent component | Undefined behaviour, or a null-pool dereference for a never-stored type | No-op |
| `destroy` / `add` / `remove` on a stale handle | `destroy` twice released the slot twice, so two live entities later shared it | No-op |
| Transition cache sentinel was the all-ones mask | An entity holding 64 column types matched the "empty" sentinel and dropped out of every query | Separate `kNoEdge` index |
| Component type ids were process-wide | The 64-type limit covered every type in the program, not one World | Numbered per World type, with no runtime limit |

Each fix has a test that fails without it. That was verified for the side-pool and
double-destroy fixes by reintroducing the old code: the model-based churn test
reported 43 mismatches, and the stale-handle test crashed.

## 8. How the measurements are taken

[bench/BENCHMARKING.md](../bench/BENCHMARKING.md) has the procedure. The rules that
exist because a number was once wrong without them:

- **Conformance before numbers**: bit-identical state across every design,
  including the hand-tuned SIMD reference.
- **One machine, named compiler.** Figures from two machines are not a
  compiler comparison.
- **Several samples, interleaved, on a quiet machine** (`rotate.py`). Other work
  on the machine and thermal drift both move a single run by more than most of
  the differences of interest.
- **The fastest design sizes a group's samples.** Sized by the slowest, a group
  that includes a very slow design times the fast ones on one cold pass.
- **No fused multiply-add and no `-ffast-math`**, so every compiler and design
  computes the same values; denormals flushed, so long runs do not time the FPU's
  slow path.
- **Profile before optimising** (`profile.py`, the compiler's vectoriser report,
  the assembly), in that order.

## 9. Not yet measured

- EnTT and flecs themselves. SparseSet and Archetype here are our implementations
  of their storage models, not those libraries.
- Embedded targets (ESP32-P4): no `-march=native`, small N, in-order cores.
- Many partitions with few entities each (2^k combinations of k optional
  components), and the cost of query-match invalidation there.
- A cost model for the `Parallel` executor's width. It splits by row count, which
  is right at 1M entities and wrong at 100K (section 5).

## Optional Pipeline adapter and ordered n-body grain

The [n-body workload](optimization/nbody.md) reuses Pipeline's execution resource
through an ECS-owned synchronous pool adapter. The default ECS chunk size exposes
only one chunk at 1,024 bodies and takes the serial path despite expensive all-pairs
work. Explicit `RowGrain<64>` exposes sixteen chunks while preserving the same
ascending-source arithmetic and bitwise state. The existing default is unchanged.

PR18 established this granularity mechanism, with bitwise conformance and a
five-pair diagnostic capture. Its instruction counts showed work overlap rather
than fewer instructions. The default remains unchanged: smaller grains did not
win at every size. The [original receipt](../bench/results/reference/work-mode-nbody/README.md)
is historical evidence for PR18; use the newer representative-workload receipt
below for current kernel comparisons. No production frame gain is claimed.

## Layered row-dispatch review (10 October 2026)

The [layered review](optimization/layered-review.md) audits architecture,
optional Pipeline integration, storage, functions and generated loops against
handwritten targets. Single-partition `eachParallel` now computes row ranges
without a per-call chunk list and resolves columns once. General fragmented
storage and pool semantics remain unchanged. The `rows` paired harness isolates
this mechanism; complete n-body ticks check whether it matters alongside real
scheduling and physics. See the review and its curated receipt for measurements,
validation and the remaining layout/consumer/platform gaps.

## Representative workloads and compact ordered kernel (after PR19)

The [consolidated review](optimization/representative.md) adds useful structural
and staging workloads, and retains merged PR19 dispatch without further library
API/header changes. Three pending one-lane shortcuts were rejected because a
`concurrency()` query does not authorize bypassing custom-pool dispatch. Their
regressions, measurements and source are retained in the receipt.

The compact n-body arm stages position/scaled mass, then splits each ascending
source reduction around self. The unchanged independent scalar oracle receives
sequential/native/Pipeline variants bit for bit. Both original and compact kernels
have handwritten controls at the same lane count, grain and join boundary.

Five alternating independent A/B process pairs, GCC13.3 portable Release
`-O3 -ffp-contract=off`, AMD EPYC9V74 virtual host with eight-CPU quota. Times
are milliseconds per complete evolving tick, median [full range] of five process
medians. Instrumented times are excluded; host power/thermals are uncontrolled.

| Bodies / arm | Original kernel, ms | Compact kernel, ms |
|---|---:|---:|
| 1,024 / HandWritten | 4.317 [4.170–4.801] | 2.992 [2.691–3.122] |
| 1,024 / ECS | 4.290 [4.147–4.537] | 2.860 [2.625–2.971] |
| 1,024 / PipelineG64x2 | 2.429 [2.333–2.632] | 1.538 [1.388–1.680] |
| 4,096 / HandWritten | 70.230 [69.247–75.685] | 47.089 [44.775–49.166] |
| 4,096 / ECS | 70.384 [70.292–74.259] | 44.578 [42.974–47.238] |
| 4,096 / PipelineG64x2 | 36.821 [35.172–41.542] | 22.755 [21.875–34.553] |

At 1,024 bodies Pipeline's median complete-tick time falls 36.7% (1.58x
throughput). The sequential ECS arm also improves, so this is a kernel/layout
change rather than just worker scaling. The two-lane compact handwritten controls
remain visible: at 4,096 bodies the matched Pipeline ECS median is 27.804 ms
versus 23.536 ms handwritten, with broad overlapping ranges. Do not claim a
universal zero-cost abstraction or combine ratios from different groups.

Callgrind records 184,491,868 → 119,168,119 total-process instruction references
at 1,024 bodies / two Pipeline lanes / five ticks, including startup and warmups:
35.4% fewer guest instructions, not a hardware retired-instruction measurement.
GCC vectorizes the split force loops with 16-byte vectors. Linked packed
sqrt/division calculates independent source contributions, then adds them in
source order. No fast-math, reduction reassociation or alias promise is introduced.
Compact ECS adds 32 bytes/body of scratch while retaining its inspection cache;
smaller hot inputs do not mean smaller total allocation.

The new complete-tick row workloads cover streaming, eight initial query signatures
with tag migration and consumed selected updates, and a staged eight-neighbor
read/update. At 65,536 rows, median microseconds per tick:

| Workload | Plain | Direct ECS | Native 2 lanes / grain1024 | Pipeline 2 lanes / grain1024 |
|---|---:|---:|---:|---:|
| Streaming | 61.1 | 61.1 | 59.9 | 85.9 |
| FragmentedChurn | 166.9 | 309.3 | 330.6 | 398.7 |
| StagedNeighbors | 1207.5 | 1183.2 | 817.8 | 811.9 |

The full ranges and every sample are in the
[receipt](../bench/results/reference/work-mode-representative/README.md).
These synthetic workloads guide future attribution: cheap streaming does not
justify Pipeline dispatch, churn still costs more than a bit-mask floor, and
bounded neighbor work can use two lanes. They do not choose a global grain/width
or establish a real spatial-grid or production Crucible gain. Library-owned
adapters remain the integration boundary; Pub types and delivery semantics remain
owned by Pub, with C1/C2/C4/C5 receiving still open.
