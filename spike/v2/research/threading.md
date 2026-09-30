# Threading: how systems scale across processors (H8)

Question: how should v2 systems use multiple processors? Lock-step (all
threads cooperate on each phase, with barriers) or independent working
(threads or engines progress without global barriers)?

Evidence comes from the real workload: the Skirmish RTS
([testbed/skirmish](../testbed/skirmish/README.md)) on a 4-core VM
(1 thread per core), plus the fusion micro-scenarios.
- Code: `Parallel` pool and `InlineRange` in
  [fusion/executors.hpp](../fusion/executors.hpp); `eachParallel` and
  `runFusedParallel` in [designs/query_partition.hpp](../designs/query_partition.hpp);
  per-worker command buffers in [testbed/skirmish/sim.hpp](../testbed/skirmish/sim.hpp).
- Numbers: [results/threading-skirmish-linux-gcc13.json](../results/threading-skirmish-linux-gcc13.json).

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

## 2. Measurements

### 2.1 Whole game (Skirmish, ms per tick, fused movement)

| Execution model | 10K units | 50K units |
|---|---:|---:|
| 1 thread | 2.25 | 11.99 |
| **First cut:** lock-step fork-join per system, sleeping pool, per-partition fused split, 4 threads | 2.68 (**0.84×**) | 7.97 (1.50×) |
| **Improved:** spin-then-park pool, grain control, chunk-level fused parallelism, 4 threads | **1.27 (1.77×)** | **5.51 (2.18×)** |
| Same, 2 threads | 1.63 (1.38×) | 7.52 (1.59×) |
| Static "owner computes" affinity, 4 threads | 2.23 (1.01×) | 8.00 (1.50×) |

All threaded variants play the **bit-identical game** at 2 and 4 threads,
checked by Skirmish conformance. ThreadSanitizer is clean on both the
game and the fusion tests.

### 2.2 Per system at 50K units (µs per tick, 1 → 4 threads)

| System | 1 T | 4 T | Scaling | Why |
|---|---:|---:|---:|---|
| movement (fused chain) | 5 465 | 1 561 | **3.50×** | Row-local, chunk-level, dynamically balanced |
| acquire (spatial search) | 4 263 | 1 349 | **3.16×** | Compute-heavy, read-only grid |
| combat | 739 | 604 | 1.22× | Dominated by serial commit (spawning projectiles) |
| grid build | 896 | 1 201 | **0.75×** | Serial merge and sort; reads positions written by other cores |
| selection / status | 21 / 5 | 146 / 134 | ≪ 1× | Tiny work; serial phases pay cross-core cache traffic |

Amdahl view: at 4 threads about 2.4 ms of the 5.5 ms tick is still serial
(grid finish, commit/apply, small systems). That serial fraction, not the
systems, caps scaling at ~2.2× here.

### 2.3 Fusion × threads (FusionFrame, fragmented)

| | 100K rows | 1M rows |
|---|---:|---:|
| Unfused, 1 thread | 236 µs | 3.03 ms |
| Fused, 1 thread | 66 µs (3.6×) | 0.91 ms (3.3×) |
| Fused + 4 threads (per-partition fork-join, sleeping pool) | 234 µs (**1.0×**) | **0.51 ms (6.0×)** |

At 1M, fusion and threads multiply. At 100K, one fork-join per partition
per group with a sleeping pool *erases* the fusion gain. Synchronisation
granularity is the real design variable.

## 3. What each experiment taught

1. **Barrier cost dominates small work.** A condition-variable wake-up
   costs tens of µs on this VM, and a tick issues ~15 dispatches. Fixes,
   all measured:
   - **spin-then-park** workers: a spin budget of 4 000 pauses beat 300,
     because back-to-back dispatches find workers awake;
   - **grain control**: fewer than 4 chunks → run inline;
   - **fusion**, which removes barriers between row-local systems.
2. **Parallelise at chunk granularity across partitions, not per
   partition.** The game has many small partitions. Forking per partition
   left most of them serial (movement 1.6× → 3.5× after the change).
3. **Dynamic balancing beats affinity here.** Chunk cost varies by an order
   of magnitude: dense unit chunks run Separation's grid scans, projectile
   chunks only integrate. Static contiguous blocks overloaded one worker
   (movement 1.56 ms dynamic vs 2.80 ms static at 4 threads). Affinity
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
| **M1 Lock-step fork-join per system** | Each system data-parallel, barrier after each | Simple, deterministic; barrier-bound for small systems (first cut 0.84× at 10K) |
| **M2 Lock-step phases, fused chains, chunk-level** | Barriers only at commit points; each worker streams chunks through the fused chain | **Recommended backbone.** 3.5× on movement, 2.18× whole tick |
| **M3 Task-parallel DAG** | Independent systems run concurrently (edges from declared Access) | Needed for the many *small* systems (status, regen, death, arrive, selection): one dispatch for all of them instead of one each. Maps onto Sub0Pipeline |
| **M4 Owner-computes affinity** | Worker owns the same chunks across systems | Loses to imbalance without a cost model; keep as an executor option (`Parallel(threads, affinity)`) |
| **M5 Pipelined frames** | Read-only consumers of frame N run while frame N+1 simulates | **Independent working where it is safe**: consumers read an immutable snapshot (the replica mechanism, research §4.3.4), so determinism holds |
| M6 Free-running threads | No barriers, eventual consistency | Rejected: non-deterministic; breaks lock-step networking, replays and conformance |

## 5. Recommendations for v2

| # | Recommendation | Rationale / evidence |
|---|---|---|
| R1 | **Phases + fused chunk-level data parallelism** as the default schedule | §2.1–2.2 |
| R2 | **Dynamic chunk claiming by default**; affinity only with cost-weighted blocks from measured per-partition cost (the same measuring planner as fusion's AutoTuner) | §3.3 |
| R3 | **Job system:** spin-then-park workers, grain control, and a *parallel planner* deciding inline vs parallel per group (an AutoTuner candidate: `{Inline, Parallel}`) | §3.1; spin 300 vs 4 000 measured |
| R4 | **Batch small independent systems into one dispatch** (M3) using Access-derived conflicts; schedule through Sub0Pipeline | Removes ~6 barriers per tick in Skirmish |
| R5 | **Shrink the serial fraction:** parallel spatial-index build (per-worker counting, parallel per-cell sort) and **sharded parallel commit** (commands bucketed by target partition, each shard applied by one worker, still Id-sorted within a shard) | ~2.4 ms of 5.5 ms is serial at 4 threads |
| R6 | **Pipeline read-only consumers across frames** (M5) from snapshots | Independent working without losing determinism |
| R7 | **Embedded (ESP32-P4: 2 HP cores + LP core):** prefer task-level parallelism (subsystems pinned per core via Sub0Pipeline's FreeRTOS executor) over fine-grained data parallelism; data-parallel only for large groups; small spin budget (power) | N is small; §2.1 shows threading loses below ~10K rows without these fixes |

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

## 7. Next spikes

| Spike | Goal | Pass criterion |
|---|---|---|
| H8b | Parallel grid build + sharded commit | Serial fraction at 50K/4 T < 1 ms; whole tick ≥ 3× |
| H8c | Task-parallel batch for small systems (one dispatch) via Access conflicts; Sub0Pipeline-backed pool | Small-system total at 4 T ≤ 1 T |
| H8d | Cross-frame pipelined consumer (render-extract snapshot) | Consumer fully overlapped; game still bit-identical |
| H8e | ESP32-P4: 2 HP cores + LP core, static plan | Speed-up and power reported; determinism holds |
| — | Cost-weighted affinity | Beats dynamic on movement at 4 T |
