# SubzeroECS backlog

Open work, grouped by what it is for. What exists today, and the measurements
behind these items, are in [FINDINGS.md](FINDINGS.md). Items are not ordered
within a group.

## Performance: the gaps the benchmarks show

| Item | Evidence |
|---|---|
| **Field-split columns and SIMD-friendly iteration.** One array per field of a component, so a loop over one field is a contiguous stream and kernels can vectorise | The hand-tuned reference is 2.7–5.7× above the store and the plain hand-written loop alike, on every compiler (FINDINGS section 2). It is the only route to vector code on MSVC and GCC (section 6) |
| **Faster lookup by handle.** `find` checks the generation, loads the entity's record, then the partition's column | 0.23–0.28× of indexing an array, half the speed of following an object pointer |
| **Cheaper churn on queried components.** Batch the row moves at commit; consider enable bits for components that toggle often | TagChurn is 0.12–0.21× of a sparse set |
| **Side-pool add/remove on MSVC** | AddRemove and TagChurn are 1.4–1.5× behind GCC, with the time spread over `SidePool::emplace` / `remove` / `find`; profile first |
| Reclaim empty partitions | Must also invalidate the add/remove transition caches, which hold partition indices |
| Bulk promotion for runtime queries: sort pending entities by source partition so the moves stream | Migration is one general row move per entity (54–63 ns each) |
| MSVC fusion inlining: a same-binary A/B of `[[msvc::forceinline_calls]]` on the fused kernel body | Function-level `[[msvc::flatten]]` cost 4 GB and 60 s per file for no consistent gain |

## Measurement

| Item | Why |
|---|---|
| **Benchmark against EnTT and flecs themselves**, through the same adapter and conformance check | SparseSet and Archetype in `bench/designs/` are our implementations of their storage models, not those libraries |
| Many partitions with few entities each (2^k combinations of k optional components), and query-match invalidation there | Not measured |
| `bench/tools/profile.py`: wrap Linux `perf` for hosts without VTune; verify the `uarch` collection from an elevated prompt | Only the hotspots collection has been run |
| A ThreadSanitizer job for the executors and the parallel store paths | Checked by hand once, not in CI |

## Storage and API

| Item | Notes |
|---|---|
| Wider layout masks as a World option (`MaxTypes<128>`), for worlds with more than 64 queried or carried types | `WideMask<Words>` (an array of words: `unsigned __int128` is not on MSVC, `std::bitset` lacks bit iteration and a cheap hash). The default stays one 64-bit word. Needs a sparse form of the per-partition tables sized by the type count |
| Option tags as the configuration style (`World<Queries, Volatile<...>, MaxTypes<N>>`), order-independent | So options can be added without breaking signatures |
| A runtime `addQuery` naming a type without a layout bit terminates; make it report instead | |
| A `Carried<...>` hint, so carried types are chosen deliberately rather than by first use | For worlds near the 64-bit limit |
| Non-trivially-copyable components | Needs type-erased move and destroy in the columns; today components are `memcpy`-moved |
| Partition granularity control, a system tree, parent/child runs | [research/design-review-pre-h2.md](research/design-review-pre-h2.md) |
| Sparse-set side storage chosen per component, beyond the world-level `Volatile` list | For tags and high-churn components |
| A spatial index maintained by the store, with neighbour iteration that can be fused and offloaded | Most of a Skirmish tick is spatial neighbour work (FINDINGS section 5) |

Rejected: a macro for the type limit (`SUB0ECS_MAX_TYPES`), a process-wide ODR hazard.

## Execution

Design: [research/executor-async.md](research/executor-async.md).

| Item |
|---|
| A `Completion` handle and optional `prepare / submit / retire` phases; `run()` stays as the synchronous composite, so existing executors do not change |
| Column residency per partition (`HostValid / DeviceValid / BothValid`) behind a compile-time `Coherence` policy (default `HostOnly`, zero cost); host access and structural changes sync first |
| `HostAllocTraits` from executors: a pluggable column allocator (pinned, DMA-capable) |
| An asynchronous emulated device (engine and copy-engine threads, a latency model). Gates: bit-identical results, bytes per frame with residency, overlap, zero cost when unused |
| A `kBitExact` capability, plus a non-exact emulated device to exercise it |
| Automatic grouping from declared `Access` over a whole schedule |
| A `Parallel` executor backed by Sub0Pipeline; batch small systems into one dispatch; parallel commit |
| **Width by work, not by rows.** `Parallel` splits into chunks of at least 4,096 rows whatever the kernel costs: 3–4× at 1M entities, 0.4–0.6× at 100K, where a chunk is about 5 µs of work and waking a parked worker costs more. Choose the number of participants from measured time per row (an `AutoTuner` candidate: inline, or N ways), so a small workload stays narrow or inline |
| Tell the pool its width ahead of time from the schedule: the planner knows how many chunks a frame's widest group has, so workers could be started before the first dispatch instead of during it |
| Use efficiency cores deliberately: a second, lower-priority pool for background work (migration, snapshots) instead of leaving them idle |
| CPU topology beyond one 64-CPU processor group (Windows) and per-thread QoS on macOS; today those fall back to one class and no pinning |
| Later: a CUDA executor behind an off-by-default option; an ESP32-P4 async-memcpy `Offload` |

## Platforms and ecosystem

| Item |
|---|
| ESP32-P4: static capacity, code size, SRAM budget; a fixed-capacity policy behind the same columns |
| Extract the store's substrate as Sub0DataStore once the store API settles |
| Skirmish as its own project (see [bench/skirmish/README.md](../bench/skirmish/README.md)) |
