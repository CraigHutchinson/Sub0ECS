# Benchmark run 20261010-011651-459a0d0-r3-control

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.5 ns | 4.1% | baseline | 69.1 M | checksum=9.403e+07 |
| Each | 21.4 ns | 3.1% | **0.68× [0.62–0.70]** | 46.7 M | checksum=9.399e+07 |
| Grain1 | 145 ns | 2.9% | **0.10× [0.09–0.11]** | 6.9 M | checksum=9.281e+07 |
| Grain64 | 22.9 ns | 2.4% | **0.63× [0.59–0.66]** | 43.8 M | checksum=9.371e+07 |
| Grain1024 | 18.3 ns | 3.2% | **0.77× [0.67–0.85]** | 54.7 M | checksum=9.396e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 ns | 2.1% | baseline | 62.7 M | checksum=8.384e+07 |
| Each | 20.9 ns | 4.3% | **0.75× [0.66–0.81]** | 47.9 M | checksum=8.382e+07 |
| Grain1 | 138 ns | 1.0% | **0.11× [0.11–0.12]** | 7.25 M | checksum=8.304e+07 |
| Grain64 | 24.2 ns | 2.4% | **0.63× [0.60–0.68]** | 41.3 M | checksum=8.416e+07 |
| Grain1024 | 20.8 ns | 2.4% | **0.75× [0.72–0.81]** | 48.1 M | checksum=8.412e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 687 ns | 1.6% | baseline | 1.46 M | checksum=2.657e+08 |
| Each | 1.16 µs | 1.7% | **0.59× [0.54–0.65]** | 859 k | checksum=2.641e+08 |
| Grain1 | 8.92 µs | 1.0% | **0.08× [0.08–0.08]** | 112 k | checksum=2.619e+08 |
| Grain64 | 1.05 µs | 2.1% | **0.66× [0.63–0.70]** | 956 k | checksum=2.645e+08 |
| Grain1024 | 719 ns | 3.4% | 0.98× [0.92–1.03] | 1.39 M | checksum=2.657e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 726 ns | 2.1% | baseline | 1.38 M | checksum=2.771e+08 |
| Each | 1.19 µs | 3.3% | **0.62× [0.53–0.65]** | 837 k | checksum=2.754e+08 |
| Grain1 | 8.8 µs | 1.8% | **0.08× [0.08–0.09]** | 114 k | checksum=2.733e+08 |
| Grain64 | 1.05 µs | 0.7% | **0.68× [0.68–0.76]** | 954 k | checksum=2.759e+08 |
| Grain1024 | 694 ns | 0.9% | **1.04× [1.00–1.14]** | 1.44 M | checksum=2.773e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.1 µs | 2.6% | baseline | 47.4 k | checksum=5.235e+09 |
| Each | 29.1 µs | 2.5% | **0.74× [0.71–0.78]** | 34.4 k | checksum=5.234e+09 |
| Grain1 | 221 µs | 1.5% | **0.10× [0.09–0.11]** | 4.53 k | checksum=5.231e+09 |
| Grain64 | 27.3 µs | 2.9% | **0.78× [0.74–0.85]** | 36.7 k | checksum=5.234e+09 |
| Grain1024 | 21.5 µs | 2.0% | 1.00× [0.94–1.07] | 46.5 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.8 µs | 4.7% | baseline | 45.8 k | checksum=5.198e+09 |
| Each | 32.3 µs | 5.3% | **0.73× [0.63–0.81]** | 31 k | checksum=5.198e+09 |
| Grain1 | 236 µs | 3.0% | **0.09× [0.09–0.11]** | 4.24 k | checksum=5.195e+09 |
| Grain64 | 30.3 µs | 7.5% | **0.77× [0.66–0.84]** | 33 k | checksum=5.197e+09 |
| Grain1024 | 23.8 µs | 10.3% | 0.97× [0.80–1.08] | 42.1 k | checksum=5.198e+09 |

