# H7 — system fusion (QPartHinted), speed-up vs sequential passes

- Host: 4 × 2100 MHz  |  Date: 2026-09-30T15:11:38+00:00  |  median of 5, random interleaving
- Seq = one pass per system; Fused = runFused(all); FusedGrouped = fuse only column-sharing systems; HandFused = hand-merged kernel (upper bound)

## FusionFrame

| Pattern | N | Seq | Fused | FusedGrouped | HandFused | ArchetypeSeq | SparseSetSeq |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 1.77 µs | 0.34 µs (5.14×) | 0.34 µs (5.27×) | 0.37 µs (4.75×) | 1.81 µs (0.98×) | 3.90 µs (0.45×) |
| Coherent | 100,000 | 195.94 µs | 43.96 µs (4.46×) | 45.08 µs (4.35×) | 47.89 µs (4.09×) | 213.24 µs (0.92×) | 496.66 µs (0.39×) |
| Coherent | 1,000,000 | 2.89 ms | 752.18 µs (3.84×) | 687.24 µs (4.20×) | 658.84 µs (4.38×) | 2.96 ms (0.97×) | 5.06 ms (0.57×) |
| Fragmented | 1,000 | 1.92 µs | 0.40 µs (4.74×) | 0.41 µs (4.69×) | 0.46 µs (4.20×) | 1.98 µs (0.97×) | 4.56 µs (0.42×) |
| Fragmented | 100,000 | 235.71 µs | 63.92 µs (3.69×) | 66.41 µs (3.55×) | 65.45 µs (3.60×) | 230.14 µs (1.02×) | 573.63 µs (0.41×) |
| Fragmented | 1,000,000 | 3.16 ms | 898.71 µs (3.52×) | 936.79 µs (3.37×) | 965.89 µs (3.27×) | 3.15 ms (1.00×) | 6.20 ms (0.51×) |

## Frame3Sys

| Pattern | N | Seq | Fused |
|---|---:|---:|---:|
| Fragmented | 1,000 | 0.75 µs | 1.16 µs (0.64×) |
| Fragmented | 100,000 | 119.52 µs | 147.14 µs (0.81×) |
| Fragmented | 1,000,000 | 1.37 ms | 1.60 ms (0.86×) |

⚠ = CV > 10%.
