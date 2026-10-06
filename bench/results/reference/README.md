# Reference captures

The captures the figures in [docs/FINDINGS.md](../../../docs/FINDINGS.md) and the
README rest on. Each was taken with `bench/tools/rotate.py`: several interleaved
samples of MSVC 19.51, clang-cl 22.1 and GCC 16.2 builds (host-tuned, no
floating-point contraction) on a Core Ultra 9 275HX, with a quiet-machine check
before every sample.

| Capture | Covers | Samples per build |
|---|---|---:|
| [`20261006-000543-rotation-bars`](CrogLegion/20261006-000543-rotation-bars/tables.md) | Every design against the hand-written references: iteration, lookup, structural change, small systems fused (`compare` profile, P-cores) | 5 |
| [`20261006-135259-rotation-exec`](CrogLegion/20261006-135259-rotation-exec/rotation.md) | Fusion at larger sizes, planners and executors, Skirmish, partition count, the [runtime-query timeline](CrogLegion/20261006-135259-rotation-exec/dynamic.md) (`standard` profile, P-cores) | 3 |
| [`20261006-140047-rotation-threads`](CrogLegion/20261006-140047-rotation-threads/rotation.md) | Thread scaling on Skirmish: 1 to 24 threads, the default pool, and the default pool pinned to the performance cores (`standard` profile, unpinned) | 5 |

In each directory: `rotation.md` (every case: median time with its spread, and
the ratio to the group's baseline with its range), `rotation.json` (every sample,
with the machine load before and after it), and `meta-<build>.json` (machine,
compiler and flags). `tables.md` is `bench/tools/tables.py` over the first
capture.

Older captures are in the repository history, not here: a figure belongs in the
tree only while the current code produces it.
