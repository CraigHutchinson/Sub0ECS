# H1 — query-signature partitions vs Archetype (speed-up column is vs Archetype)

- Host: 4 × 2100 MHz, caches: L1 D 48 KiB, L1 I 32 KiB, L2 U 2048 KiB, L3 U 266240 KiB
- Date: 2026-09-30T14:29:45+00:00  |  Build: release  |  Statistic: median of repetitions; cell = time/iteration (speed-up vs Archetype)
- `—` = unsupported by that design; `n/a` = not run for that design


## Create

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 34.76 µs (1.11×) | 34.36 µs (1.13×) | 38.74 µs | 50.94 µs (0.76×) | 46.09 µs (0.84×) | n/a |
| Coherent | 100,000 | 4.88 ms (0.76×) | 3.38 ms (1.10×) ⚠ | 3.73 ms | 4.78 ms (0.78×) | 4.35 ms (0.86×) | n/a |
| Coherent | 1,000,000 | 61.23 ms (0.89×) | 39.88 ms (1.37×) | 54.58 ms ⚠ | 59.18 ms (0.92×) ⚠ | 64.27 ms (0.85×) ⚠ | n/a |
| Fragmented | 1,000 | 75.18 µs (0.97×) | 68.06 µs (1.07×) | 72.89 µs | 93.01 µs (0.78×) | 78.00 µs (0.93×) | n/a |
| Fragmented | 100,000 | 10.81 ms (0.63×) | 7.34 ms (0.93×) ⚠ | 6.80 ms | 9.05 ms (0.75×) | 7.73 ms (0.88×) | n/a |
| Fragmented | 1,000,000 | 155.72 ms (0.57×) | 123.78 ms (0.72×) | 89.29 ms | 135.29 ms (0.66×) | 107.68 ms (0.83×) | n/a |

## Iter1

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 0.51 µs (0.68×) | 0.33 µs (1.03×) | 0.34 µs | 0.34 µs (1.00×) | 0.35 µs (0.99×) | n/a |
| Coherent | 100,000 | 54.50 µs (0.66×) | 36.06 µs (1.00×) | 36.04 µs | 35.74 µs (1.01×) | 34.99 µs (1.03×) | n/a |
| Coherent | 1,000,000 | 622.70 µs (0.92×) | 582.16 µs (0.98×) | 573.37 µs | 594.80 µs (0.96×) | 602.49 µs (0.95×) | n/a |
| Fragmented | 1,000 | 0.50 µs (0.72×) | 0.33 µs (1.10×) | 0.36 µs | 0.36 µs (1.02×) | 0.35 µs (1.03×) | n/a |
| Fragmented | 100,000 | 54.86 µs (0.66×) | 35.23 µs (1.02×) | 36.00 µs | 34.80 µs (1.03×) | 36.01 µs (1.00×) | n/a |
| Fragmented | 1,000,000 | 581.69 µs (0.93×) | 555.68 µs (0.98×) | 542.69 µs | 595.59 µs (0.91×) | 583.78 µs (0.93×) | n/a |

## Update2

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 2.81 µs (0.13×) | 2.23 µs (0.17×) | 0.37 µs | 0.33 µs (1.13×) | 0.34 µs (1.11×) ⚠ | 0.32 µs (1.15×) |
| Coherent | 100,000 | 290.68 µs (0.17×) | 239.83 µs (0.21×) | 49.72 µs | 47.90 µs (1.04×) | 45.45 µs (1.09×) | 43.33 µs (1.15×) |
| Coherent | 1,000,000 | 3.09 ms (0.23×) | 2.64 ms (0.26×) | 696.04 µs | 696.20 µs (1.00×) | 734.67 µs (0.95×) | 742.24 µs (0.94×) |
| Fragmented | 1,000 | 2.78 µs (0.14×) | 2.23 µs (0.17×) | 0.39 µs | 0.35 µs (1.11×) | 0.35 µs (1.12×) ⚠ | 0.31 µs (1.24×) |
| Fragmented | 100,000 | 294.34 µs (0.17×) | 249.41 µs (0.20×) | 51.01 µs | 47.02 µs (1.08×) | 42.91 µs (1.19×) | 42.86 µs (1.19×) |
| Fragmented | 1,000,000 | 3.13 ms (0.22×) | 2.54 ms (0.27×) | 690.51 µs | 691.74 µs (1.00×) | 680.82 µs (1.01×) | 712.03 µs (0.97×) |

## Frame3

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 4.44 µs (0.18×) | 3.79 µs (0.21×) | 0.79 µs | 0.71 µs (1.12×) | 0.75 µs (1.05×) | n/a |
| Fragmented | 100,000 | 458.94 µs (0.25×) | 427.64 µs (0.27×) | 115.00 µs | 113.35 µs (1.01×) | 117.75 µs (0.98×) | n/a |
| Fragmented | 1,000,000 | 5.28 ms (0.27×) ⚠ | 5.76 ms (0.25×) ⚠ | 1.44 ms | 1.32 ms (1.09×) | 1.37 ms (1.05×) | n/a |

## SparseQuery

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 0.52 µs (0.07×) | 0.05 µs (0.82×) | 0.04 µs | 0.03 µs (1.40×) | 0.03 µs (1.38×) | n/a |
| Fragmented | 100,000 | 79.25 µs (0.01×) | 4.45 µs (0.10×) | 0.45 µs | 0.38 µs (1.19×) | 0.39 µs (1.15×) | n/a |
| Fragmented | 1,000,000 | 1.78 ms (0.00×) | 296.03 µs (0.02×) | 5.10 µs | 4.84 µs (1.05×) | 4.70 µs (1.09×) | n/a |

## RandomGet

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 39.04 µs (0.07×) | 1.88 µs (1.37×) | 2.57 µs | 3.76 µs (0.68×) | 4.04 µs (0.64×) | n/a |
| Fragmented | 100,000 | 11.13 ms (0.11×) | 658.98 µs (1.91×) | 1.26 ms ⚠ | 1.77 ms (0.71×) | 1.81 ms (0.69×) | n/a |
| Fragmented | 1,000,000 | 207.87 ms (0.13×) | 19.43 ms (1.36×) ⚠ | 26.38 ms ⚠ | 34.48 ms (0.77×) ⚠ | 34.61 ms (0.76×) ⚠ | n/a |

## AddRemove

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | — | 0.73 µs (14.35×) | 10.41 µs | 0.72 µs (14.47×) | 0.71 µs (14.61×) | n/a |
| Fragmented | 100,000 | — | 81.13 µs (14.72×) | 1.19 ms | 80.08 µs (14.91×) | 88.81 µs (13.45×) | n/a |
| Fragmented | 1,000,000 | — | 1.10 ms (12.71×) | 13.94 ms | 1.17 ms (11.95×) | 1.21 ms (11.48×) | n/a |

## TagChurn

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | — | 0.74 µs (14.91×) ⚠ | 11.01 µs | 11.85 µs (0.93×) | 14.63 µs (0.75×) | n/a |
| Fragmented | 100,000 | — | 67.45 µs (18.38×) | 1.24 ms | 1.26 ms (0.98×) | 1.57 ms (0.79×) | n/a |
| Fragmented | 1,000,000 | — | 1.02 ms (13.21×) | 13.53 ms | 14.19 ms (0.95×) | 20.05 ms (0.67×) ⚠ | n/a |

## DestroyCreate

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | — | 3.63 µs (1.10×) | 4.01 µs | 4.84 µs (0.83×) | 4.61 µs (0.87×) | n/a |
| Fragmented | 100,000 | — | 507.16 µs (1.01×) | 511.49 µs | 608.14 µs (0.84×) | 540.00 µs (0.95×) | n/a |
| Fragmented | 1,000,000 | — | 7.42 ms (1.10×) | 8.20 ms ⚠ | 9.38 ms (0.87×) | 7.31 ms (1.12×) ⚠ | n/a |

## Memory (from Create)

Live heap bytes per entity after populate (world only, excludes handle list), and heap allocations per entity.

| Pattern | N | V1 | SparseSet | Archetype | QueryPart | QPartHinted |
|---|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 25.2 B / 0.044 | 37.1 B / 0.071 | 42.6 B / 0.041 | 42.6 B / 0.055 | 42.6 B / 0.055 |
| Coherent | 100,000 | 31.5 B / 0.001 | 45.9 B / 0.001 | 46.2 B / 0.001 | 47.5 B / 0.001 | 47.5 B / 0.001 |
| Coherent | 1,000,000 | 25.2 B / 0.000 | 37.6 B / 0.000 | 41.0 B / 0.000 | 41.2 B / 0.000 | 41.2 B / 0.000 |
| Fragmented | 1,000 | 68.3 B / 0.170 | 117.7 B / 0.268 | 80.6 B / 0.202 | 103.7 B / 0.291 | 79.1 B / 0.229 |
| Fragmented | 100,000 | 86.5 B / 0.003 | 148.2 B / 0.004 | 90.8 B / 0.003 | 123.5 B / 0.005 | 92.1 B / 0.004 |
| Fragmented | 1,000,000 | 69.2 B / 0.000 | 119.3 B / 0.001 | 76.6 B / 0.000 | 102.0 B / 0.001 | 76.8 B / 0.000 |

⚠ = coefficient of variation > 10% across repetitions (noisy host).
