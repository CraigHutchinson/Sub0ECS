# Threading: how systems scale across processors

> Design note: the reasoning behind a part of the library. Measurements are kept in
> [FINDINGS.md](../FINDINGS.md), open work in [BACKLOG.md](../BACKLOG.md).

Question: how should systems use multiple processors? Lock-step (all
threads cooperate on each phase, with barriers) or independent working
(threads or engines progress without global barriers)?

The reasoning comes from a real workload, the Skirmish RTS
([bench/skirmish](../../bench/skirmish/README.md)), plus the fusion micro-scenarios.
Thread-scaling figures are in [FINDINGS.md](../FINDINGS.md), section 5.
- Code: `Parallel` pool and `InlineRange` in
  [fusion/executors/parallel.hpp](../../include/sub0ecs/fusion/executors/parallel.hpp) and [fusion/executors/inline.hpp](../../include/sub0ecs/fusion/executors/inline.hpp); `eachParallel` and
  `runFusedParallel` in [store/world.hpp](../../include/sub0ecs/store/world.hpp);
  per-worker command buffers in [bench/skirmish/sim.hpp](../../bench/skirmish/sim.hpp).

## 1. Answer in one paragraph

Make the backbone **lock-step phases with independent work inside each
phase**. Commit points stay global barriers, which is what makes the
simulation deterministic and easy to reason about. Inside a phase:
- row-local systems are **fused** into one chain;
- the chain runs **chunk by chunk across all partitions, dynamically load
  balanced**, so each worker streams its chunks through the whole chain
  with no barrier between systems.

Three things remain serial or overhead-bound, and each has a specific
remedy:
- **Barriers:** a spin-then-park job system, grain control, and batching
  small independent systems into one dispatch.
- **The serial remainder:** parallel commit and parallel spatial-index
  build.
- **Read-only consumers** (render extract, net snapshot, AI planning): run
  them *independently across frames* from immutable snapshots.

Fully barrier-free, free-running threads are rejected. They give up
determinism (lock-step networking, replays, conformance) for gains this
workload doesn't need.

## 2. What limits scaling

All threaded variants play the **bit-identical game** at every thread count,
checked by Skirmish conformance.

Per system, the pattern is stable across machines:

| System | Scales? | Why |
|---|---|---|
| movement (fused chain) | Well | Row-local, chunk-level, dynamically balanced |
| acquire (spatial search) | Well | Compute-heavy, reads an immutable grid |
| combat | Barely | Dominated by the serial commit (spawning projectiles) |
| grid build | Gets slower | Serial merge and sort; reads positions other cores just wrote |
| selection / status | Gets slower | Tiny work; serial phases pay cross-core cache traffic |

The serial fraction (grid finish, commit, the small systems), not the parallel
systems, caps whole-tick scaling. And synchronisation granularity is the real
design variable: with enough rows fusion and threads multiply, but one fork-join
per partition per group with a sleeping pool can erase the fusion gain entirely.

## 3. What each experiment taught

1. **Barrier cost dominates small work.** A condition-variable wake-up
   costs tens of microseconds, and a tick issues about 15 dispatches. Fixes:
   - **spin-then-park** workers: a long spin budget beat a short one,
     because back-to-back dispatches find workers awake;
   - **grain control**: fewer than 4 chunks → run inline;
   - **fusion**, which removes barriers between row-local systems.
2. **Parallelise at chunk granularity across partitions, not per
   partition.** The game has many small partitions. Forking per partition
   left most of them serial; chunk-level work across all partitions more than
   doubled the movement group's scaling.
3. **Dynamic balancing beats affinity here.** Chunk cost varies by an order
   of magnitude: dense unit chunks run Separation's grid scans, projectile
   chunks only integrate. Static contiguous blocks overloaded one worker.
   Affinity
   only pays with **cost-weighted** assignment; see R2.
4. **Serial phases pay for parallel phases.** Once workers have written
   columns, the main thread's serial passes (grid merge, commit) incur
   cross-core cache misses. Serial work gets *slower* when surrounded by
   parallel work, which is another reason to parallelise or shrink it.
5. **Determinism is cheap under the right rules.**
   - Per-worker command buffers are merged and sorted by the stable `Id`
     (keys are unique per system, so merge order is irrelevant).
   - Spatial cells are sorted by `Id`.
   - Integer or Id-ordered reductions.
   - No shared mutable state outside buffers.

   With these, threading changed nothing in the game's output.

## 4. Models compared

| Model | Description | Verdict |
|---|---|---|
| **M1 Lock-step fork-join per system** | Each system data-parallel, barrier after each | Simple, deterministic; barrier-bound for small systems (the first cut was slower than one thread at small sizes) |
| **M2 Lock-step phases, fused chains, chunk-level** | Barriers only at commit points; each worker streams chunks through the fused chain | **Recommended backbone.** |
| **M3 Task-parallel DAG** | Independent systems run concurrently (edges from declared Access) | Needed for the many *small* systems (status, regen, death, arrive, selection): one dispatch for all of them instead of one each. Maps onto Sub0Pipeline |
| **M4 Owner-computes affinity** | Worker owns the same chunks across systems | Loses to imbalance without a cost model; keep as an executor option (`Parallel(threads, affinity)`) |
| **M5 Pipelined frames** | Read-only consumers of frame N run while frame N+1 simulates | **Independent working where it is safe**: consumers read an immutable snapshot (the replica mechanism, [holographic-storage.md](holographic-storage.md) §4.3), so determinism holds |
| M6 Free-running threads | No barriers, eventual consistency | Rejected: non-deterministic; breaks lock-step networking, replays and conformance |

## 5. Recommendations

| # | Recommendation | Rationale / evidence |
|---|---|---|
| R1 | **Phases + fused chunk-level data parallelism** as the default schedule | §2 |
| R2 | **Dynamic chunk claiming by default**; affinity only with cost-weighted blocks from measured per-partition cost (the same measuring planner as fusion's AutoTuner) | §3.3 |
| R3 | **Job system:** spin-then-park workers, grain control, and a *parallel planner* deciding inline vs parallel per group (an AutoTuner candidate: `{Inline, Parallel}`) | §3.1 |
| R4 | **Batch small independent systems into one dispatch** (M3) using Access-derived conflicts; schedule through Sub0Pipeline | Removes about six barriers per tick in Skirmish |
| R5 | **Shrink the serial fraction:** parallel spatial-index build (per-worker counting, parallel per-cell sort) and **sharded parallel commit** (commands bucketed by target partition, each shard applied by one worker, still Id-sorted within a shard) | The serial fraction caps scaling (§2) |
| R6 | **Pipeline read-only consumers across frames** (M5) from snapshots | Independent working without losing determinism |
| R7 | **Embedded (ESP32-P4: 2 HP cores + LP core):** prefer task-level parallelism (subsystems pinned per core via Sub0Pipeline's FreeRTOS executor) over fine-grained data parallelism; data-parallel only for large groups; small spin budget (power) | N is small; threading loses at small row counts without these fixes |
| R8 | **Size the pool by the machine and grow it on demand:** default to one thread per performance core the process may use, create threads only when a dispatch needs them, wake only the workers a dispatch uses. Pinning workers to the performance cores is an option, not the default | Implemented in `Parallel` (`cpu_topology.hpp`). Scaling peaks at the performance-core count; pinning measured the same or slower where the OS already schedules for hybrid CPUs |

## 6. Integration with the Sub0 family

- **Sub0Pipeline** schedules *phases* (the DAG of fused groups and
  task-parallel batches, with edges from Access conflicts and commit
  points). Its executors (desktop worker pool, FreeRTOS per-core) host the
  worker threads. The data-parallel `parallelFor` inside a phase is the
  fusion executor seam from
  [fusion-extension-points.md](fusion-extension-points.md), so threading is
  simply another executor.
- **Sub0DataStore** provides chunk enumeration across partitions and (R5)
  sharded commit.
- **Sub0Pub** events are emitted after commit on the main thread, so
  observers never see intra-phase state.

## 7. Next steps

| Goal | Pass criterion |
|---|---|
| Parallel grid build and sharded commit | The serial fraction no longer dominates the tick |
| Task-parallel batch for small systems (one dispatch) via Access conflicts; Sub0Pipeline-backed pool | Small-system total with threads ≤ single-threaded |
| Cross-frame pipelined consumer (render-extract snapshot) | Consumer fully overlapped; game still bit-identical |
| ESP32-P4: 2 HP cores + LP core, static plan | Speed-up and power reported; determinism holds |
| Cost-weighted affinity | Beats dynamic claiming on movement |
