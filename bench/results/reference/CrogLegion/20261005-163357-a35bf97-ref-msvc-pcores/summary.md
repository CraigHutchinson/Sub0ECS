# Benchmark run 20261005-163357-a35bf97-ref-msvc-pcores

- baseline: ok in 1056 s
- fusion: ok in 38 s
- fusion-exec: ok in 38 s
- skirmish: ok in 100 s
- dynamic: ok in 4 s
- spans: ok in 2 s

## Create/Coherent/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 36 µs | 5.4% |  | 27.7 M |  |
| SortedSoA | 28.6 µs | 5.1% |  | 35 M |  |
| SparseSet | 35.6 µs | 3.6% |  | 28.1 M |  |
| Archetype | 42.6 µs | 1.9% |  | 23.4 M |  |
| QueryPart | 39.5 µs | 2.6% |  | 25.3 M |  |
| QPartHinted | 39 µs | 2.9% |  | 25.6 M |  |
| StaticBitmask | 10.3 µs | 1.0% |  | 97.1 M |  |
| OOP | 26.4 µs | 2.1% |  | 37.9 M |  |

## Iter1/Coherent/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 341 ns | 4.9% | baseline | 2.93 G |  |
| SortedSoA | 123 ns | 4.5% | **2.85× [2.71–2.91]** | 8.14 G |  |
| SparseSet | 121 ns | 3.0% | **2.84× [2.71–2.95]** | 8.28 G |  |
| Archetype | 130 ns | 3.0% | **2.64× [2.58–2.74]** | 7.7 G | tables=1 |
| QueryPart | 121 ns | 3.8% | **2.76× [2.62–2.88]** | 8.25 G | tables=1 |
| QPartHinted | 123 ns | 3.4% | **2.73× [2.64–2.89]** | 8.15 G | tables=1 |
| StaticBitmask | 280 ns | 4.1% | **1.20× [1.18–1.24]** | 3.57 G |  |

## Update2/Coherent/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 2.11 µs | 6.4% | baseline | 475 M |  |
| SortedSoA | 1.58 µs | 5.2% | **1.36× [1.31–1.43]** | 635 M |  |
| SparseSet | 1.98 µs | 7.0% | **1.08× [1.03–1.14]** | 506 M |  |
| Archetype | 1.41 µs | 6.4% | **1.49× [1.43–1.57]** | 710 M | tables=1 |
| QueryPart | 1.37 µs | 5.1% | **1.56× [1.48–1.61]** | 730 M | tables=1 |
| QPartHinted | 1.42 µs | 5.8% | **1.48× [1.40–1.57]** | 703 M | tables=1 |
| StaticBitmask | 1.31 µs | 5.2% | **1.61× [1.54–1.71]** | 763 M |  |
| OOP | 1.88 µs | 5.8% | **1.12× [1.08–1.17]** | 533 M |  |
| RawSoA | 1.16 µs | 6.0% | **1.83× [1.74–1.90]** | 860 M |  |

## Create/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 67.3 µs | 9.3% |  | 14.9 M |  |
| SortedSoA | 39.8 µs | 2.3% |  | 25.2 M |  |
| SparseSet | 77.1 µs | 3.2% |  | 13 M |  |
| Archetype | 55.9 µs | 3.5% |  | 17.9 M |  |
| QueryPart | 72.8 µs | 3.3% |  | 13.7 M |  |
| QPartHinted | 60.9 µs | 2.3% |  | 16.4 M |  |
| StaticBitmask | 11 µs | 0.9% |  | 90.9 M |  |
| OOP | 29 µs | 1.8% |  | 34.5 M |  |

## Iter1/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 326 ns | 3.0% | baseline | 3.06 G |  |
| SortedSoA | 118 ns | 3.1% | **2.81× [2.71–2.87]** | 8.5 G |  |
| SparseSet | 118 ns | 3.5% | **2.78× [2.73–2.85]** | 8.51 G |  |
| Archetype | 147 ns | 2.6% | **2.21× [2.17–2.27]** | 6.82 G | tables=3 |
| QueryPart | 132 ns | 3.2% | **2.49× [2.39–2.61]** | 7.59 G | tables=3 |
| QPartHinted | 132 ns | 3.3% | **2.47× [2.39–2.53]** | 7.57 G | tables=3 |
| StaticBitmask | 271 ns | 3.8% | **1.19× [1.17–1.23]** | 3.68 G |  |

## Update2/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 2.03 µs | 8.5% | baseline | 491 M |  |
| SortedSoA | 1.51 µs | 8.2% | **1.30× [1.23–1.37]** | 660 M |  |
| SparseSet | 2.03 µs | 8.0% | 0.99× [0.95–1.02] | 494 M |  |
| Archetype | 1.42 µs | 7.0% | **1.43× [1.36–1.49]** | 707 M | tables=3 |
| QueryPart | 1.38 µs | 6.9% | **1.43× [1.35–1.49]** | 724 M | tables=3 |
| QPartHinted | 1.39 µs | 10.6% | **1.43× [1.35–1.52]** | 718 M | tables=3 |
| StaticBitmask | 1.36 µs | 8.9% | **1.50× [1.45–1.56]** | 733 M |  |
| OOP | 1.97 µs | 5.5% | 1.00× [0.97–1.05] | 509 M |  |
| RawSoA | 1.13 µs | 6.6% | **1.77× [1.70–1.84]** | 887 M |  |

## Frame3/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 3.21 µs | 3.4% | baseline | 312 M |  |
| SortedSoA | 2.73 µs | 4.6% | **1.19× [1.16–1.21]** | 366 M |  |
| SparseSet | 2.96 µs | 3.3% | **1.10× [1.06–1.13]** | 338 M |  |
| Archetype | 1.77 µs | 4.8% | **1.81× [1.79–1.88]** | 564 M | tables=3 |
| QueryPart | 1.72 µs | 3.2% | **1.90× [1.84–1.93]** | 582 M | tables=3 |
| QPartHinted | 1.77 µs | 4.3% | **1.83× [1.79–1.87]** | 565 M | tables=3 |
| StaticBitmask | 2.13 µs | 4.3% | **1.54× [1.51–1.58]** | 470 M |  |

## SparseQuery/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 296 ns | 7.3% | baseline | 3.38 G |  |
| SortedSoA | 137 ns | 3.1% | **2.16× [2.00–2.26]** | 7.31 G |  |
| SparseSet | 39.1 ns | 3.9% | **7.66× [6.86–8.01]** | 25.6 G |  |
| Archetype | 34 ns | 6.2% | **8.76× [8.23–9.04]** | 29.5 G | tables=6 |
| QueryPart | 22.7 ns | 3.4% | **12.62× [11.87–13.52]** | 44.1 G | tables=6 |
| QPartHinted | 22.6 ns | 5.3% | **13.01× [12.60–13.62]** | 44.3 G | tables=6 |
| StaticBitmask | 379 ns | 2.8% | **0.78× [0.73–0.81]** | 2.64 G |  |

## RandomGet/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 29.6 µs | 9.4% | baseline | 33.8 M |  |
| SortedSoA | 30 µs | 10.9% | 0.98× [0.85–1.06] | 33.3 M |  |
| SparseSet | 2.07 µs | 3.9% | **14.09× [13.07–15.13]** | 483 M |  |
| Archetype | 2.08 µs | 2.2% | **13.96× [12.90–14.89]** | 481 M |  |
| QueryPart | 2.21 µs | 2.7% | **13.35× [12.52–14.27]** | 452 M |  |
| QPartHinted | 2.2 µs | 2.4% | **13.26× [12.28–14.14]** | 455 M |  |
| StaticBitmask | 497 ns | 4.1% | **60.10× [56.72–62.24]** | 2.01 G |  |

## AddRemove/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 974 ns | 3.4% | baseline | 1.03 G | churned=100 |
| SparseSet | 700 ns | 2.8% | **1.38× [1.36–1.41]** | 1.43 G | churned=100 |
| Archetype | 8.01 µs | 1.9% | **0.12× [0.12–0.12]** | 125 M | tables=6, churned=100 |
| QueryPart | 1.02 µs | 4.0% | **0.96× [0.93–1.00]** | 980 M | tables=3, churned=100 |
| QPartHinted | 1 µs | 4.5% | **0.97× [0.94–0.99]** | 998 M | tables=3, churned=100 |
| StaticBitmask | 97.4 ns | 2.9% | **9.96× [9.78–10.12]** | 10.3 G | churned=100 |

## TagChurn/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 914 ns | 6.2% | baseline | 1.09 G | churned=100 |
| SparseSet | 653 ns | 5.6% | **1.38× [1.33–1.41]** | 1.53 G | churned=100 |
| Archetype | 8.54 µs | 5.9% | **0.11× [0.10–0.11]** | 117 M | tables=6, churned=100 |
| QueryPart | 5.47 µs | 4.6% | **0.17× [0.16–0.17]** | 183 M | tables=6, churned=100 |
| QPartHinted | 5.85 µs | 5.0% | **0.16× [0.15–0.16]** | 171 M | tables=6, churned=100 |
| StaticBitmask | 99.4 ns | 5.4% | **9.12× [8.82–9.52]** | 10.1 G | churned=100 |

## DestroyCreate/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 5.76 µs | 3.7% | baseline | 174 M |  |
| SparseSet | 2.74 µs | 5.1% | **2.08× [2.03–2.14]** | 365 M |  |
| Archetype | 3.14 µs | 4.8% | **1.85× [1.77–1.89]** | 318 M |  |
| QueryPart | 3.33 µs | 2.8% | **1.73× [1.69–1.75]** | 301 M |  |
| QPartHinted | 3.02 µs | 4.5% | **1.92× [1.86–1.97]** | 331 M |  |
| StaticBitmask | 998 ns | 6.7% | **5.94× [5.72–6.05]** | 1 G |  |

## Create/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 4.72 ms | 3.1% |  | 21.2 M |  |
| SortedSoA | 3.4 ms | 7.3% |  | 29.4 M |  |
| SparseSet | 3.4 ms | 19.6% |  | 29.4 M |  |
| Archetype | 3.4 ms | 6.1% |  | 29.4 M |  |
| QueryPart | 4.07 ms | 6.8% |  | 24.6 M |  |
| QPartHinted | 3.49 ms | 5.7% |  | 28.7 M |  |
| StaticBitmask | 923 µs | 3.5% |  | 108 M |  |
| OOP | 2.53 ms | 2.2% |  | 39.5 M |  |

## Iter1/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 35.7 µs | 6.8% | baseline | 2.8 G |  |
| SortedSoA | 18.8 µs | 4.9% | **1.92× [1.85–2.00]** | 5.33 G |  |
| SparseSet | 18.5 µs | 4.8% | **1.97× [1.90–2.08]** | 5.4 G |  |
| Archetype | 18.8 µs | 4.6% | **1.91× [1.85–1.97]** | 5.33 G | tables=1 |
| QueryPart | 18.4 µs | 4.6% | **1.95× [1.90–2.04]** | 5.44 G | tables=1 |
| QPartHinted | 18.7 µs | 3.8% | **1.97× [1.87–2.09]** | 5.36 G | tables=1 |
| StaticBitmask | 28.7 µs | 4.7% | **1.27× [1.21–1.34]** | 3.48 G |  |

## Update2/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 216 µs | 1.8% | baseline | 462 M |  |
| SortedSoA | 172 µs | 1.7% | **1.26× [1.23–1.28]** | 581 M |  |
| SparseSet | 217 µs | 1.2% | 1.00× [0.99–1.02] | 462 M |  |
| Archetype | 141 µs | 2.4% | **1.54× [1.51–1.57]** | 711 M | tables=1 |
| QueryPart | 153 µs | 2.6% | **1.41× [1.40–1.44]** | 655 M | tables=1 |
| QPartHinted | 153 µs | 1.9% | **1.41× [1.38–1.45]** | 655 M | tables=1 |
| StaticBitmask | 147 µs | 1.9% | **1.47× [1.45–1.51]** | 681 M |  |
| OOP | 228 µs | 4.7% | **0.96× [0.92–0.99]** | 439 M |  |
| RawSoA | 113 µs | 2.4% | **1.91× [1.85–1.95]** | 881 M |  |

## Create/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 7.38 ms | 4.5% |  | 13.6 M |  |
| SortedSoA | 3.78 ms | 3.5% |  | 26.5 M |  |
| SparseSet | 8.2 ms | 19.1% |  | 12.2 M |  |
| Archetype | 5.24 ms | 8.6% |  | 19.1 M |  |
| QueryPart | 5.51 ms | 2.2% |  | 18.1 M |  |
| QPartHinted | 4.68 ms | 3.4% |  | 21.4 M |  |
| StaticBitmask | 1.08 ms | 4.0% |  | 92.9 M |  |
| OOP | 2.73 ms | 4.7% |  | 36.6 M |  |

## Iter1/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 34.8 µs | 0.7% | baseline | 2.87 G |  |
| SortedSoA | 18.1 µs | 1.7% | **1.91× [1.88–1.94]** | 5.51 G |  |
| SparseSet | 18.2 µs | 1.5% | **1.92× [1.87–1.94]** | 5.51 G |  |
| Archetype | 18.3 µs | 1.9% | **1.91× [1.88–1.93]** | 5.48 G | tables=3 |
| QueryPart | 18.1 µs | 0.7% | **1.92× [1.91–1.94]** | 5.52 G | tables=3 |
| QPartHinted | 18.2 µs | 1.3% | **1.92× [1.88–1.94]** | 5.5 G | tables=3 |
| StaticBitmask | 27.7 µs | 1.0% | **1.25× [1.25–1.27]** | 3.6 G |  |

## Update2/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 217 µs | 2.9% | baseline | 462 M |  |
| SortedSoA | 174 µs | 2.2% | **1.25× [1.22–1.26]** | 573 M |  |
| SparseSet | 221 µs | 3.9% | 0.99× [0.95–1.02] | 452 M |  |
| Archetype | 143 µs | 3.1% | **1.52× [1.47–1.56]** | 701 M | tables=3 |
| QueryPart | 142 µs | 3.0% | **1.52× [1.47–1.56]** | 703 M | tables=3 |
| QPartHinted | 143 µs | 2.5% | **1.52× [1.47–1.55]** | 697 M | tables=3 |
| StaticBitmask | 149 µs | 2.3% | **1.44× [1.40–1.49]** | 669 M |  |
| OOP | 279 µs | 6.2% | **0.81× [0.76–0.84]** | 359 M |  |
| RawSoA | 113 µs | 2.9% | **1.91× [1.85–1.97]** | 883 M |  |

## Frame3/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 365 µs | 7.1% | baseline | 274 M |  |
| SortedSoA | 313 µs | 7.0% | **1.13× [1.12–1.16]** | 320 M |  |
| SparseSet | 341 µs | 4.5% | **1.05× [1.01–1.07]** | 293 M |  |
| Archetype | 200 µs | 6.6% | **1.77× [1.72–1.84]** | 501 M | tables=3 |
| QueryPart | 202 µs | 6.0% | **1.78× [1.72–1.83]** | 496 M | tables=3 |
| QPartHinted | 201 µs | 5.0% | **1.75× [1.70–1.80]** | 498 M | tables=3 |
| StaticBitmask | 304 µs | 6.3% | **1.19× [1.16–1.23]** | 329 M |  |

## SparseQuery/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 40.3 µs | 5.9% | baseline | 2.48 G |  |
| SortedSoA | 13.9 µs | 3.8% | **2.91× [2.81–2.99]** | 7.19 G |  |
| SparseSet | 4.62 µs | 3.4% | **8.76× [8.39–8.98]** | 21.7 G |  |
| Archetype | 1.37 µs | 7.6% | **29.88× [28.53–31.57]** | 72.8 G | tables=6 |
| QueryPart | 1.35 µs | 8.4% | **30.33× [28.21–31.93]** | 74.2 G | tables=6 |
| QPartHinted | 1.32 µs | 7.2% | **30.81× [28.79–32.06]** | 75.8 G | tables=6 |
| StaticBitmask | 38.9 µs | 2.9% | 1.03× [0.97–1.05] | 2.57 G |  |

## RandomGet/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 6.53 ms | 0.8% | baseline | 15.3 M |  |
| SortedSoA | 6.65 ms | 0.7% | **0.99× [0.98–0.99]** | 15 M |  |
| SparseSet | 426 µs | 14.4% | **15.51× [13.14–16.76]** | 235 M |  |
| Archetype | 566 µs | 22.1% | **11.45× [9.65–13.11]** | 177 M |  |
| QueryPart | 562 µs | 17.7% | **11.53× [9.71–13.03]** | 178 M |  |
| QPartHinted | 588 µs | 15.9% | **11.08× [9.44–12.39]** | 170 M |  |
| StaticBitmask | 207 µs | 13.4% | **31.69× [29.45–36.57]** | 483 M |  |

## AddRemove/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 115 µs | 4.4% | baseline | 869 M | churned=1e+04 |
| SparseSet | 82.1 µs | 2.7% | **1.40× [1.36–1.46]** | 1.22 G | churned=1e+04 |
| Archetype | 801 µs | 1.2% | **0.14× [0.14–0.15]** | 125 M | tables=6, churned=1e+04 |
| QueryPart | 107 µs | 3.0% | **1.09× [1.04–1.11]** | 937 M | tables=3, churned=1e+04 |
| QPartHinted | 108 µs | 2.4% | **1.05× [1.01–1.09]** | 927 M | tables=3, churned=1e+04 |
| StaticBitmask | 28.1 µs | 4.4% | **4.05× [3.84–4.22]** | 3.56 G | churned=1e+04 |

## TagChurn/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 107 µs | 7.5% | baseline | 933 M | churned=1e+04 |
| SparseSet | 76.7 µs | 3.5% | **1.36× [1.33–1.43]** | 1.3 G | churned=1e+04 |
| Archetype | 835 µs | 2.3% | **0.13× [0.12–0.13]** | 120 M | tables=6, churned=1e+04 |
| QueryPart | 518 µs | 1.7% | **0.20× [0.20–0.21]** | 193 M | tables=6, churned=1e+04 |
| QPartHinted | 567 µs | 2.4% | **0.19× [0.18–0.19]** | 176 M | tables=6, churned=1e+04 |
| StaticBitmask | 31.1 µs | 5.8% | **3.57× [3.43–3.70]** | 3.22 G | churned=1e+04 |

## DestroyCreate/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 794 µs | 2.7% | baseline | 126 M |  |
| SparseSet | 335 µs | 3.4% | **2.38× [2.32–2.43]** | 298 M |  |
| Archetype | 351 µs | 3.4% | **2.25× [2.20–2.30]** | 285 M |  |
| QueryPart | 396 µs | 4.4% | **2.02× [1.96–2.07]** | 253 M |  |
| QPartHinted | 341 µs | 2.5% | **2.32× [2.29–2.40]** | 294 M |  |
| StaticBitmask | 122 µs | 4.5% | **6.56× [6.43–6.64]** | 822 M |  |

## Create/Coherent/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 50.7 ms | 0.9% |  | 19.7 M |  |
| SortedSoA | 33.7 ms | 1.7% |  | 29.6 M |  |
| SparseSet | 43 ms | 2.3% |  | 23.3 M |  |
| Archetype | 45.7 ms | 1.1% |  | 21.9 M |  |
| QueryPart | 45.4 ms | 3.5% |  | 22 M |  |
| QPartHinted | 41.4 ms | 3.0% |  | 24.1 M |  |
| StaticBitmask | 9.93 ms | 2.3% |  | 101 M |  |
| OOP | 32.9 ms | 4.2% |  | 30.4 M |  |

## Iter1/Coherent/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 469 µs | 4.9% | baseline | 2.13 G |  |
| SortedSoA | 280 µs | 4.8% | **1.70× [1.66–1.77]** | 3.58 G |  |
| SparseSet | 275 µs | 3.8% | **1.70× [1.65–1.77]** | 3.63 G |  |
| Archetype | 276 µs | 5.0% | **1.72× [1.64–1.80]** | 3.62 G | tables=1 |
| QueryPart | 275 µs | 4.9% | **1.73× [1.66–1.77]** | 3.63 G | tables=1 |
| QPartHinted | 272 µs | 3.8% | **1.71× [1.68–1.81]** | 3.67 G | tables=1 |
| StaticBitmask | 398 µs | 3.7% | **1.20× [1.16–1.25]** | 2.51 G |  |

## Update2/Coherent/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 2.26 ms | 2.4% | baseline | 442 M |  |
| SortedSoA | 1.89 ms | 2.1% | **1.21× [1.19–1.24]** | 530 M |  |
| SparseSet | 2.24 ms | 2.3% | 1.00× [0.98–1.03] | 446 M |  |
| Archetype | 1.69 ms | 2.3% | **1.33× [1.31–1.37]** | 591 M | tables=1 |
| QueryPart | 1.7 ms | 3.6% | **1.34× [1.30–1.37]** | 590 M | tables=1 |
| QPartHinted | 1.72 ms | 2.8% | **1.32× [1.29–1.35]** | 583 M | tables=1 |
| StaticBitmask | 1.68 ms | 2.3% | **1.34× [1.30–1.39]** | 596 M |  |
| OOP | 3.93 ms | 12.3% | **0.57× [0.53–0.63]** | 254 M |  |
| RawSoA | 1.5 ms | 2.7% | **1.50× [1.46–1.54]** | 667 M |  |

## Create/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 110 ms | 2.5% |  | 9.08 M |  |
| SortedSoA | 63.8 ms | 2.8% |  | 15.7 M |  |
| SparseSet | 106 ms | 3.2% |  | 9.45 M |  |
| Archetype | 69.5 ms | 4.6% |  | 14.4 M |  |
| QueryPart | 84.5 ms | 5.1% |  | 11.8 M |  |
| QPartHinted | 67.1 ms | 5.5% |  | 14.9 M |  |
| StaticBitmask | 12.5 ms | 6.6% |  | 79.8 M |  |
| OOP | 40.4 ms | 10.5% |  | 24.8 M |  |

## Iter1/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 546 µs | 11.7% | baseline | 1.83 G |  |
| SortedSoA | 315 µs | 13.0% | **1.76× [1.65–1.91]** | 3.17 G |  |
| SparseSet | 323 µs | 11.4% | **1.70× [1.61–1.79]** | 3.09 G |  |
| Archetype | 309 µs | 10.8% | **1.76× [1.67–1.86]** | 3.24 G | tables=3 |
| QueryPart | 316 µs | 12.5% | **1.75× [1.65–1.87]** | 3.17 G | tables=3 |
| QPartHinted | 304 µs | 8.2% | **1.77× [1.68–1.86]** | 3.29 G | tables=3 |
| StaticBitmask | 453 µs | 12.4% | **1.19× [1.12–1.25]** | 2.21 G |  |

## Update2/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 2.45 ms | 9.3% | baseline | 408 M |  |
| SortedSoA | 2.13 ms | 11.1% | **1.17× [1.08–1.28]** | 471 M |  |
| SparseSet | 2.56 ms | 6.5% | 0.94× [0.88–1.06] | 390 M |  |
| Archetype | 1.99 ms | 12.8% | **1.28× [1.23–1.36]** | 502 M | tables=3 |
| QueryPart | 2.09 ms | 13.3% | **1.17× [1.07–1.28]** | 479 M | tables=3 |
| QPartHinted | 1.97 ms | 9.8% | **1.28× [1.15–1.39]** | 508 M | tables=3 |
| StaticBitmask | 1.96 ms | 10.5% | **1.25× [1.17–1.35]** | 511 M |  |
| OOP | 7.92 ms | 3.4% | **0.31× [0.29–0.34]** | 126 M |  |
| RawSoA | 2.05 ms | 6.3% | **1.21× [1.14–1.26]** | 488 M |  |

## Frame3/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 4.2 ms | 5.8% | baseline | 238 M |  |
| SortedSoA | 4.79 ms | 6.7% | **0.87× [0.83–0.91]** | 209 M |  |
| SparseSet | 4.27 ms | 9.9% | 0.97× [0.91–1.04] | 234 M |  |
| Archetype | 2.9 ms | 6.4% | **1.49× [1.39–1.56]** | 344 M | tables=3 |
| QueryPart | 2.87 ms | 6.6% | **1.47× [1.38–1.54]** | 349 M | tables=3 |
| QPartHinted | 3.12 ms | 11.2% | **1.38× [1.30–1.46]** | 320 M | tables=3 |
| StaticBitmask | 4.34 ms | 6.9% | 1.00× [0.94–1.07] | 230 M |  |

## SparseQuery/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 630 µs | 8.2% | baseline | 1.59 G |  |
| SortedSoA | 322 µs | 9.8% | **1.95× [1.83–2.07]** | 3.1 G |  |
| SparseSet | 201 µs | 8.6% | **3.03× [2.82–3.26]** | 4.97 G |  |
| Archetype | 13.9 µs | 9.2% | **44.07× [39.88–47.46]** | 72 G | tables=6 |
| QueryPart | 14.6 µs | 11.6% | **42.26× [39.37–46.27]** | 68.3 G | tables=6 |
| QPartHinted | 17 µs | 16.9% | **35.41× [32.72–44.05]** | 58.9 G | tables=6 |
| StaticBitmask | 485 µs | 8.0% | **1.26× [1.18–1.32]** | 2.06 G |  |

## RandomGet/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 103 ms | 2.1% | baseline | 9.7 M |  |
| SortedSoA | 105 ms | 2.8% | **0.97× [0.95–1.00]** | 9.52 M |  |
| SparseSet | 11.4 ms | 7.0% | **8.97× [8.43–9.31]** | 87.4 M |  |
| Archetype | 19.8 ms | 8.3% | **5.29× [4.99–5.61]** | 50.6 M |  |
| QueryPart | 24.3 ms | 8.0% | **4.26× [4.05–4.47]** | 41.1 M |  |
| QPartHinted | 23.9 ms | 10.0% | **4.38× [4.01–4.60]** | 41.8 M |  |
| StaticBitmask | 4.97 ms | 3.1% | **20.80× [20.23–21.13]** | 201 M |  |

## AddRemove/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 1.66 ms | 5.7% | baseline | 602 M | churned=1e+05 |
| SparseSet | 1.28 ms | 3.8% | **1.27× [1.23–1.31]** | 779 M | churned=1e+05 |
| Archetype | 9.15 ms | 5.5% | **0.17× [0.17–0.18]** | 109 M | tables=6, churned=1e+05 |
| QueryPart | 1.64 ms | 3.9% | 0.99× [0.97–1.03] | 611 M | tables=3, churned=1e+05 |
| QPartHinted | 1.78 ms | 3.9% | **0.92× [0.89–0.93]** | 561 M | tables=3, churned=1e+05 |
| StaticBitmask | 900 µs | 1.8% | **1.83× [1.78–1.92]** | 1.11 G | churned=1e+05 |

## TagChurn/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 1.57 ms | 4.2% | baseline | 636 M | churned=1e+05 |
| SparseSet | 1.23 ms | 3.4% | **1.30× [1.25–1.32]** | 811 M | churned=1e+05 |
| Archetype | 8.98 ms | 3.2% | **0.17× [0.17–0.18]** | 111 M | tables=6, churned=1e+05 |
| QueryPart | 5.98 ms | 3.8% | **0.26× [0.25–0.27]** | 167 M | tables=6, churned=1e+05 |
| QPartHinted | 6.66 ms | 5.4% | **0.24× [0.23–0.24]** | 150 M | tables=6, churned=1e+05 |
| StaticBitmask | 936 µs | 2.6% | **1.71× [1.66–1.77]** | 1.07 G | churned=1e+05 |

## DestroyCreate/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 11.7 ms | 5.6% | baseline | 85.4 M |  |
| SparseSet | 5.45 ms | 7.3% | **2.16× [2.03–2.26]** | 184 M |  |
| Archetype | 4.94 ms | 6.1% | **2.39× [2.29–2.49]** | 202 M |  |
| QueryPart | 5.49 ms | 6.9% | **2.17× [2.05–2.30]** | 182 M |  |
| QPartHinted | 4.76 ms | 6.4% | **2.48× [2.34–2.61]** | 210 M |  |
| StaticBitmask | 1.98 ms | 5.8% | **5.97× [5.68–6.27]** | 506 M |  |

## Create/Coherent/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 657 ms | 2.3% |  | 15.2 M |  |
| SortedSoA | 403 ms | 2.0% |  | 24.8 M |  |
| SparseSet | 519 ms | 1.9% |  | 19.3 M |  |
| Archetype | 526 ms | 2.7% |  | 19 M |  |
| QueryPart | 527 ms | 3.0% |  | 19 M |  |
| QPartHinted | 509 ms | 2.2% |  | 19.6 M |  |
| OOP | 361 ms | 4.6% |  | 27.7 M |  |

## Iter1/Coherent/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 6.95 ms | 3.0% | baseline | 1.44 G |  |
| SortedSoA | 5.44 ms | 2.3% | **1.27× [1.24–1.30]** | 1.84 G |  |
| SparseSet | 5.42 ms | 1.5% | **1.28× [1.25–1.30]** | 1.85 G |  |
| Archetype | 5.44 ms | 2.1% | **1.28× [1.24–1.30]** | 1.84 G | tables=1 |
| QueryPart | 5.45 ms | 2.6% | **1.26× [1.22–1.30]** | 1.83 G | tables=1 |
| QPartHinted | 5.43 ms | 2.0% | **1.28× [1.25–1.29]** | 1.84 G | tables=1 |

## Update2/Coherent/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 24.3 ms | 3.0% | baseline | 412 M |  |
| SortedSoA | 19.9 ms | 2.3% | **1.22× [1.20–1.24]** | 502 M |  |
| SparseSet | 24.2 ms | 3.1% | 1.01× [0.98–1.04] | 414 M |  |
| Archetype | 17.3 ms | 2.0% | **1.38× [1.37–1.42]** | 578 M | tables=1 |
| QueryPart | 17.4 ms | 2.3% | **1.38× [1.36–1.42]** | 574 M | tables=1 |
| QPartHinted | 31.5 ms | 1.6% | **0.77× [0.75–0.79]** | 317 M | tables=1 |
| OOP | 43.2 ms | 2.7% | **0.58× [0.55–0.63]** | 231 M |  |
| RawSoA | 15.3 ms | 2.2% | **1.58× [1.54–1.60]** | 654 M |  |

## Create/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 1.34 s | 0.3% |  | 7.44 M |  |
| SortedSoA | 711 ms | 0.5% |  | 14.1 M |  |
| SparseSet | 1.15 s | 0.5% |  | 8.73 M |  |
| Archetype | 766 ms | 0.4% |  | 13 M |  |
| QueryPart | 836 ms | 1.0% |  | 12 M |  |
| QPartHinted | 617 ms | 1.0% |  | 16.2 M |  |
| OOP | 394 ms | 3.2% |  | 25.4 M |  |

## Iter1/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 6.88 ms | 5.7% | baseline | 1.45 G |  |
| SortedSoA | 5.37 ms | 3.0% | **1.29× [1.25–1.32]** | 1.86 G |  |
| SparseSet | 5.4 ms | 4.9% | **1.29× [1.20–1.30]** | 1.85 G |  |
| Archetype | 5.42 ms | 2.0% | **1.29× [1.26–1.31]** | 1.85 G | tables=3 |
| QueryPart | 5.42 ms | 4.3% | **1.27× [1.24–1.30]** | 1.84 G | tables=3 |
| QPartHinted | 5.31 ms | 2.7% | **1.30× [1.29–1.33]** | 1.88 G | tables=3 |

## Update2/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 23.5 ms | 1.4% | baseline | 426 M |  |
| SortedSoA | 19.4 ms | 1.7% | **1.20× [1.19–1.22]** | 516 M |  |
| SparseSet | 23.4 ms | 1.6% | 1.00× [0.99–1.01] | 427 M |  |
| Archetype | 17.1 ms | 1.6% | **1.37× [1.35–1.39]** | 586 M | tables=3 |
| QueryPart | 17.1 ms | 2.0% | **1.37× [1.35–1.38]** | 586 M | tables=3 |
| QPartHinted | 17.1 ms | 2.5% | **1.37× [1.34–1.39]** | 585 M | tables=3 |
| OOP | 68.5 ms | 9.7% | **0.37× [0.33–0.38]** | 146 M |  |
| RawSoA | 14.8 ms | 2.0% | **1.58× [1.56–1.60]** | 674 M |  |

## Frame3/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 37.9 ms | 1.6% | baseline | 264 M |  |
| SortedSoA | 36.4 ms | 3.0% | **1.05× [1.02–1.06]** | 274 M |  |
| SparseSet | 38.3 ms | 3.9% | 1.01× [0.98–1.03] | 261 M |  |
| Archetype | 25.7 ms | 4.1% | **1.50× [1.47–1.53]** | 389 M | tables=3 |
| QueryPart | 26 ms | 4.6% | **1.48× [1.45–1.52]** | 384 M | tables=3 |
| QPartHinted | 25.9 ms | 4.6% | **1.50× [1.45–1.54]** | 386 M | tables=3 |

## SparseQuery/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 13.4 ms | 2.9% | baseline | 748 M |  |
| SortedSoA | 5.26 ms | 3.6% | **2.59× [2.55–2.67]** | 1.9 G |  |
| SparseSet | 2.94 ms | 3.3% | **4.58× [4.34–4.73]** | 3.4 G |  |
| Archetype | 163 µs | 3.9% | **83.55× [81.89–84.66]** | 61.3 G | tables=6 |
| QueryPart | 157 µs | 4.5% | **85.44× [83.79–87.39]** | 63.7 G | tables=6 |
| QPartHinted | 160 µs | 3.9% | **84.71× [82.97–86.24]** | 62.7 G | tables=6 |

## RandomGet/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| V1 | 1.86 s | 1.4% | baseline | 5.37 M |  |
| SortedSoA | 1.94 s | 1.7% | **0.96× [0.95–0.97]** | 5.16 M |  |
| SparseSet | 331 ms | 0.8% | **5.66× [5.60–5.75]** | 30.2 M |  |
| Archetype | 314 ms | 1.0% | **5.98× [5.94–6.05]** | 31.8 M |  |
| QueryPart | 395 ms | 0.9% | **4.77× [4.71–4.83]** | 25.3 M |  |
| QPartHinted | 393 ms | 0.7% | **4.77× [4.71–4.88]** | 25.4 M |  |

## AddRemove/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 16.3 ms | 4.8% | baseline | 614 M | churned=1e+06 |
| SparseSet | 13 ms | 5.9% | **1.26× [1.22–1.28]** | 767 M | churned=1e+06 |
| Archetype | 101 ms | 7.1% | **0.16× [0.16–0.17]** | 99.4 M | tables=6, churned=1e+06 |
| QueryPart | 15.6 ms | 6.6% | **1.05× [1.02–1.08]** | 640 M | tables=3, churned=1e+06 |
| QPartHinted | 15.7 ms | 6.2% | **1.04× [1.02–1.08]** | 638 M | tables=3, churned=1e+06 |

## TagChurn/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 16 ms | 5.6% | baseline | 627 M | churned=1e+06 |
| SparseSet | 12.5 ms | 5.4% | **1.25× [1.21–1.33]** | 802 M | churned=1e+06 |
| Archetype | 97.4 ms | 6.8% | **0.16× [0.15–0.16]** | 103 M | tables=6, churned=1e+06 |
| QueryPart | 65.4 ms | 5.8% | **0.24× [0.23–0.25]** | 153 M | tables=6, churned=1e+06 |
| QPartHinted | 71.1 ms | 6.6% | **0.22× [0.21–0.23]** | 141 M | tables=6, churned=1e+06 |

## DestroyCreate/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SortedSoA | 111 ms | 6.8% | baseline | 90.3 M |  |
| SparseSet | 48.3 ms | 6.2% | **2.25× [2.18–2.30]** | 207 M |  |
| Archetype | 46 ms | 6.7% | **2.48× [2.33–2.55]** | 217 M |  |
| QueryPart | 50.8 ms | 6.7% | **2.23× [2.10–2.28]** | 197 M |  |
| QPartHinted | 44.3 ms | 7.3% | **2.51× [2.43–2.57]** | 226 M |  |

## FusionExec/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 1.48 µs | 5.4% | baseline | 676 M |  |
| AlwaysFuse | 1.3 µs | 3.1% | **1.15× [1.10–1.19]** | 770 M |  |
| ShareColumns | 1.24 µs | 3.6% | **1.20× [1.14–1.29]** | 806 M |  |
| AutoTuned | 1.21 µs | 3.8% | **1.24× [1.19–1.28]** | 827 M | chose: ShareColumns |
| ShareColumns+Tiled4K | 1.23 µs | 3.1% | **1.22× [1.18–1.26]** | 813 M |  |
| ShareColumns+Parallel | 1.27 µs | 3.5% | **1.18× [1.14–1.23]** | 787 M |  |
| DeviceAware+Offload1K | 1.57 µs | 3.2% | 0.95× [0.91–1.01] | 638 M |  |

## Frame3Exec/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 1.99 µs | 6.2% | baseline | 502 M |  |
| AlwaysFuse | 1.89 µs | 6.4% | **1.05× [1.04–1.07]** | 529 M |  |
| ShareColumns | 2.03 µs | 4.9% | 0.99× [0.97–1.00] | 493 M |  |
| AutoTuned | 2 µs | 7.4% | 0.99× [0.96–1.01] | 500 M | chose: NeverFuse |
| ShareColumns+Tiled4K | 2 µs | 7.7% | 1.00× [0.97–1.01] | 501 M |  |
| ShareColumns+Parallel | 2.01 µs | 5.6% | 1.00× [0.98–1.02] | 498 M |  |
| DeviceAware+Offload1K | 2.51 µs | 6.6% | **0.79× [0.77–0.80]** | 398 M |  |

## FusionExec/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 152 µs | 5.7% | baseline | 657 M |  |
| AlwaysFuse | 118 µs | 4.6% | **1.33× [1.23–1.35]** | 849 M |  |
| ShareColumns | 119 µs | 5.4% | **1.29× [1.21–1.34]** | 841 M |  |
| AutoTuned | 117 µs | 4.7% | **1.30× [1.27–1.35]** | 853 M | chose: AlwaysFuse |
| ShareColumns+Tiled4K | 118 µs | 6.2% | **1.29× [1.24–1.36]** | 847 M |  |
| ShareColumns+Parallel | 1.03 ms | 17.6% | **0.14× [0.13–0.16]** | 97.1 M |  |
| DeviceAware+Offload1K | 152 µs | 5.4% | 1.00× [0.96–1.06] | 658 M |  |

## Frame3Exec/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 206 µs | 4.7% | baseline | 485 M |  |
| AlwaysFuse | 191 µs | 7.3% | **1.10× [1.04–1.14]** | 524 M |  |
| ShareColumns | 206 µs | 5.6% | 1.01× [1.00–1.03] | 485 M |  |
| AutoTuned | 186 µs | 6.4% | **1.12× [1.08–1.16]** | 537 M | chose: AlwaysFuse |
| ShareColumns+Tiled4K | 208 µs | 8.0% | 1.02× [0.98–1.05] | 481 M |  |
| ShareColumns+Parallel | 1.55 ms | 7.6% | **0.14× [0.13–0.16]** | 64.5 M |  |
| DeviceAware+Offload1K | 245 µs | 4.7% | **0.86× [0.83–0.88]** | 409 M |  |

## FusionExec/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 2.63 ms | 3.6% | baseline | 381 M |  |
| AlwaysFuse | 4.14 ms | 2.7% | **0.61× [0.57–0.64]** | 242 M |  |
| ShareColumns | 1.51 ms | 2.9% | **1.72× [1.64–1.75]** | 662 M |  |
| AutoTuned | 1.46 ms | 3.0% | **1.77× [1.69–1.84]** | 686 M | chose: AlwaysFuse |
| ShareColumns+Tiled4K | 1.52 ms | 2.1% | **1.71× [1.65–1.78]** | 658 M |  |
| ShareColumns+Parallel | 1.59 ms | 10.2% | **1.50× [1.40–1.72]** | 630 M |  |
| DeviceAware+Offload1K | 2.06 ms | 2.4% | **1.27× [1.19–1.31]** | 485 M |  |

## Frame3Exec/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 2.65 ms | 5.8% | baseline | 378 M |  |
| AlwaysFuse | 2.35 ms | 5.7% | **1.12× [1.08–1.15]** | 426 M |  |
| ShareColumns | 2.61 ms | 3.4% | 1.00× [0.98–1.04] | 383 M |  |
| AutoTuned | 2.27 ms | 6.6% | **1.17× [1.12–1.19]** | 441 M | chose: AlwaysFuse |
| ShareColumns+Tiled4K | 4.23 ms | 5.1% | **0.61× [0.59–0.64]** | 236 M |  |
| ShareColumns+Parallel | 2.31 ms | 7.4% | **1.18× [1.14–1.25]** | 433 M |  |
| DeviceAware+Offload1K | 3.01 ms | 3.9% | **0.86× [0.81–0.89]** | 333 M |  |

## FusionExec/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 26.4 ms | 7.8% | baseline | 379 M |  |
| AlwaysFuse | 15.2 ms | 5.8% | **1.73× [1.60–1.81]** | 657 M |  |
| ShareColumns | 15.8 ms | 4.8% | **1.63× [1.57–1.75]** | 632 M |  |
| AutoTuned | 15.7 ms | 4.3% | **1.66× [1.58–1.73]** | 637 M | chose: ShareColumns |
| ShareColumns+Tiled4K | 26.6 ms | 3.2% | 0.96× [0.91–1.03] | 376 M |  |
| ShareColumns+Parallel | 9.04 ms | 7.7% | **2.95× [2.74–3.17]** | 1.11 G |  |
| DeviceAware+Offload1K | 21.4 ms | 4.5% | **1.20× [1.16–1.26]** | 467 M |  |

## Frame3Exec/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| NeverFuse | 25.9 ms | 3.7% | baseline | 387 M |  |
| AlwaysFuse | 23.1 ms | 4.2% | **1.12× [1.09–1.16]** | 433 M |  |
| ShareColumns | 26 ms | 4.2% | 0.99× [0.97–1.02] | 385 M |  |
| AutoTuned | 36.2 ms | 2.1% | **0.71× [0.69–0.73]** | 276 M | chose: AlwaysFuse |
| ShareColumns+Tiled4K | 26.2 ms | 4.1% | 0.99× [0.96–1.01] | 382 M |  |
| ShareColumns+Parallel | 10.3 ms | 3.7% | **2.51× [2.46–2.64]** | 967 M |  |
| DeviceAware+Offload1K | 31.2 ms | 3.0% | **0.83× [0.81–0.84]** | 321 M |  |

## FusionFrame/Coherent/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 1.32 µs | 6.8% | baseline | 758 M | tables=1 |
| QPartHintedFused | 1.13 µs | 10.0% | **1.21× [1.15–1.25]** | 887 M | tables=1 |
| QPartHintedFusedGrouped | 1.07 µs | 9.3% | **1.24× [1.19–1.27]** | 935 M | tables=1 |
| QPartHintedHandFused | 1.45 µs | 9.5% | **0.95× [0.90–0.98]** | 688 M | tables=1 |
| ArchetypeSeq | 1.43 µs | 8.4% | 0.97× [0.94–1.01] | 699 M | tables=1 |
| SparseSetSeq | 2.94 µs | 9.0% | **0.48× [0.45–0.49]** | 340 M |  |

## FusionFrame/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 1.52 µs | 8.2% | baseline | 657 M | tables=3 |
| QPartHintedFused | 1.23 µs | 6.3% | **1.23× [1.18–1.28]** | 812 M | tables=3 |
| QPartHintedFusedGrouped | 1.19 µs | 7.6% | **1.26× [1.20–1.32]** | 841 M | tables=3 |
| QPartHintedHandFused | 1.56 µs | 6.5% | 0.98× [0.93–1.03] | 642 M | tables=3 |
| ArchetypeSeq | 1.51 µs | 8.2% | 1.00× [0.94–1.02] | 663 M | tables=3 |
| SparseSetSeq | 3.66 µs | 6.1% | **0.42× [0.40–0.43]** | 273 M |  |

## Frame3Sys/Fragmented/1000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 2.02 µs | 3.7% | baseline | 495 M | tables=3 |
| QPartHintedFused | 1.91 µs | 6.8% | **1.03× [1.01–1.07]** | 524 M | tables=3 |

## FusionFrame/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 136 µs | 5.4% | baseline | 733 M | tables=1 |
| QPartHintedFused | 122 µs | 6.1% | **1.13× [1.10–1.18]** | 819 M | tables=1 |
| QPartHintedFusedGrouped | 120 µs | 7.1% | **1.17× [1.10–1.19]** | 831 M | tables=1 |
| QPartHintedHandFused | 160 µs | 8.1% | **0.85× [0.82–0.88]** | 624 M | tables=1 |
| ArchetypeSeq | 138 µs | 6.5% | 0.98× [0.96–1.02] | 724 M | tables=1 |
| SparseSetSeq | 286 µs | 5.3% | **0.47× [0.45–0.48]** | 350 M |  |

## FusionFrame/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 162 µs | 5.4% | baseline | 618 M | tables=3 |
| QPartHintedFused | 131 µs | 9.2% | **1.24× [1.18–1.30]** | 762 M | tables=3 |
| QPartHintedFusedGrouped | 129 µs | 8.3% | **1.24× [1.19–1.31]** | 773 M | tables=3 |
| QPartHintedHandFused | 171 µs | 7.9% | **0.95× [0.92–0.98]** | 585 M | tables=3 |
| ArchetypeSeq | 165 µs | 6.6% | 1.00× [0.97–1.03] | 604 M | tables=3 |
| SparseSetSeq | 383 µs | 6.4% | **0.42× [0.42–0.44]** | 261 M |  |

## Frame3Sys/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 198 µs | 6.4% | baseline | 504 M | tables=3 |
| QPartHintedFused | 193 µs | 9.3% | **1.05× [1.03–1.07]** | 519 M | tables=3 |

## FusionFrame/Coherent/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 2.59 ms | 7.2% | baseline | 386 M | tables=1 |
| QPartHintedFused | 1.47 ms | 7.0% | **1.75× [1.67–1.82]** | 680 M | tables=1 |
| QPartHintedFusedGrouped | 1.4 ms | 4.3% | **1.80× [1.70–1.91]** | 717 M | tables=1 |
| QPartHintedHandFused | 1.77 ms | 7.2% | **1.43× [1.36–1.52]** | 566 M | tables=1 |
| ArchetypeSeq | 2.65 ms | 7.1% | 0.98× [0.94–1.04] | 377 M | tables=1 |
| SparseSetSeq | 3.78 ms | 5.2% | **0.70× [0.67–0.73]** | 265 M |  |

## FusionFrame/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 2.64 ms | 4.6% | baseline | 379 M | tables=3 |
| QPartHintedFused | 1.51 ms | 2.8% | **1.72× [1.67–1.81]** | 663 M | tables=3 |
| QPartHintedFusedGrouped | 1.55 ms | 2.3% | **1.65× [1.55–1.72]** | 647 M | tables=3 |
| QPartHintedHandFused | 1.87 ms | 2.1% | **1.39× [1.34–1.44]** | 536 M | tables=3 |
| ArchetypeSeq | 2.67 ms | 5.1% | 1.00× [0.95–1.03] | 375 M | tables=3 |
| SparseSetSeq | 4.26 ms | 3.0% | **0.61× [0.59–0.63]** | 235 M |  |

## Frame3Sys/Fragmented/1000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 2.56 ms | 2.1% | baseline | 391 M | tables=3 |
| QPartHintedFused | 2.37 ms | 1.9% | **1.08× [1.05–1.10]** | 422 M | tables=3 |

## FusionFrame/Coherent/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 24.1 ms | 8.0% | baseline | 415 M | tables=1 |
| QPartHintedFused | 13.9 ms | 7.2% | **1.72× [1.61–1.78]** | 721 M | tables=1 |
| QPartHintedFusedGrouped | 13.6 ms | 5.0% | **1.73× [1.63–1.78]** | 738 M | tables=1 |
| QPartHintedHandFused | 17.3 ms | 5.2% | **1.33× [1.26–1.40]** | 578 M | tables=1 |
| ArchetypeSeq | 24.6 ms | 10.5% | 0.98× [0.95–1.03] | 406 M | tables=1 |
| SparseSetSeq | 37.8 ms | 7.1% | **0.63× [0.61–0.65]** | 265 M |  |

## FusionFrame/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 25 ms | 6.6% | baseline | 399 M | tables=3 |
| QPartHintedFused | 16 ms | 3.2% | **1.55× [1.50–1.68]** | 624 M | tables=3 |
| QPartHintedFusedGrouped | 16.7 ms | 2.6% | **1.51× [1.45–1.60]** | 598 M | tables=3 |
| QPartHintedHandFused | 21.4 ms | 2.1% | **1.18× [1.14–1.27]** | 468 M | tables=3 |
| ArchetypeSeq | 26.5 ms | 6.6% | 0.99× [0.95–1.02] | 377 M | tables=3 |
| SparseSetSeq | 51.9 ms | 2.0% | **0.49× [0.47–0.53]** | 193 M |  |

## Frame3Sys/Fragmented/10000000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| QPartHintedSeq | 28.2 ms | 4.0% | baseline | 355 M | tables=3 |
| QPartHintedFused | 27.4 ms | 3.1% | **1.04× [1.03–1.05]** | 366 M | tables=3 |

## Tick/-/250

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SparseSet | 136 µs | 7.3% | baseline | 7.37 k | entities=1020, projectiles=129, us_population=2.738, us_production=0.4433, us_grid=16.11, us_selection=0.3283, us_acquire=18.04, us_commander=0.2533, us_workers=2.59, us_movement=72.35, us_arrive=1.023, us_combat=19.65, us_projectiles=4.383, us_status=0.285, us_death=1.878, us_regen=1.073 |
| Archetype | 129 µs | 6.1% | **1.07× [1.02–1.10]** | 7.77 k | entities=1020, projectiles=129, us_population=2.005, us_production=0.5967, us_grid=17.73, us_selection=0.4783, us_acquire=17.37, us_commander=0.35, us_workers=2.622, us_movement=64.66, us_arrive=0.8017, us_combat=16.55, us_projectiles=3.292, us_status=0.4533, us_death=1.383, us_regen=0.9517 |
| QueryPart | 129 µs | 8.4% | **1.06× [1.02–1.10]** | 7.76 k | entities=1020, projectiles=129, us_population=1.182, us_production=0.2633, us_grid=18.34, us_selection=0.3683, us_acquire=16.51, us_commander=0.28, us_workers=3.183, us_movement=64.71, us_arrive=0.5883, us_combat=16.24, us_projectiles=14.71, us_status=0.2917, us_death=0.8583, us_regen=0.6517 |
| QueryPartFused | 125 µs | 8.4% | **1.08× [1.07–1.11]** | 7.98 k | entities=1020, projectiles=129, us_population=1.453, us_production=0.2217, us_grid=17.48, us_selection=0.4683, us_acquire=17.45, us_commander=0.265, us_workers=3.56, us_movement=67.68, us_arrive=0.6117, us_combat=16.7, us_projectiles=2.87, us_status=0.3083, us_death=0.8767, us_regen=0.655 |
| QPartHinted | 128 µs | 8.6% | **1.07× [1.04–1.09]** | 7.8 k | entities=1020, projectiles=129, us_population=1.432, us_production=0.2217, us_grid=17.79, us_selection=0.4333, us_acquire=17.23, us_commander=0.3, us_workers=3.153, us_movement=65.08, us_arrive=0.5767, us_combat=16.4, us_projectiles=3.88, us_status=0.3217, us_death=0.9883, us_regen=0.7483 |
| QPartHintedFused | 126 µs | 8.5% | **1.08× [1.05–1.12]** | 7.93 k | entities=1020, projectiles=129, us_population=1.695, us_production=0.2133, us_grid=18.11, us_selection=0.5117, us_acquire=17.86, us_commander=0.8283, us_workers=2.792, us_movement=63.91, us_arrive=0.655, us_combat=16.71, us_projectiles=2.827, us_status=0.3317, us_death=0.985, us_regen=0.74 |
| QPH+NeverFuse | 127 µs | 8.0% | **1.06× [1.04–1.09]** | 7.85 k | entities=1020, projectiles=129, us_population=1.6, us_production=0.2817, us_grid=18.85, us_selection=0.3967, us_acquire=18.67, us_commander=0.3767, us_workers=3.26, us_movement=65.61, us_arrive=0.5783, us_combat=16.94, us_projectiles=2.902, us_status=0.325, us_death=0.9917, us_regen=0.7567 |
| QPH+Parallel | 128 µs | 8.1% | **1.06× [1.02–1.11]** | 7.82 k | entities=1020, projectiles=129, us_population=1.435, us_production=0.3217, us_grid=17.68, us_selection=0.5083, us_acquire=17.75, us_commander=0.3517, us_workers=3.108, us_movement=65.53, us_arrive=0.6817, us_combat=17.34, us_projectiles=3.563, us_status=0.38, us_death=1.215, us_regen=0.775 |
| QPH+Offload | 128 µs | 8.9% | **1.06× [1.01–1.08]** | 7.84 k | entities=1020, projectiles=129, us_population=1.548, us_production=0.3483, us_grid=17.14, us_selection=0.4033, us_acquire=16.72, us_commander=0.335, us_workers=2.912, us_movement=67.14, us_arrive=0.66, us_combat=16.86, us_projectiles=3.32, us_status=0.3817, us_death=1.033, us_regen=0.8 |
| QPH+AutoTuned | 131 µs | 9.4% | **1.04× [1.01–1.10]** | 7.66 k | entities=1020, projectiles=129, us_population=1.743, us_production=0.5883, us_grid=19, us_selection=0.4717, us_acquire=17.89, us_commander=0.3467, us_workers=2.91, us_movement=65.08, us_arrive=0.7367, us_combat=18.09, us_projectiles=3.627, us_status=0.3833, us_death=1.633, us_regen=0.785 |
| StaticBitmask | 136 µs | 7.3% | 1.00× [0.99–1.03] | 7.35 k | entities=1020, projectiles=129, us_population=3.77, us_production=0.705, us_grid=14.93, us_selection=0.8083, us_acquire=16.46, us_commander=0.2333, us_workers=2.635, us_movement=71.91, us_arrive=3.893, us_combat=12.82, us_projectiles=3.593, us_status=1.182, us_death=1.872, us_regen=1.713 |
| SortedSoA | 231 µs | 6.4% | **0.59× [0.57–0.61]** | 4.33 k | entities=1020, projectiles=129, us_population=2.78, us_production=1.395, us_grid=16.43, us_selection=1.195, us_acquire=26.2, us_commander=0.9217, us_workers=14.72, us_movement=73.06, us_arrive=5.835, us_combat=62.33, us_projectiles=16.28, us_status=1.46, us_death=6.912, us_regen=0.8417 |

## Tick/-/2500

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SparseSet | 1.81 ms | 4.2% | baseline | 552 | entities=1.004e+04, projectiles=826, us_population=24.04, us_production=2.604, us_grid=145.4, us_selection=1.931, us_acquire=427.4, us_commander=2.007, us_workers=23.45, us_movement=1012, us_arrive=13.05, us_combat=141.6, us_projectiles=32.48, us_status=1.093, us_death=19.46, us_regen=10.11 |
| Archetype | 1.71 ms | 4.4% | **1.06× [1.04–1.10]** | 586 | entities=1.004e+04, projectiles=826, us_population=11.85, us_production=1.507, us_grid=176.3, us_selection=3.138, us_acquire=451, us_commander=1.456, us_workers=20.7, us_movement=932.5, us_arrive=4, us_combat=104.3, us_projectiles=19.23, us_status=1.285, us_death=10.41, us_regen=4.325 |
| QueryPart | 1.74 ms | 4.2% | **1.04× [1.03–1.07]** | 576 | entities=1.004e+04, projectiles=826, us_population=11.57, us_production=1.269, us_grid=177.4, us_selection=2.936, us_acquire=462.9, us_commander=1.485, us_workers=28.79, us_movement=933.5, us_arrive=3.722, us_combat=104.9, us_projectiles=21.44, us_status=1.053, us_death=10.85, us_regen=3.94 |
| QueryPartFused | 1.71 ms | 3.7% | **1.06× [1.03–1.09]** | 586 | entities=1.004e+04, projectiles=826, us_population=10.66, us_production=1.311, us_grid=173.6, us_selection=2.938, us_acquire=454, us_commander=1.495, us_workers=26.82, us_movement=927.9, us_arrive=4.484, us_combat=107.7, us_projectiles=21.84, us_status=1.162, us_death=10.45, us_regen=4.138 |
| QPartHinted | 1.72 ms | 2.8% | **1.04× [1.02–1.07]** | 583 | entities=1.004e+04, projectiles=826, us_population=12.36, us_production=1.067, us_grid=178.3, us_selection=2.989, us_acquire=465, us_commander=1.518, us_workers=25.96, us_movement=962, us_arrive=4.129, us_combat=113.9, us_projectiles=20.71, us_status=1.331, us_death=11.96, us_regen=4.28 |
| QPartHintedFused | 1.75 ms | 3.9% | **1.04× [1.01–1.06]** | 570 | entities=1.004e+04, projectiles=826, us_population=11.93, us_production=1.275, us_grid=176.5, us_selection=3.007, us_acquire=469.2, us_commander=1.478, us_workers=27.91, us_movement=943.3, us_arrive=4.165, us_combat=109.8, us_projectiles=18.59, us_status=1.182, us_death=10.25, us_regen=4.249 |
| QPH+NeverFuse | 1.72 ms | 3.6% | **1.05× [1.02–1.08]** | 581 | entities=1.004e+04, projectiles=826, us_population=11.56, us_production=0.9982, us_grid=174.2, us_selection=2.989, us_acquire=462.7, us_commander=1.444, us_workers=26.28, us_movement=941.9, us_arrive=3.744, us_combat=107.2, us_projectiles=19.49, us_status=1.109, us_death=10.13, us_regen=4.267 |
| QPH+Parallel | 1.74 ms | 3.8% | **1.04× [1.01–1.07]** | 573 | entities=1.004e+04, projectiles=826, us_population=14.48, us_production=1.982, us_grid=187.5, us_selection=3.307, us_acquire=472.7, us_commander=1.655, us_workers=25.57, us_movement=928.2, us_arrive=3.765, us_combat=108.5, us_projectiles=18.85, us_status=1.202, us_death=10.68, us_regen=4.262 |
| QPH+Offload | 1.73 ms | 4.4% | **1.04× [1.02–1.08]** | 576 | entities=1.004e+04, projectiles=826, us_population=11.93, us_production=1.384, us_grid=176.5, us_selection=3.013, us_acquire=457.7, us_commander=1.438, us_workers=25.17, us_movement=946.4, us_arrive=4.218, us_combat=107.2, us_projectiles=18.82, us_status=1.195, us_death=10.68, us_regen=4.222 |
| QPH+AutoTuned | 1.74 ms | 5.2% | **1.05× [1.01–1.07]** | 576 | entities=1.004e+04, projectiles=826, us_population=11.97, us_production=1.34, us_grid=176.9, us_selection=3.044, us_acquire=465.4, us_commander=1.835, us_workers=25.67, us_movement=941.7, us_arrive=4.409, us_combat=108.4, us_projectiles=19.45, us_status=1.278, us_death=10.83, us_regen=4.356 |
| StaticBitmask | 1.68 ms | 5.2% | **1.07× [1.05–1.11]** | 595 | entities=1.004e+04, projectiles=826, us_population=17.99, us_production=4.558, us_grid=122.4, us_selection=5.035, us_acquire=395.5, us_commander=2.149, us_workers=16.55, us_movement=982.9, us_arrive=30.84, us_combat=71.46, us_projectiles=32.47, us_status=11.54, us_death=13.17, us_regen=10.23 |

## Tick/-/12500

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SparseSet | 9.35 ms | 1.7% | baseline | 107 | entities=5.028e+04, projectiles=2098, us_population=105.4, us_production=5.64, us_grid=778, us_selection=13.32, us_acquire=2943, us_commander=8.415, us_workers=159.9, us_movement=4645, us_arrive=91.97, us_combat=603.2, us_projectiles=91.24, us_status=2.302, us_death=90.94, us_regen=52.1 |
| Archetype | 8.34 ms | 2.6% | **1.12× [1.11–1.14]** | 120 | entities=5.028e+04, projectiles=2098, us_population=63.56, us_production=3.091, us_grid=867.3, us_selection=14.24, us_acquire=3011, us_commander=8.102, us_workers=119.6, us_movement=4005, us_arrive=20.37, us_combat=318.9, us_projectiles=48.01, us_status=2.115, us_death=57.3, us_regen=20.45 |
| QueryPart | 8.53 ms | 2.4% | **1.10× [1.09–1.13]** | 117 | entities=5.028e+04, projectiles=2098, us_population=62.24, us_production=3.104, us_grid=866.2, us_selection=13.49, us_acquire=3089, us_commander=8.362, us_workers=132.3, us_movement=4005, us_arrive=22.29, us_combat=343.1, us_projectiles=56.2, us_status=1.873, us_death=58.22, us_regen=20.48 |
| QueryPartFused | 8.4 ms | 1.8% | **1.11× [1.10–1.13]** | 119 | entities=5.028e+04, projectiles=2098, us_population=62.59, us_production=2.573, us_grid=846.5, us_selection=13.28, us_acquire=3033, us_commander=7.705, us_workers=129.5, us_movement=3969, us_arrive=22.85, us_combat=340.5, us_projectiles=57.46, us_status=1.802, us_death=63.83, us_regen=20.6 |
| QPartHinted | 8.54 ms | 2.7% | **1.10× [1.09–1.11]** | 117 | entities=5.028e+04, projectiles=2098, us_population=63.48, us_production=2.382, us_grid=869.2, us_selection=13.57, us_acquire=3083, us_commander=8.624, us_workers=133.5, us_movement=4049, us_arrive=21.28, us_combat=340.9, us_projectiles=47.63, us_status=1.984, us_death=56.89, us_regen=20.2 |
| QPartHintedFused | 8.46 ms | 3.0% | **1.11× [1.09–1.13]** | 118 | entities=5.028e+04, projectiles=2098, us_population=62.84, us_production=2.704, us_grid=846.4, us_selection=13.28, us_acquire=3018, us_commander=7.804, us_workers=130.6, us_movement=3938, us_arrive=23.13, us_combat=337, us_projectiles=47.85, us_status=2.064, us_death=56.22, us_regen=20.73 |
| QPH+NeverFuse | 8.56 ms | 3.3% | **1.10× [1.09–1.11]** | 117 | entities=5.028e+04, projectiles=2098, us_population=67.24, us_production=2.451, us_grid=885.3, us_selection=14.69, us_acquire=3158, us_commander=8.465, us_workers=129.2, us_movement=4019, us_arrive=21.72, us_combat=346.8, us_projectiles=48.41, us_status=1.98, us_death=57.64, us_regen=20.36 |
| QPH+Parallel | 6.97 ms | 3.9% | **1.35× [1.32–1.41]** | 144 | entities=5.028e+04, projectiles=2098, us_population=64.6, us_production=3.095, us_grid=883.5, us_selection=14.8, us_acquire=3128, us_commander=8.32, us_workers=131.9, us_movement=2249, us_arrive=54.24, us_combat=383.4, us_projectiles=46.59, us_status=2.029, us_death=59.64, us_regen=21.04 |
| QPH+Offload | 8.66 ms | 3.1% | **1.09× [1.07–1.10]** | 115 | entities=5.028e+04, projectiles=2098, us_population=65.51, us_production=3.124, us_grid=892.3, us_selection=14.12, us_acquire=3133, us_commander=8.965, us_workers=123.4, us_movement=4081, us_arrive=22.47, us_combat=341, us_projectiles=56.97, us_status=2.691, us_death=58.06, us_regen=22.88 |
| QPH+AutoTuned | 8.56 ms | 2.2% | **1.10× [1.08–1.11]** | 117 | entities=5.028e+04, projectiles=2098, us_population=65.04, us_production=3.127, us_grid=873.6, us_selection=13.81, us_acquire=3098, us_commander=7.927, us_workers=144.8, us_movement=4085, us_arrive=21.03, us_combat=338.2, us_projectiles=47.76, us_status=2.02, us_death=57.12, us_regen=20.35 |
| StaticBitmask | 8.1 ms | 2.5% | **1.16× [1.14–1.19]** | 123 | entities=5.028e+04, projectiles=2098, us_population=74.86, us_production=17.93, us_grid=691.2, us_selection=35.7, us_acquire=2665, us_commander=4.171, us_workers=76.16, us_movement=4072, us_arrive=92.91, us_combat=197.9, us_projectiles=124.9, us_status=49.75, us_death=67.43, us_regen=39.79 |

## Tick/-/50000

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| SparseSet | 42.4 ms | 3.8% | baseline | 23.6 | entities=2.006e+05, projectiles=3989, us_population=413.4, us_production=11.77, us_grid=3425, us_selection=65.17, us_acquire=1.401e+04, us_commander=31.86, us_workers=642.9, us_movement=1.967e+04, us_arrive=579.8, us_combat=2185, us_projectiles=258.5, us_status=6.749, us_death=383.1, us_regen=313.8 |
| Archetype | 37.6 ms | 3.1% | **1.12× [1.09–1.14]** | 26.6 | entities=2.006e+05, projectiles=3989, us_population=278.5, us_production=5.985, us_grid=3767, us_selection=52.53, us_acquire=1.394e+04, us_commander=29.13, us_workers=413.7, us_movement=1.729e+04, us_arrive=161.5, us_combat=961.7, us_projectiles=129.6, us_status=4.736, us_death=229.4, us_regen=210.6 |
| QueryPart | 38.5 ms | 3.2% | **1.11× [1.09–1.12]** | 26 | entities=2.006e+05, projectiles=3989, us_population=303.3, us_production=7.947, us_grid=3843, us_selection=50.68, us_acquire=1.424e+04, us_commander=39.86, us_workers=491.8, us_movement=1.745e+04, us_arrive=156.4, us_combat=1075, us_projectiles=141.1, us_status=3.916, us_death=197.2, us_regen=192.9 |
| QueryPartFused | 38.4 ms | 3.2% | **1.12× [1.09–1.13]** | 26 | entities=2.006e+05, projectiles=3989, us_population=293.4, us_production=7.231, us_grid=3761, us_selection=51.7, us_acquire=1.414e+04, us_commander=34.21, us_workers=492.2, us_movement=1.739e+04, us_arrive=178.3, us_combat=1058, us_projectiles=135.2, us_status=3.971, us_death=243.7, us_regen=197 |
| QPartHinted | 37.3 ms | 3.5% | **1.12× [1.10–1.14]** | 26.8 | entities=2.006e+05, projectiles=3989, us_population=294.4, us_production=7.067, us_grid=3770, us_selection=51.75, us_acquire=1.404e+04, us_commander=45.82, us_workers=474.4, us_movement=1.709e+04, us_arrive=159.2, us_combat=1065, us_projectiles=120.6, us_status=4.28, us_death=191.9, us_regen=191.5 |
| QPartHintedFused | 38.2 ms | 3.3% | **1.11× [1.09–1.12]** | 26.2 | entities=2.006e+05, projectiles=3989, us_population=301.9, us_production=7.458, us_grid=3867, us_selection=51.22, us_acquire=1.408e+04, us_commander=31.7, us_workers=488.9, us_movement=1.73e+04, us_arrive=179.3, us_combat=1027, us_projectiles=124.1, us_status=4.551, us_death=215.9, us_regen=200.7 |
| QPH+NeverFuse | 38.4 ms | 3.2% | **1.10× [1.09–1.12]** | 26.1 | entities=2.006e+05, projectiles=3989, us_population=311.2, us_production=7.302, us_grid=3882, us_selection=50.02, us_acquire=1.415e+04, us_commander=34.71, us_workers=470.3, us_movement=1.743e+04, us_arrive=158.4, us_combat=1058, us_projectiles=126, us_status=4.3, us_death=205.8, us_regen=190.9 |
| QPH+Parallel | 26.5 ms | 2.0% | **1.59× [1.54–1.64]** | 37.7 | entities=2.006e+05, projectiles=3989, us_population=295.8, us_production=7.656, us_grid=3790, us_selection=50.31, us_acquire=1.413e+04, us_commander=32.11, us_workers=457.4, us_movement=6012, us_arrive=185.9, us_combat=971.9, us_projectiles=126.1, us_status=4.653, us_death=228.2, us_regen=214.6 |
| QPH+Offload | 38.6 ms | 3.2% | **1.10× [1.07–1.13]** | 25.9 | entities=2.006e+05, projectiles=3989, us_population=291.1, us_production=6.895, us_grid=3821, us_selection=50.58, us_acquire=1.411e+04, us_commander=32.95, us_workers=449.1, us_movement=1.764e+04, us_arrive=157, us_combat=1037, us_projectiles=127.3, us_status=4.025, us_death=207.3, us_regen=192.2 |
| QPH+AutoTuned | 38.2 ms | 2.6% | **1.11× [1.08–1.13]** | 26.2 | entities=2.006e+05, projectiles=3989, us_population=288.9, us_production=7.498, us_grid=3869, us_selection=50.85, us_acquire=1.41e+04, us_commander=34.13, us_workers=445.6, us_movement=1.744e+04, us_arrive=161.4, us_combat=1097, us_projectiles=126.2, us_status=4.433, us_death=210.6, us_regen=192.8 |
| StaticBitmask | 37.3 ms | 3.8% | **1.13× [1.11–1.15]** | 26.8 | entities=2.006e+05, projectiles=3989, us_population=319.1, us_production=95.62, us_grid=3358, us_selection=134.2, us_acquire=1.32e+04, us_commander=10.31, us_workers=291.9, us_movement=1.767e+04, us_arrive=277.7, us_combat=666.6, us_projectiles=396.8, us_status=184.4, us_death=284.9, us_regen=259 |

## Spans/-/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| k1 | 128 µs | 7.0% | baseline | 780 M | per_span=1e+05 |
| k6 | 114 µs | 6.3% | **1.11× [1.07–1.13]** | 875 M | per_span=1.667e+04 |
| k64 | 115 µs | 5.2% | **1.11× [1.07–1.15]** | 871 M | per_span=1562 |
| k512 | 121 µs | 5.4% | **1.04× [1.00–1.10]** | 828 M | per_span=195.3 |
| k4096 | 153 µs | 5.6% | **0.84× [0.80–0.85]** | 654 M | per_span=24.41 |
| k16384 | 172 µs | 5.0% | **0.75× [0.73–0.77]** | 582 M | per_span=6.104 |

## dynamic

| budget | degraded_us | full_us | worst_frame_us | flip_frame | migration_ms |
|---|---|---|---|---|---|
| stall (all at once) | 5773.5 | 284.5 | 40721.9 | 0 | 34.5953 |
| 65536 | 3535.5 | 277.4 | 10484.4 | 7 | 35.4619 |
| 16384 | 3260.0 | 311.9 | 7765.9 | 30 | 36.9691 |
| 4096 | 4845.9 | 0.0 | 7136.3 | -1 | 18.2949 |
| never (degraded only) | 5654.2 | 0.0 | 7015.5 | -1 | 0.002 |

