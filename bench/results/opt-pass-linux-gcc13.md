# Code review & optimisation pass — QueryPart storage (linux, gcc 13, 4-core VM)

Filter: `(Create|RandomGet|TagChurn|DestroyCreate|AddRemove|Update2|Iter1)/Fragmented/(Archetype|QueryPart|QPartHinted)/(100K|1M)`,
5 repetitions, random interleaving. Times are medians in µs; "speed-up" is before ÷ after.
Raw data: `opt-pass-before-linux-gcc13.json`, `opt-pass-after-linux-gcc13.json`.

**Read Archetype rows as the drift control.** Archetype's code did not change, yet its rows
moved 0.87–1.02× between the two runs (shared VM). A QueryPart change is only real if it
clearly exceeds that.

| AddRemove/Fragmented/Archetype/100000 | 1093 | 1187 | 0.92× | 5.5%/8.1% |
| AddRemove/Fragmented/Archetype/1000000 | 1.193e+04 | 1.306e+04 | 0.91× | 4.5%/3.0% |
| AddRemove/Fragmented/QPartHinted/100000 | 78.59 | 95.47 | 0.82× | 13.1%/8.3% |
| AddRemove/Fragmented/QPartHinted/1000000 | 1006 | 1263 | 0.80× | 6.9%/9.0% |
| AddRemove/Fragmented/QueryPart/100000 | 80.67 | 85.15 | 0.95× | 8.1%/9.6% |
| AddRemove/Fragmented/QueryPart/1000000 | 1025 | 1200 | 0.85× | 6.8%/5.1% |
| Benchmark | before | after | speed-up | cv(b/a) |
| DestroyCreate/Fragmented/Archetype/100000 | 469.5 | 483.3 | 0.97× | 6.2%/7.4% |
| DestroyCreate/Fragmented/Archetype/1000000 | 6702 | 6981 | 0.96× | 8.2%/8.0% |
| DestroyCreate/Fragmented/QPartHinted/100000 | 508.7 | 433.3 | 1.17× | 3.9%/7.7% |
| DestroyCreate/Fragmented/QPartHinted/1000000 | 6324 | 5550 | 1.14× | 6.4%/6.2% |
| DestroyCreate/Fragmented/QueryPart/100000 | 571 | 463.3 | 1.23× | 4.5%/7.2% |
| DestroyCreate/Fragmented/QueryPart/1000000 | 8995 | 7326 | 1.23× | 4.3%/2.1% |
| Iter1/Fragmented/Archetype/100000 | 34.59 | 34.1 | 1.01× | 11.1%/9.2% |
| Iter1/Fragmented/Archetype/1000000 | 569.3 | 555.6 | 1.02× | 8.2%/8.7% |
| Iter1/Fragmented/QPartHinted/100000 | 32.54 | 34.66 | 0.94× | 9.0%/7.8% |
| Iter1/Fragmented/QPartHinted/1000000 | 568.4 | 561.8 | 1.01× | 6.4%/6.8% |
| Iter1/Fragmented/QueryPart/100000 | 35.03 | 35.14 | 1.00× | 11.0%/4.8% |
| Iter1/Fragmented/QueryPart/1000000 | 587.9 | 562.1 | 1.05× | 12.9%/7.2% |
| RandomGet/Fragmented/Archetype/100000 | 1122 | 1157 | 0.97× | 2.8%/3.5% |
| RandomGet/Fragmented/Archetype/1000000 | 2.131e+04 | 2.438e+04 | 0.87× | 4.5%/8.3% |
| RandomGet/Fragmented/QPartHinted/100000 | 1711 | 1688 | 1.01× | 7.4%/3.1% |
| RandomGet/Fragmented/QPartHinted/1000000 | 2.836e+04 | 3.257e+04 | 0.87× | 9.5%/8.3% |
| RandomGet/Fragmented/QueryPart/100000 | 1669 | 1699 | 0.98× | 5.9%/6.3% |
| RandomGet/Fragmented/QueryPart/1000000 | 3.097e+04 | 3.327e+04 | 0.93× | 10.5%/9.2% |
| TagChurn/Fragmented/Archetype/100000 | 1121 | 1157 | 0.97× | 9.9%/2.1% |
| TagChurn/Fragmented/Archetype/1000000 | 1.316e+04 | 1.339e+04 | 0.98× | 2.3%/4.1% |
| TagChurn/Fragmented/QPartHinted/100000 | 1470 | 768 | 1.91× | 1.3%/3.1% |
| TagChurn/Fragmented/QPartHinted/1000000 | 1.594e+04 | 1.089e+04 | 1.46× | 11.2%/12.4% |
| TagChurn/Fragmented/QueryPart/100000 | 1242 | 697.5 | 1.78× | 9.8%/3.9% |
| TagChurn/Fragmented/QueryPart/1000000 | 1.443e+04 | 9161 | 1.58× | 4.3%/5.7% |
| Update2/Fragmented/Archetype/100000 | 48.17 | 47.69 | 1.01× | 3.8%/5.7% |
| Update2/Fragmented/Archetype/1000000 | 719.7 | 734 | 0.98× | 5.6%/7.0% |
| Update2/Fragmented/QPartHinted/100000 | 44.75 | 39.74 | 1.13× | 6.3%/8.4% |
| Update2/Fragmented/QPartHinted/1000000 | 732.5 | 673.4 | 1.09× | 4.2%/8.6% |
| Update2/Fragmented/QueryPart/100000 | 40.77 | 40.26 | 1.01× | 7.9%/4.3% |
| Update2/Fragmented/QueryPart/1000000 | 701.4 | 728.6 | 0.96× | 6.0%/6.1% |
|---|---:|---:|---:|---:|

## RandomGet after the `find()` reorder (separate run, 6 repetitions)

The "after" JSON above predates the final `find()` change (column lookup before the
`has` check). Re-measured in one interleaved run:

| N | Archetype | QueryPart | QPartHinted | QueryPart vs Arch | Hinted vs Arch | before (vs Arch) |
|---|---:|---:|---:|---:|---:|---:|
| 100K | 1183 | 1371 | 1331 | 0.86× | 0.89× | 0.67× / 0.66× |
| 1M | 26387 | 23632 | 27252 | 1.12× | 0.97× | 0.69× / 0.75× |

## H9 dynamic timeline, 1M entities (500K promoted)

| Budget | worst frame before (ms) | after (ms) | total migration before (ms) | after (ms) |
|---|---:|---:|---:|---:|
| stall (all at once) | 109.3 | 91.6 | 99.9 | 83.8 |
| 65536 | 25.4 | 18.8 | 112.2 | 80.9 |
| 16384 | 13.3 | 10.8 | 113.3 | 71.5 |
| 4096 | 10.4 | 9.9 | 49.8 | 33.2 |
| never (degraded only) | 10.2 | 8.4 | 0.0 | 0.0 |

## Create (separate run, 6 repetitions; ratio vs Archetype, H1 ratio in brackets)

| N | Archetype | QueryPart | QPartHinted |
|---|---:|---:|---:|
| 100K | 6.94 ms | 7.25 ms, 0.96× (0.75×) | 5.35 ms, 1.30× (0.88×) |
| 1M | 103.8 ms | 125.0 ms, 0.83× (0.66×) | 92.3 ms, 1.12× (0.83×) |

The original filter's trailing `$` excluded Create (`/manual_time` suffix), so Create was
compared by ratio to Archetype against the committed H1 run instead.
