# Representative workload optimization pass

This pass extends the library's benchmark applications; it does not receive
Crucible production frame, FP, replay or staging contracts. The existing
[consumer campaign](crucible.md) still owns those gates.

## Workload spectrum

| Workload | Complete timed operation | Why it is included |
|---|---|---|
| Streaming | Advance every row's position from velocity | Cheap, contiguous work exposes traversal and dispatch overhead |
| FragmentedChurn | Update all rows; toggle one tag on roughly 1/16 of stable IDs; run three tag-selected updates | Eight initial query signatures, migration after join, and real consumers of every tag query |
| StagedNeighbors | Gather coordinates by stable ID; read eight ordered neighbors per row; update velocity/position | Includes staging and bounded read-heavy work; fixed wrapped topology is synthetic, not a spatial grid |
| Ordered n-body | Gather immutable force inputs; evaluate all directed non-self interactions; update each body | Expensive quadratic kernel with an independent ordered scalar oracle |

The row workloads compare a contiguous hand-written floor, direct ECS iteration,
and native/Pipeline pools at explicit grains 64 and 1024. The n-body Compact group
compares original and compact-input kernels on the same scheduling paths. Keep
worker widths and source sizes explicit. Three widths at most are allowed per
process: the default groups fit the harness's 16-alternative limit. Larger evolving
groups would advance the shared reference again when split into comparison chunks.

Every epoch performs one complete evolving tick after two equal warmup ticks.
Construction, pool startup and final state inspection are outside timing. Streaming
and neighbor rows keep fixed membership; churn migrates only after all borrowed row
work joins. Churn's tags are used for separate velocity/position updates, not just
unconsumed partition fragmentation. The plain floor updates logical membership bits
and the same selected behaviors without ECS's query-maintenance costs.

State is checked bit for bit by stable ID after every test tick, with 1/2/4 workers,
empty/single/tail worlds, both grains, and two complete churn cycles plus a tail.
Tests also compare actual ECS query membership and include independent analytic
streaming and two-body neighbor controls. The n-body oracle is unchanged.

Counters report complete ticks and state checksums plus useful work: entity count,
eight neighbor reads per row, total membership toggles, or directed n-body
interactions. The collector rejects unequal tick counts/checksums within a paired
group. Full scalar reference cost remains visible; no startup-only microbenchmark
is presented as a whole-tick gain.

## Mechanisms under review

**Compact force snapshot.** The original n-body snapshot reads 56-byte bodies,
including velocities the force loop does not need. The alternate reads 32-byte
position/scaled-mass sources and computes `0.01 * mass` once per source per tick.
It then splits the source loop before/after the target to omit self without a
per-interaction equality branch. Each target still accumulates sources in ascending
order, with the same scalar operations and no fast-math or reassociation. The
first compact pass retained that branch; its exploratory receipt is preserved.
The full-state inspection cache remains allocated, so smaller hot force inputs do
not mean lower total allocated memory: the alternate adds 32 bytes/body of scratch.

**Retained dispatch and rejected shortcut.** This increment builds on merged
PR19 (`671eedb`): arithmetic single-partition dispatch and custom pool behavior
are retained without changes to library headers. An earlier one-lane bypass
experiment removed chunk setup and recovered inlining in cheap native traversal.
Three implementations and their slower samples are retained as rejected evidence.
Review against PR19 showed that `concurrency() == 1` does not authorize bypassing
a custom pool's dispatch contract. The shortcut is not shipped, irrespective of
its timing. Any future inline capability must be explicit and separately received.

The final measurement compares original and compact kernels against the same
merged library and executable. Both have same-pool, same-grain handwritten controls;
the independent scalar oracle remains unchanged. Shared native/Pipeline simulation
fixtures keep executor ownership/lifetime consistent across applications. No new
common runtime dependency or Pub architectural type is introduced.

## Reproduce

```sh
cmake --preset default -DSUB0ECS_WITH_PIPELINE=ON
cmake --build --preset default
ctest --preset default
ctest --preset exhaustive
python3 bench/tools/run.py --profile representative --build-dir build/default
```

The `quick` profile includes a small row-workload smoke case. Both suites still
compile/run without Pipeline in standalone C++23 (the earlier receipt used C++20). Use `ROW_SIZES`, `ROW_THREADS`,
`NBODY_SIZES`, `NBODY_THREADS` and the common `--filter` to bound individual arms.

See the [capture recipe and receipts](../../bench/results/reference/work-mode-representative/README.md)
for matched-build setup, five alternating process pairs, instruction profiles and
validation. Callgrind Ir is a dynamic guest instruction-reference count for the
whole process, including setup; it is not a retired hardware-counter measurement.
Instrumented elapsed times are excluded. Shared-host timing is diagnostic and does
not replace Crucible G1–G7 or a production caller's p95/p99 and lifetime gates.

Use these different cost profiles to select work granularity per workload. Do not
infer a global worker/grain default or a production spatial-query speedup from the
n-body kernel. Publication, bounded messaging and a real spatial grid remain useful
next applications; reuse Pub's owned types rather than inventing a router here.

## Consolidated decisions

| Finding | Retained decision | Remaining evidence |
|---|---|---|
| PR18: expensive small n-body worlds underutilize default chunks | Optional ECS-owned Pipeline bridge and explicit `RowGrain`; defaults unchanged | Actual Crucible pinned caller, FP environment, frame distributions and failure/lifetime receiving |
| PR19: single-partition descriptor setup is redundant | Arithmetic ranges, hoisted typed columns, original pool/inline behavior; all five CI jobs passed | Many tiny partitions and production attribution; diagnostic gains are not whole-frame gains |
| Pending one-lane shortcut changes generic pool behavior | Reject all three variants; preserve their raw results, including regressions | Any future coordinator bypass needs an explicit capability contract |
| N-body force input includes unused fields and a self branch | Compact staged sources and split ordered loops, same-resource compact handwritten controls | Best admissible field-split/target-SIMD bar; total memory and more workload sizes |
| Fixed-population n-body hides structural/staging costs | Add complete streaming, consumed tag-churn and staged eight-neighbor ticks | Real spatial grid, bounded publication/delivery and many-tiny-partition applications |
| Cross-library helper/type similarity does not establish identical semantics | Share benchmark pool ownership fixtures; retain library-owned adapters and Pub types | C5 Wiring/Domain comparison; no common runtime/type layer until real consumers justify it |

The current measurement table is in FINDINGS and the linked receipt. Preserve
original and compact kernels as executable controls, rather than replacing the
oracle with the candidate. Use each workload's same-resource floor before deciding
which owning library or application layer should change next.
