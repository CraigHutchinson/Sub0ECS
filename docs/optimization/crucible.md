# Crucible optimization campaign

Status: source review and measurement-tool receiving; **no performance qualification**.
Owner: integration maintainer. Reviewed 2026-10-09. This is the current proposed
campaign, not a claim that its adapters or optimizations are implemented.

## Frozen source and integration review

| Repository | Reviewed default branch | Consumed by Crucible |
|---|---|---|
| Sub0ECS | `1b1114ad2a15569ce106da429e94435912144f4f` | Same full pin |
| Sub0Pipeline | `f730c4ec2973a449c45fbf9a74595414b9bf30e1` | Same full pin |
| Sub0Pub | `504d772ecd8da6f22a22a26dd78ae3cb61c97868` | `d566c71c47cc5aeba3ed0b615052dbe6fcd91f23` |
| Crucible | `9f1716410cd223ad65237ded1779a7c3b4f39d54` | Control source |

GitHub review found no open PRs, including drafts, in the three libraries.
Crucible PR33 (`codex/production-journey`, `22125709b30b5b1df0c9133eb6c6bcda82748620`)
is open and is not a draft; it owns mission/profile/Filament work. Its metadata and
scope were reviewed, not its complete implementation. Keep that work separate.

Sub0ECS PR14 is on master. **PR15 is merged only into `fix/audit-correctness`: its
seven commits through `c88e58751938e5dc5488bdfa3dcdb5e948190eaa` are absent from
master.** Bring that existing conversion forward with integration checks, rather
than repeating the style work or closing #12. Both `style/sub0-profile` and
`fix/audit-correctness` have commits outside master. Other listed ECS branches
are ancestors of master. The closed, unmerged PR7's mechanisms are already in
master through the subsequent integration history; its state alone is not a
reason to replay it.

Pipeline retains `spike/executor-interface`; issue27 records its alternatives and
why a dispatch rewrite still needs real-workload measurement. Pub retains
`feature/api-convergence-spikes` and `feature/wiring-audit`; their names are not
acceptance receipts. Pub issue5 remains open although its comments and current
`Domain` implementation describe delivered scoped lifetime support. Verify its
remaining acceptance before administrative closure. Pub main also has a newer
configuration change (PR36) than Crucible's pin: receive migration separately.

PR17 subsequently received the existing conversion on master at
`703d97f216d413049e6de7c7999e5fb08aecca3b` after exact-head CI passed. The branch
inventory above records the initial audit, before that recovery.

## Actual consumer and hypothesis

At the frozen Crucible source, `src/simulation.cpp` uses `World::each` to gather,
commit, rebuild spatial samples and capture state. The steering path gathers and
sorts staging, rebuilds the grid, runs `scheduling::RowPartitions`, then commits
with a `lower_bound` per mobile entity. ECS is not the owner of that search or sort.
`RowPartitions` already uses a startup-built Pipeline graph and PriorityExecutor.
`runtime::BoundaryPipeline` runs the boundary/capture/publish DAG inline.
`runtime::IntentDelivery` uses Pub's scoped broker to admit bounded commands.

The first question is how much complete-tick time belongs to ECS iteration versus
sorting, spatial work, neighbor computation, commit lookup and publication. It is
not established that the ECS pool is a current bottleneck: the production path
does not invoke ECS `Parallel`, `eachParallel` or `runFusedParallel`.

An adapter may improve workload utilization by assigning independent ECS ranges
and existing proposal work to one execution budget, reducing duplicate scheduling
policy and avoiding an additional pool. Its falsifier is unchanged/worse complete
tick time or an ownership/allocation/numerical regression. The control already has
one parallel row pool, so do not report elimination of two active pools as a gain.

## Adapter first, with one defining owner

| Concern | Defining owner | Proposed integration seam |
|---|---|---|
| Entity identity, columns, query membership, structural mutation | Sub0ECS | Borrowed row work valid only until synchronous join; storage stays private |
| DAG dependencies, accepted-task accounting, execution resource | Sub0Pipeline | A coordinator-owned adapter borrows `IExecutor`; reuse the existing scheduler/pool |
| Typed delivery, scoped subscriptions and disconnect quiescence | Sub0Pub | Retain `Domain<T>` where dynamic registration is needed; evaluate `Wiring` for fixed session endpoints |
| Command queue, admission receipts, tick/commit/publish ordering | Crucible | Keep game policy and staging ownership in the production coordinator |
| Floating-point install/restore and scratch ownership | Current Crucible RowPartitions contract | Receive a second generic consumer before extracting; do not duplicate its machinery in an ECS pool |

**First adapter experiment:** implement the pool-shaped `parallelFor(items, fn)`
seam already accepted by ECS `eachParallel` and `runFusedParallel`, using a borrowed
Pipeline executor and a bounded set of lanes. Submit a small batch per group,
not one job per entity. Keep the coordinator outside the pool while it waits.
Give each concurrently active lane a unique scratch index for the entire batch;
Pipeline has no worker-index contract, so a task ordinal must not be mistaken for
an OS worker ID. Multiple items may reuse a lane only sequentially.

An executor-shaped `run<Info>(n, cols, kernel)` bridge can use `runFusedOn` unchanged,
but it joins once per partition. Measure it as the simple control; the across-all-
partitions pool seam is the first refinement for fragmented workloads. It currently
has fixed 1024/2048-row chunking and an inline threshold. A measured need to override
those is a small ECS API proposal, not justification for exposing Partition internals.

The optional bridge must not add Pipeline's C++23 requirement or dependencies to
ECS's standalone C++20 target. Begin at the consumer composition boundary. Promote
a product-neutral optional adapter only after its real caller and standalone,
omitted-dependency and cross-compiler gates pass.

### Hard adapter contract before timing

- One non-reentrant coordinator owns each world and adapter invocation. No query
  registration, component add/remove, migration, world destruction or scratch
  resize while work borrows rows. A const callback does not make captures safe.
- Catch body exceptions before crossing `IExecutor`'s nonthrowing boundary. A
  rejected submission must leave no retained callbacks. Join every already
  accepted body, completion and callable destructor before returning failure.
- Shared executor use needs explicit accounting and queue budget. A global
  `waitAll()` is only suitable when the caller exclusively owns that execution
  interval. `ScopedExecutor` scopes accounting, but allocates dispatch state;
  it does not solve pool starvation when all workers synchronously await child
  work. Flatten the DAG or submit from the coordinator; test a one-worker pool.
- Cancellation is a request, not permission to reclaim rows. Preserve FP state
  installation/restoration, unsupported fallback after join, poison handling,
  deterministic reductions and bitwise 1/2/N state/replay.
- Parallel gather cannot reuse today's shared `index++`. Use counted disjoint
  output slices with a deterministic ordering contract. Direct parallel commit
  needs proof of unique stable IDs and complete staging, plus failure semantics.
  Existing staging prevents partial authoritative writes on proposal failure;
  bypassing it is an architectural change requiring equivalent receiving.
- `Access<Read<T>>` currently does not enforce const access (#13 finding6). Do not
  automatically infer safe parallel system execution from these annotations.
  Receive enforcement and hidden-state/alias contracts first. Sharing a component
  is a fusion opportunity, not proof of independence.

## DRY and Sub0Pub architectural types

Reuse contracts at their owner before extracting a common library.

| Candidate | Decision and reason |
|---|---|
| Thread pool / dispatch / completion | Bridge to Pipeline for integrated workloads; retain ECS's standalone executor until equivalent contracts and measured costs justify replacement |
| Pub `Wiring`, `Sink`, `Domain`, typed capabilities | Use these existing public composition types; do not copy a message router into ECS or Pipeline |
| Fixed IntentDelivery endpoint | Controlled alternate `sub0::wire(sink)` arm is plausible: current session has one fixed sink and coordinator-only calls. Preserve receipt IDs, exception transport, session isolation and teardown; `Wiring` adds no synchronization and is `noexcept` |
| Asynchronous Pub-to-Pipeline forwarding | A Pub callback returning is not completion of an enqueued task. Copy into bounded owned queue storage or retain an explicit owner until Pipeline join. Never enqueue the borrowed `IntentBatch.commands` span directly |
| `Read/Write` declarations and job dependencies | ECS owns component access; map validated hazards into Pipeline edges in the integration layer. Pub receive capability says nothing about memory access or scheduling hazards |
| Type identity | ECS indices describe a World's storage layout; Pub stream IDs are user-assigned protocol identities. Keep them distinct; never use first-use ECS indices as wire IDs |
| Type option/configuration helpers | Pub `config`/`with` demonstrates a useful style, but its defaults and storage/dispatch policy are Pub-specific. ECS option tags should not import Pub policy just to reuse a fold template |
| Common lifetime/error types | Pipeline join, Pub disconnect and ECS borrow expiry have different guarantees. Preserve explicit conversions and tests; do not alias their status enums or invent a universal completion type |

Do not add a Sub0Common dependency for `Empty`, tuple traits or similarly small
helpers. A shared primitive becomes justified by two consumed implementations with
the same semantics and measurable maintenance/cost benefit, including embedded builds.

## Check, review, iterate

Use Crucible's [optimization process](https://github.com/CraigHutchinson/Crucible/blob/9f1716410cd223ad65237ded1779a7c3b4f39d54/docs/OPTIMIZATION_PROCESS.md)
and [iteration template](https://github.com/CraigHutchinson/Crucible/blob/9f1716410cd223ad65237ded1779a7c3b4f39d54/docs/optimization/iteration-template.md).
Each mechanism gets an exact-source record and an accountable owner.

1. **Check:** freeze caller, source/pins, symbols/binary hashes, toolchain/flags,
   workload, worker/partition settings and acceptance criteria. Capture the actual
   whole consumer timeline, then attribute its dominant stages with VTune.
2. **Review:** inspect useful bytes, copies/allocations, alias/lifetime/FP contracts,
   successful and missed vectorization, and final linked disassembly. Select the
   defining library only after separating its cost from the callback's work.
3. **Iterate:** one attributable mechanism at a time; correctness and joins first.
   At least three changed implementations before parking for negative performance.
   An unsafe pass stops immediately. Re-profile after a large gain or changed
   dominant stage; preserve slower runs, failed collections and raw receipts.
4. **Qualify:** at least five alternating independent A/B process pairs on the same
   machine/compiler/settings, with observed load/power/thermal state and noise.
   Profiled/diagnostic timing is excluded. Record medians/ranges and consumer gate
   distributions. Measure control/A/B/A+B; explain omitted arms.
5. **Receive:** independent review and library/platform/sanitizer gates, then pin
   the actual merged library commit and rerun the production caller. A draft PR,
   microbenchmark, source audit or compile-only adapter is not a delivered speedup.

Core workloads remain the 2K mission, 100K/400x250 and 150K/500x300 evolving worlds;
dense 64x32 stress is separate. Preserve Crucible G1-G7: bitwise/replay/bounds,
whole-frame p95/p99, pacing/input, useful parallelism and lifecycle/release gates.
Use existing frame capture/classification rather than a new acceptance timing loop.

### Ordered work packages

| ID / owner | Existing backlog connection | Next evidence and decision |
|---|---|---|
| C0 / integration | PR15, ECS #12 | Recover the stranded style conversion with exact-head gates; keep API rename migration visible |
| C1 / measurement | ECS Measurement: profile tool | Export failures and empty samples must fail closed. Then receive bounded collection/finalization supervision on Windows/Linux and actual symbol attribution |
| C2 / Crucible | Gather/commit, lookup, layout | Attribute actual ECS share; separate sort and `lower_bound` costs from ECS `find` before dispatching a library optimization |
| C3 / ECS + Pipeline | Pipeline-backed executor, width by work; Pipeline #27 | Adapter passes: existing partition executor control; across-partition bounded lanes; measured grain/inline selection. Validate small work, many tiny partitions and 1/2/N workers |
| C4 / ECS | #13 declared access, concepts, read-only access | Enforce contracts before hazard-derived DAG scheduling; no speculative graph rewrite |
| C5 / Pub + Crucible | Existing Wiring/configuration APIs | Compare fixed wiring vs existing scoped delivery with receipts/lifetime tests; combine with C3 only after separate receiving |
| C6 / ECS | Paged side pools, destroy traversal, churn/migration | Dispatch only for demonstrated consumer component vocabulary/churn. The fixed-population current caller does not establish this need |

Field-split/SIMD layout is a later arm if data movement/code generation dominates.
An ECS layout rewrite does not automatically remove Crucible's staging conversions.
Treat elimination of gather/commit as a separate epoch/borrow API proposal with
failure-atomicity proof. EnTT/flecs comparators remain useful library coverage but
do not substitute for the production caller. TSan belongs to adapter acceptance.

## This increment: receipts and limits

The profiler wrapper previously ignored report exit codes and treated an empty
CSV as a successful empty top-functions list. This increment rejects failed,
empty, zero-time, nonfinite, unsupported-schema and wholly unresolved exports;
retains both streams and command exit codes; records binary hash/source; explicitly
requests software Hotspots; and records `samples-exported` separately from manually
reviewed attribution. It also requests a collection duration.

**External collection/finalization supervision is still required.** The wrapper
is not a process-tree supervisor; `-duration` does not bound a stalled finalizer.
Do not use it unattended. The next C1 pass must implement and receive owned process
cleanup, supported stop and timeout logs on each platform before that gate closes.
Installed CLI help succeeding does not prove counters or the selected knobs work.

Local host has Python and GCC13, but no VTune, perf or CMake on PATH. No profiler
collection, C++ full suite, real hardware counter receipt, performance campaign or
native frame acceptance was run here. Python failure tests use a fake collector;
they validate orchestration/report handling only. All architecture passes remain
proposed; none count toward three measured refinement passes. Runtime sources and
Crucible dependency pins are unchanged.


## Subsequent receiving status

ECS PR16 and recovery PR17, and Crucible PR33/PR34, have now merged after their
exact-head CI gates. The inventory and local-tool limitations above describe the
initial audit. The next increment implements the optional ECS-owned Pipeline
pool bridge and an evolving ordered n-body application, with explicit row-grain
control; see [n-body](nbody.md) and the FINDINGS receipt. This advances C3's library
experiment. It does not update Crucible's runtime pins or receive production FP,
staged commit, replay, p95/p99 or lifecycle contracts. C1 supervision, C2 actual
caller attribution, C4 access enforcement and C5 Pub delivery remain open.

## Consolidated status after PR19

PR18 merged the optional Pipeline pool and explicit row grain. PR19 merged the
single-partition arithmetic dispatch specialization and same-resource handwritten
controls, with all five platform/sanitizer CI jobs passing. The
[representative follow-up](representative.md) extends C3/C6 measurement coverage
with complete streaming, tag-selected churn and staged-neighbor ticks, plus a
compact n-body force snapshot. C6 now has a synthetic churn benchmark; this does
not attribute Crucible's current runtime cost to migration.

Keep adapters in the library that owns the translated contract. Keep Pub Wiring,
Domain and protocol identities owned by Pub, Pipeline execution/join owned by
Pipeline, and ECS storage/query identity owned by ECS. Shared benchmark pool
fixtures eliminate duplicated setup without adding a common runtime layer.
A batching/borrow API or shared primitive still needs two actual consumers and
proof of matching lifetime/error semantics. These findings do not justify an
architectural type merger.

Next receiving work remains C1 bounded profiler supervision, C2 real caller
attribution and production library pins, C4 access enforcement, and C5 a meaningful
Pub delivery comparison. Real spatial workloads, field-split target SIMD and
many-tiny-partition stress remain distinct experiments. Do not combine their
hypotheses into a default scheduler/layout change.
