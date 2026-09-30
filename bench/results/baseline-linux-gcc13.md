# SubzeroECS v2 spike — benchmark baseline

- Host: 4 × 2100 MHz, caches: L1 D 48 KiB, L1 I 32 KiB, L2 U 2048 KiB, L3 U 266240 KiB
- Date: 2026-09-30T11:28:09+00:00  |  Build: release  |  Statistic: median of repetitions; cell = time/iteration (speed-up vs V1)
- `—` = unsupported by that design; `n/a` = not run for that design


## Create

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 35.74 µs | 30.17 µs (1.18×) | 34.01 µs (1.05×) | 34.17 µs (1.05×) | 13.83 µs (2.58×) | n/a |
| Coherent | 100,000 | 5.74 ms | 4.23 ms (1.36×) | 4.54 ms (1.27×) | 5.03 ms (1.14×) | 1.33 ms (4.31×) | n/a |
| Coherent | 1,000,000 | 76.25 ms | 43.46 ms (1.75×) | 53.74 ms (1.42×) | 54.87 ms (1.39×) | 13.90 ms (5.48×) | n/a |
| Fragmented | 1,000 | 61.72 µs | 52.39 µs (1.18×) | 71.82 µs (0.86×) | 68.68 µs (0.90×) | 14.87 µs (4.15×) | n/a |
| Fragmented | 100,000 | 13.35 ms | 7.66 ms (1.74×) | 11.74 ms (1.14×) | 9.27 ms (1.44×) | 1.49 ms (8.98×) | n/a |
| Fragmented | 1,000,000 | 168.00 ms | 89.19 ms (1.88×) | 128.62 ms (1.31×) | 104.56 ms (1.61×) | 16.03 ms (10.48×) | n/a |

## Iter1

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 0.49 µs | 0.34 µs (1.43×) | 0.34 µs (1.43×) | 0.35 µs (1.40×) | 0.63 µs (0.77×) | n/a |
| Coherent | 100,000 | 45.33 µs | 35.43 µs (1.28×) | 35.29 µs (1.28×) | 35.28 µs (1.28×) | 72.00 µs (0.63×) | n/a |
| Coherent | 1,000,000 | 601.63 µs | 566.71 µs (1.06×) | 567.63 µs (1.06×) | 609.86 µs (0.99×) | 830.34 µs (0.72×) | n/a |
| Fragmented | 1,000 | 0.46 µs ⚠ | 0.33 µs (1.40×) | 0.34 µs (1.37×) | 0.36 µs (1.30×) | 0.63 µs (0.73×) | n/a |
| Fragmented | 100,000 | 47.72 µs | 34.42 µs (1.39×) | 37.23 µs (1.28×) | 36.27 µs (1.32×) | 66.13 µs (0.72×) | n/a |
| Fragmented | 1,000,000 | 586.66 µs | 563.21 µs (1.04×) | 577.76 µs (1.02×) | 579.23 µs (1.01×) | 799.54 µs (0.73×) | n/a |

## Update2

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 2.52 µs | 2.41 µs (1.05×) | 2.30 µs (1.10×) | 0.38 µs (6.65×) | 1.61 µs (1.56×) | 0.32 µs (7.95×) |
| Coherent | 100,000 | 282.87 µs | 256.88 µs (1.10×) | 229.10 µs (1.23×) | 48.91 µs (5.78×) | 183.35 µs (1.54×) | 38.40 µs (7.37×) |
| Coherent | 1,000,000 | 3.13 ms | 2.72 ms (1.15×) | 2.52 ms (1.24×) | 685.64 µs (4.57×) | 1.95 ms (1.60×) | 667.61 µs (4.69×) |
| Fragmented | 1,000 | 2.78 µs | 2.47 µs (1.13×) | 2.29 µs (1.21×) ⚠ | 0.41 µs (6.72×) | 1.62 µs (1.71×) | 0.32 µs (8.76×) |
| Fragmented | 100,000 | 290.48 µs | 250.37 µs (1.16×) | 239.08 µs (1.21×) | 44.00 µs (6.60×) | 174.27 µs (1.67×) | 38.29 µs (7.59×) |
| Fragmented | 1,000,000 | 2.64 ms | 2.83 ms (0.93×) | 2.56 ms (1.03×) | 721.76 µs (3.66×) | 1.91 ms (1.38×) | 704.14 µs (3.75×) |

## Frame3

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 4.40 µs | 5.18 µs (0.85×) | 3.91 µs (1.12×) ⚠ | 0.77 µs (5.75×) | 2.27 µs (1.94×) | n/a |
| Fragmented | 100,000 | 496.39 µs | 544.95 µs (0.91×) | 430.62 µs (1.15×) | 112.32 µs (4.42×) | 334.13 µs (1.49×) | n/a |
| Fragmented | 1,000,000 | 4.49 ms | 8.21 ms (0.55×) | 5.01 ms (0.90×) | 1.34 ms (3.36×) | 3.89 ms (1.15×) | n/a |

## SparseQuery

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 0.60 µs | 0.23 µs (2.58×) | 0.04 µs (16.27×) | 0.04 µs (16.64×) | 0.44 µs (1.36×) | n/a |
| Fragmented | 100,000 | 90.96 µs | 23.29 µs (3.90×) | 4.58 µs (19.87×) | 0.35 µs (260.13×) ⚠ | 47.46 µs (1.92×) | n/a |
| Fragmented | 1,000,000 | 1.73 ms | 638.42 µs (2.71×) | 281.44 µs (6.15×) | 5.06 µs (342.49×) | 927.71 µs (1.87×) | n/a |

## RandomGet

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | 37.95 µs | 47.76 µs (0.79×) | 1.69 µs (22.52×) | 2.61 µs (14.52×) | 0.90 µs (41.98×) | n/a |
| Fragmented | 100,000 | 11.17 ms | 11.19 ms (1.00×) | 623.15 µs (17.92×) | 1.11 ms (10.03×) | 146.05 µs (76.47×) | n/a |
| Fragmented | 1,000,000 | 196.07 ms | 199.19 ms (0.98×) | 20.65 ms (9.49×) | 24.10 ms (8.13×) | 9.29 ms (21.10×) | n/a |

## AddRemove

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | — | 1.26 µs | 0.49 µs | 10.12 µs | 0.10 µs | n/a |
| Fragmented | 100,000 | — | 136.97 µs | 61.96 µs | 1.13 ms | 30.21 µs | n/a |
| Fragmented | 1,000,000 | — | 1.49 ms | 923.80 µs | 13.10 ms | 905.84 µs | n/a |

## DestroyCreate

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask | RawSoA |
|---|---:|---:|---:|---:|---:|---:|---:|
| Fragmented | 1,000 | — | 8.63 µs | 2.77 µs | 4.30 µs ⚠ | 0.92 µs | n/a |
| Fragmented | 100,000 | — | 1.27 ms | 442.47 µs | 462.36 µs | 119.54 µs | n/a |
| Fragmented | 1,000,000 | — | 15.74 ms | 6.91 ms | 6.86 ms | 1.63 ms | n/a |

## Memory (from Create)

Live heap bytes per entity after populate (world only, excludes handle list), and heap allocations per entity.

| Pattern | N | V1 | SortedSoA | SparseSet | Archetype | StaticBitmask |
|---|---:|---:|---:|---:|---:|---:|
| Coherent | 1,000 | 25.2 B / 0.044 | 24.9 B / 0.048 | 37.1 B / 0.071 | 42.6 B / 0.041 | 70.7 B / 0.000 |
| Coherent | 100,000 | 31.5 B / 0.001 | 31.5 B / 0.001 | 45.9 B / 0.001 | 46.2 B / 0.001 | 90.4 B / 0.000 |
| Coherent | 1,000,000 | 25.2 B / 0.000 | 25.2 B / 0.000 | 37.6 B / 0.000 | 41.0 B / 0.000 | 72.4 B / 0.000 |
| Fragmented | 1,000 | 68.3 B / 0.170 | 68.7 B / 0.182 | 117.7 B / 0.268 | 80.6 B / 0.202 | 70.7 B / 0.000 |
| Fragmented | 100,000 | 86.5 B / 0.003 | 86.5 B / 0.003 | 148.2 B / 0.004 | 90.8 B / 0.003 | 90.4 B / 0.000 |
| Fragmented | 1,000,000 | 69.2 B / 0.000 | 69.2 B / 0.000 | 119.3 B / 0.001 | 76.6 B / 0.000 | 72.4 B / 0.000 |

⚠ = coefficient of variation > 10% across repetitions (noisy host).
