# Fusion as a first-class feature: extension points

> Design note: the reasoning behind a part of the library. Measurements are kept in
> [FINDINGS.md](../FINDINGS.md), open work in [BACKLOG.md](../BACKLOG.md).

Fusion is a headline feature. This note designs it as **two extension
points**, each usable at compile time or at runtime. The fusion prototype ([fusion.md](fusion.md))
proved the mechanism; this note turns it into architecture.

- Prototype: [fusion/access.hpp](../../include/sub0ecs/fusion/access.hpp),
  [fusion/planners/](../../include/sub0ecs/fusion/planners/),
  [fusion/executors/](../../include/sub0ecs/fusion/executors/) (one header per executor), and `runFusedOn()` in
  [store/world.hpp](../../include/sub0ecs/store/world.hpp).
- Tests: [test/fusion_test.cpp](../../tests/test_fusion.cpp) (compile-time plan
  asserts, equivalence of every combination) and the Skirmish conformance
  test (the real game with four runners).
- Evidence: §6.

## 1. The pitch

> **Write small, single-purpose systems. SubzeroECS fuses them into
> single passes and runs those passes on whatever engines the target has
> (vector units, threads, coprocessors), with bit-identical results
> whichever plan or engine is chosen.**

Three properties back this up:
- **No cost for small systems.** Fusion reaches hand-merged kernel speed
  ([FINDINGS.md](../FINDINGS.md) has the measurements).
- **Plans are interchangeable.** Every legal plan produces the same state,
  so the plan can be chosen at compile time, at startup, or switched live.
- **Capability-aware placement.** Systems declare what they need; the plan
  only puts a system on an engine that can run it.

## 2. Architecture

```
 system declarations              planner (WHAT fuses)              executor (WHERE / HOW it runs)
 ─────────────────────            ───────────────────────           ──────────────────────────────
 Query<...>      membership       constexpr policies                Inline      one fused loop
 Access<Read/Write<...>>   ──►    NeverFuse / AlwaysFuse /    ──►   Tiled<N>    tiles (DMA / cache shape)
 capabilities (kDeviceSafe,       ShareColumns / DeviceAware<>      Parallel    thread pool (row chunks)
   kBitExact, kernel variants)    runtime: AutoTuner<candidates>    Offload<Dev,N> stage → launch → write back
                                         │                                 ▲
                legality filter L1–L6 ───┘   store: per-partition columns ─┘  (one pointer per component type)
```

### 2.1 Declarations (what a system states)

```cpp
struct Integrate {
    using Query  = Query<Position, Velocity>;                         // membership (required)
    using Access = fusion::Access<fusion::Write<Position>,
                                  fusion::Read<Velocity>>;            // default: all Write
    static constexpr bool kDeviceSafe = true;                         // no host pointers / host state
    void operator()(Position& p, Velocity& v) const;
};
```

- `Access` feeds the planner (sharing, conflicts) and the executors (only
  *written* columns are copied back from a device). The offload test
  verifies this: `Integrate` stages 160 KB and writes back 80 KB.
- Capabilities gate placement. In Skirmish, `Separation` reads the host
  grid through a pointer, so it is *not* device-safe, and `DeviceAware`
  isolates it automatically.
- *Planned:* `kBitExact` (the executor preserves host FP semantics),
  per-target kernel variants (`kernel<PieSimd>`, `kernel<Sycl>`), and
  `Reduce<T>` / non-row-local markers (legality L1, L3).

### 2.2 Extension point 1: the planner (what fuses)

```cpp
struct MyPlanner {
    template <typename... S>
    static constexpr std::array<bool, sizeof...(S)> plan();   // true = system i starts a new group
};
```

- **Compile time.** A plan is a `constexpr` value, so group structure lives
  in the type system. The library ships `NeverFuse`, `AlwaysFuse`,
  `ShareColumns` (the sharing rule: extend a group while the next system shares
  a column) and `DeviceAware<Base>` (never mix device-safe and host-only
  systems in one group). `fusion_test.cpp` checks the decisions with
  `static_assert`:
  - FusionFrame → {Integrate, Forces, Wrap} + {RotHealth};
  - Frame3 → no fusion;
  - Skirmish movement → Seek | Separation | StunFreeze…Bounds under
    `DeviceAware`.
- **Runtime: "constexpr enumerates, runtime selects".**
  `AutoTuner<P1, P2, ...>` instantiates each candidate plan at compile time
  and picks one live by measuring. This is FFTW's planner model: *estimate*
  (a static cost model) or *measure* (time the real thing)
  ([Frigo & Johnson 2005](https://www.fftw.org/fftw-paper-ieee.pdf)).
  Code size is bounded by the candidate list, and the dispatch is a switch
  per group, not per row.
- **Fully dynamic grouping** (arbitrary runtime plans) would need
  type-erased kernels and would lose the fused-kernel speed. It is reserved
  as a fallback for hot-reloaded or scripted systems.
- **Planners cannot break correctness.** The framework applies the legality
  filter (L1–L6: row-local, no mid-group structural change, no in-group
  reduction consumers, and so on) *before* the planner. A planner only
  chooses among legal groupings.

### 2.3 Extension point 2: the executor (where and how a group runs)

The store hands an executor, per partition:
- the row count;
- one pointer per component type in the group;
- the fused kernel `kernel(cols, count)`;
- compile-time `Info` (written columns, device-safety).

```cpp
struct MyExecutor {
    static constexpr bool kRequiresDeviceSafe = /*...*/;
    template <typename Info, typename Cols, typename Kernel>
    void run(std::size_t n, const Cols& cols, Kernel& kernel);   // each row exactly once
};
```

- **Contract.** Every row is processed exactly once. Rows may be split,
  tiled, reordered or run in parallel, because row-locality (L1) makes all
  of those equivalent. Written columns must end up back in the store.
- **Shipped:** `Inline`, `Tiled<N>`, `Parallel` (a persistent
  pool; clean under ThreadSanitizer) and `Offload<Device, N>` (stage a tile
  into device-local buffers → `Device::launch` → copy back written columns
  only).
- **Capability fallback.** If an executor requires device-safe systems and
  a group is not, `runPlanned` runs that group on the host executor. The
  plan never has to be all-or-nothing.
- **Host fallback is always available.** Every group can run `Inline`, so
  an executor may decline work (e.g. a busy coprocessor) without changing
  results.

## 3. Mapping to real targets

| Target | Engine | Executor shape | Notes |
|---|---|---|---|
| **ESP32-P4 HP cores** | PIE 128-bit SIMD ([Espressif](https://developer.espressif.com/blog/2024/12/pie-introduction/)) | `Inline`/`Tiled` with a PIE kernel variant | Auto-vectorisation or a system-provided `kernel<PieSimd>` |
| **ESP32-P4 dual HP cores** | 2 × RISC-V | `Parallel` over FreeRTOS tasks (via Sub0Pipeline's FreeRTOS executor) | Row chunks per core |
| **ESP32-P4 LP core** | low-power RISC-V coprocessor ([Zephyr ESP32-P4 features](https://docs.zephyrproject.org/latest/boards/espressif/common/soc-esp32p4-features.html)) | `Offload<LpCore, N>` for background groups while the HP cores sleep | Shared memory, so staging can be skipped; slow clock |
| **ESP32-P4 2D-DMA** | DMA engine | The copy-in/out inside `Offload`, double-buffered tiles | `Tiled` is the right shape already |
| **ESP32-P4 PPA** | pixel-processing accelerator | Domain-specific: render-extract groups only | Not general compute |
| Desktop | thread pool | `Parallel` / Sub0Pipeline workers | Implemented |
| Desktop GPU | SYCL / CUDA / Metal | `Offload<Gpu, N>` with kernel variants; unified memory avoids staging | FP semantics differ (see §5) |

## 4. Integration

- **SubzeroECS** owns declarations, the legality filter and the planner.
  **Sub0DataStore** provides partition columns (it knows nothing about
  fusion). **Executors** plug in per world or per schedule, including
  Sub0Pipeline workers.
- **Sub0Pipeline.** A fused group is one job; executors with async engines
  (DMA, coprocessor) return completion to the pipeline, and the next commit
  point waits on it.
- **Static mode (embedded).** Planner, plan and executor are all types, so
  there is zero runtime planning cost and only the chosen instantiations
  are compiled.
- **Adaptive mode (desktop).** The auto-tuner re-measures on a trigger
  (entity counts double, a system is added, a power mode changes).
  Switching is always safe (§5).

## 5. The determinism invariant, and its one caveat

**Invariant:** for executors with host-identical FP semantics, every legal
(plan, executor) pair produces bit-identical state. It is verified by:
- `fusion_test` (planners × Inline/Tiled/Parallel/Offload, plus an
  auto-tuner switching plans mid-run);
- Skirmish conformance (the real game with NeverFuse, ShareColumns+Parallel,
  DeviceAware+Offload and AutoTuned runners, all playing the identical game).

This is what makes runtime selection safe: trials, re-tuning and CPU ↔
coprocessor load-balancing never change the simulation.

**Caveat.** Real accelerators may differ in floating-point behaviour: FMA
contraction, rounding modes, denormal flushing, and approximate
transcendentals. The emulated device here is bit-exact by construction; a
GPU or DSP generally is not. The design therefore needs:
- a declared `kBitExact` capability per executor;
- systems or worlds that require bit-exactness (lockstep games, replays,
  regulated embedded logic) rejecting non-bit-exact executors at compile
  time;
- other worlds opting into tolerance explicitly.

## 6. What the measurements established

Figures per planner and executor are in [FINDINGS.md](../FINDINGS.md), section 5.
The conclusions the design rests on:

- **A static planner can get it right, and a measuring planner is the safety
  net.** `ShareColumns` fuses a frame whose systems share columns and declines one
  whose systems do not, where `AlwaysFuse` loses. Which separate passes are cheap
  differs by compiler, so no static rule is right everywhere; `AutoTuner` lands on
  or near the best plan by measuring.
- **Executors compose with fusion.** With enough rows, threads multiply the fusion
  gain. With too few, a pool that forks per partition and sleeps between jobs
  erases it, which motivated the chunk-level, spin-then-park pool in
  [threading.md](threading.md).
- **Offload's cost is data movement.** An emulated device that stages every tile
  in and out keeps only part of the fusion gain. Access-driven write-back removes
  the return traffic for read-only columns (verified in the tests). On real
  hardware, DMA overlap (double-buffered `Tiled`) and unified memory decide
  whether offload wins.
- **In the real game, capability splitting keeps Offload correct but costly.**
  Isolating Separation (which reads the spatial grid) from the movement group
  costs most of the fusion benefit. The fix is kernel variants that make
  Separation device-safe (e.g. the grid view as an input column), not a weaker
  planner.

## 7. Risks and next steps

| Risk | Mitigation |
|---|---|
| Code size: plans × subsets × executors instantiations | Instantiate only occurring subsets; bound candidate lists; measure flash on ESP32 |
| Capability splits reduce fusion (Separation isolated → movement fused less) | Kernel variants make more systems device-safe (e.g. a grid view passed as a column) |
| Async offload and commit points | Executors return completion; groups end at commit points; stage buffers are per executor |
| Debuggability | `NeverFuse` + `Inline` is always available as a build or runtime switch |

Next: auto-grouping from Access over a whole schedule (not a hand
list), the `kBitExact` capability plus a non-exact emulated device, a
Sub0Pipeline-backed `Parallel`, and an ESP32-P4 LP-core `Offload` prototype.
