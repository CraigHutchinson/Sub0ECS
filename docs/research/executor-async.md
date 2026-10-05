# Executors for accelerators: split-phase contract and column residency

Status: design note, not yet prototyped (see "H10" at the end).
Builds on [fusion-extension-points.md](fusion-extension-points.md) §2.3
(the executor extension point) and §5 (the determinism caveat).

## 1. The problem with `run()`

Today an executor has one entry point:

```cpp
template <typename Info, typename Cols, typename Kernel>
void run(std::size_t n, const Cols& cols, Kernel& kernel);   // synchronous, every row once
```

That contract fits CPU executors (`Inline`, `Tiled`, `Parallel`). It does not fit an
engine with its own memory or its own queue. `Offload<Dev, N>` shows the cost. Every
call, every frame, it stages each tile in, launches, and copies the written columns
back, blocking throughout. Real accelerators need three things this shape cannot
express:

1. **Phases that can overlap.** Transfer-in, launch and transfer-out are separate,
   asynchronous operations on every target surveyed (§2). Overlapping them
   (double-buffered tiles, copy engines running beside compute) is where offload pays.
2. **A completion handle.** A caller (Sub0Pipeline) must be able to continue and
   wait later. Otherwise a GPU or DMA engine blocks a CPU worker for its whole duration.
3. **Residency.** On a discrete GPU the win comes from columns staying on the device
   across many frames, so only what changed moves. Re-staging every tick throws the
   gain away. Residency is **not an executor-local concern**. If a column can live on
   a device, the store must know, because a host `find()` or host-side system must
   not read a stale host copy.

Point 3 is what shapes a future asynchronous API. The store API needs a coherence state per
column, and host access paths need a place to sync. A later executor cannot add
those without changing the store.

## 2. What the targets look like

Survey of primary documentation, delegated research (2026-09-30).
✔ = verified from the fetched primary source (tool-summarised, not verbatim).
~ = from background knowledge, still to verify.

| Target | Async phases | Completion primitive | Memory model and host-buffer rules |
|---|---|---|---|
| CUDA 12/13 | `cudaMallocAsync`, `cudaMemcpyAsync`, kernel launch: all stream-ordered ✔ | `cudaEvent` record/query/synchronize; `cudaStreamWaitEvent`; `cudaLaunchHostFunc` ✔. CUDA Graphs for repeated frames ~ | Discrete memory (staging); managed memory with prefetch/advise ~; zero-copy pinned host memory. **Async copies need pinned (page-locked) host buffers** ✔ |
| SYCL 2020 / oneAPI (incl. Intel iGPU) | Everything is async; `queue::submit` returns `sycl::event` ✔ | `event::wait`, `handler::depends_on`, `host_task`, `in_order` queues ✔ | Buffers/accessors: the runtime derives the dependency DAG and transfers ✔. USM `malloc_device/host/shared` is explicit ✔ |
| HIP (AMD) | As CUDA: `hipMemcpyAsync`, streams ✔ | `hipEvent*`, `hipStreamWaitEvent` ✔ | As CUDA; `hipHostMalloc` pinned, `hipMallocManaged` ✔ |
| Metal | Command buffers with blit/compute encoders; `commit` ~ | Completion handler, `waitUntilCompleted`, `MTLSharedEvent` ~ | Unified memory (`StorageModeShared`): **no transfer phase**, but ordering and visibility still need a completion ~ |
| Vulkan compute | `vkQueueSubmit`; copies are recorded commands ✔ | `VkFence`; timeline semaphores ✔ (function list ~) | Device-local + staging, or host-visible memory (zero-copy on integrated GPUs); explicit barriers for availability and visibility ✔ |
| ESP32-P4 async memcpy (GDMA AHB/AXI, 2D-DMA) | `esp_async_memcpy` queues a copy (backlog, default 4) ✔ | ISR-context callback: **no blocking calls**; give a semaphore or notify a task ✔ | Same address space. DMA-capable memory via `MALLOC_CAP_DMA`; PSRAM+DMA handled by the heap allocator ✔; cache maintenance (`esp_cache_msync`) ~ |
| ESP32-P4 LP core | No transfer: load binary, `ulp_lp_core_run` ✔ | LP core wakes the HP core by software interrupt, or shared flag polling ✔ | Shared SRAM. **The LP core reaches HP SRAM only while the HP system is awake** ✔. Columns must live in reachable memory |

Takeaways:
- Transfer-in and transfer-out **must be allowed to be no-ops** (unified memory, shared
  SRAM, zero-copy). They are optional phases, not hard-coded copies.
- A completion handle is universal. Its implementation is target-specific: CUDA
  event, `sycl::event`, fence or timeline value, ISR-signalled semaphore. For CPU
  executors it is "already complete" and must cost nothing.
- **Host-allocation requirements belong to the executor** (pinned for CUDA async
  copies, DMA-capable and cache-line aligned for ESP32 DMA) but are **satisfied by the
  store's allocator**. The contract needs a way for an executor to state them. Columns
  are already 64-byte aligned (optimisation pass), which covers alignment. Pinning
  and DMA capability are not covered yet.
- Completion callbacks may run in **interrupt context** (ESP-IDF), so the contract
  must say that `then()` continuations only signal and never do work.

## 3. Prior art for the two new concepts

- **Split-phase execution.** Every framework converges on
  `submit(work, deps) → handle`, plus poll, wait and continuation on the handle, plus a
  fixed point where results must be visible. Examples: P2300 senders/receivers ✔,
  Kokkos execution spaces and fences ~, Unity DOTS `JobHandle` with `Complete()` ~,
  Taskflow cudaFlow ~, and Bevy's extract phase ~.
- **Residency and coherence.** Kokkos `DualView` ✔ is the closest match. It tracks a
  modified flag per side; `modify<Space>()` marks a side as written; `sync<Space>()`
  copies only if the other side is newer; `need_sync<Space>()` queries. SYCL accessors
  ✔ do the same implicitly from declared access modes.

  Both map directly onto what the fusion layer already has: **declared `Access`
  (Read/Write per component per system) is exactly the information that
  `modify`/`sync` need.** The executor already copies back only written columns
  (`Info::kWritten<T>`), which is this idea at tile granularity. Residency extends it
  across frames.

## 4. Recommended contract

Keep `run()` as the synchronous composite, so every existing executor and call site
is unchanged. Add optional phases that an executor opts into:

```cpp
struct MyDeviceExecutor
{
    static constexpr bool kRequiresDeviceSafe = true;
    static constexpr bool kBitExact = false;                  // per (executor, kernel build), tested not assumed
    static constexpr HostAllocTraits kHostAlloc{ .align = 64, .pinned = true, .dmaCapable = false };

    // 1. Make the group's columns available to the engine; no-op for shared memory.
    //    Transfers only columns whose residency says the device copy is stale.
    template <typename Info, typename Cols> Staged<Cols> prepare(std::size_t n, const Cols&, Residency&);
    // 2. Enqueue the fused kernel. Non-blocking. `after` orders it behind earlier work.
    template <typename Info, typename Cols, typename K> Completion submit(Staged<Cols>&, K&, Completion after = {});
    // 3. Record which columns the device now owns (from Info::kWritten<T>). Write back
    //    eagerly (Offload today) or lazily when the host next needs them (DualView style).
    template <typename Info, typename Cols> Completion retire(Staged<Cols>&, Residency&, Completion after);
};
```

- **`Completion`** is a small move-only handle with `ready()`, `wait()`, `then(signal)`,
  and an empty "already complete" state. CPU executors return the empty state, so the
  split-phase path compiles to the current code. The store and the planner detect
  phase support with a concept and fall back to `run()` otherwise.
- **Residency lives in the store**, per partition column: `{ HostValid, DeviceValid,
  BothValid }` plus the owning executor. It costs nothing unless a device executor is
  attached, because it is a store policy chosen at compile time. The default,
  `Coherence::HostOnly`, compiles every check away.
- **Host access paths sync first.** `find<T>()`, host-side `each`, structural changes
  (`add`/`remove`/`destroy`/`commit`) and host executors check the residency of the
  columns they touch. If the device owns a column they wait on its completion and pull
  it back. **Structural changes must wait on every outstanding completion for the
  partitions they move.** This is the "commit point" of the Sub0Pipeline integration.
- **Tiling and double buffering stay inside the executor.** One fused group yields one
  `Completion` for the caller, however many tiles it used.
- **Sub0Pipeline:** one fused group is one job, and the job's completion is the
  handle. The pipeline polls it, attaches an ISR-safe signal, or, as a last resort,
  blocks a worker.
- **P2300** maps cleanly: an executor is a scheduler; `prepare → submit → retire` is a
  `let_value`/`then` chain; `Completion` is the operation state's completion. Provide
  it as an optional adapter layer. The core stays free of P2300, which is heavy,
  C++26, and poorly suited to embedded targets (~).

### What CUDA additionally asks of the API (code shape, not just runtime)

- The fused kernel must be **compiled for the device**. The TU that names the world's
  system list (where `runFused` is instantiated) must be compiled by nvcc (or through
  SYCL/HIP device compilation), and system `operator()` bodies must be device-callable.
  Plan: a `SUB0ECS_HD` macro (`__host__ __device__` under nvcc, empty elsewhere) on
  system call operators, plus the existing `kDeviceSafe` capability as the
  compile-time gate.
- The column bundle handed to a kernel is already a tuple of raw pointers, which is
  trivially copyable and device-passable. Whether `std::tuple` is usable in device code
  as-is or needs `cuda::std::tuple` is **unverified**; the executor could re-pack
  into a POD array of pointers if not.
- Pinned host memory for async copies means **the store's column allocator must be
  pluggable**: `cudaHostAlloc`, or `cudaHostRegister` on existing buffers. The
  allocator/capacity policy the FINDINGS recommend for embedded is the same hook.

## 5. Determinism

The invariant (every legal plan and executor produces bit-identical state) holds only
for executors with host-identical floating-point semantics.

- nvcc contracts to FMA by default ✔. The CUDA guide states that bitwise-identical
  host and device results are not guaranteed ✔.
- A `kBitExact` executor therefore needs a matched build (`--fmad=false` ~, no fast
  math, matching denormal mode) and no vendor transcendentals.
- Lockstep and replay worlds reject non-bit-exact executors at compile time; other
  worlds opt into tolerance explicitly (as in fusion-extension-points §5).

## 6. Open questions

| # | Question | How to resolve |
|---|---|---|
| E1 | `Completion`: type-erased small buffer, or a per-executor type? | Type-erased keeps planners and the pipeline executor-agnostic. Measure the cost in H10 |
| E2 | Residency granularity: per partition column or per column tile? | Per partition column first; tiles only if partial dirtiness shows up |
| E3 | Eager vs lazy write-back | Lazy wins when the device writes and the next reader is also on the device; eager is simpler. Make it an executor property |
| E4 | LP-core availability window (HP asleep) | Executor `available()` query; the planner's host fallback already covers declining work |
| E5 | Does residency interact with H9 promotion and relayout? | Relayout moves rows between partitions, so it is a structural change and syncs first |

## 7. Validation spike: H10 (backlog)

Add an **async emulated device** to `executors/`: a worker thread as the "engine" and
a second one as the "copy engine", with configurable transfer latency and bandwidth.
Implement `prepare/submit/retire` and `Completion`, plus store residency for
query-partition worlds. Then show:

1. **Bit-identical state:** every existing fusion and Skirmish conformance check
   passes with it.
2. **Residency pays:** bytes moved per frame drop from "all group columns in and all
   written columns out" to "columns changed on the host in, and only columns the host
   reads out". Report both.
3. **Overlap:** with double-buffered tiles, the frame time approaches
   max(transfer, compute) instead of their sum.
4. **Zero cost when unused:** host-only worlds have identical codegen and benchmark
   numbers (`Coherence::HostOnly`).

Follow-ups: a real CUDA executor behind an off-by-default CMake option (needs the
device-compiled TU, §4), and an ESP32-P4 async-memcpy `Offload` (H4/H8e).
