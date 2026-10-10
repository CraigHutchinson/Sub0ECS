# Benchmark run 20261010-011654-459a0d0-r3-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.1 ns | 3.0% | baseline | 71 M | checksum=9.403e+07 |
| Each | 16 ns | 1.6% | **0.87× [0.80–0.98]** | 62.6 M | checksum=9.399e+07 |
| Grain1 | 49.4 ns | 2.7% | **0.29× [0.27–0.31]** | 20.2 M | checksum=9.329e+07 |
| Grain64 | 18.3 ns | 6.0% | **0.77× [0.71–0.87]** | 54.7 M | checksum=9.401e+07 |
| Grain1024 | 17.6 ns | 4.3% | **0.82× [0.70–0.86]** | 56.9 M | checksum=9.4e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.2 ns | 1.8% | baseline | 65.9 M | checksum=8.792e+07 |
| Each | 18.6 ns | 1.6% | **0.81× [0.79–0.85]** | 53.7 M | checksum=8.792e+07 |
| Grain1 | 140 ns | 2.1% | **0.11× [0.10–0.11]** | 7.13 M | checksum=8.68e+07 |
| Grain64 | 25.1 ns | 3.7% | **0.60× [0.56–0.65]** | 39.8 M | checksum=8.785e+07 |
| Grain1024 | 25.1 ns | 1.2% | **0.61× [0.58–0.63]** | 39.8 M | checksum=8.778e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 683 ns | 1.6% | baseline | 1.46 M | checksum=2.935e+08 |
| Each | 657 ns | 1.0% | 1.03× [0.98–1.05] | 1.52 M | checksum=2.938e+08 |
| Grain1 | 2.45 µs | 1.4% | **0.28× [0.27–0.29]** | 408 k | checksum=2.9e+08 |
| Grain64 | 1.27 µs | 4.4% | **0.55× [0.50–0.58]** | 787 k | checksum=2.917e+08 |
| Grain1024 | 730 ns | 1.4% | 0.95× [0.92–1.03] | 1.37 M | checksum=2.934e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 715 ns | 2.6% | baseline | 1.4 M | checksum=2.704e+08 |
| Each | 683 ns | 2.1% | 1.04× [0.98–1.08] | 1.46 M | checksum=2.699e+08 |
| Grain1 | 9.2 µs | 2.7% | **0.08× [0.08–0.08]** | 109 k | checksum=2.665e+08 |
| Grain64 | 1.05 µs | 1.1% | **0.67× [0.63–0.70]** | 949 k | checksum=2.691e+08 |
| Grain1024 | 719 ns | 4.1% | 1.00× [0.85–1.06] | 1.39 M | checksum=2.704e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.7 µs | 1.5% | baseline | 48.4 k | checksum=5.232e+09 |
| Each | 20.5 µs | 1.7% | **1.01× [1.00–1.07]** | 48.9 k | checksum=5.235e+09 |
| Grain1 | 61.5 µs | 2.4% | **0.34× [0.32–0.36]** | 16.3 k | checksum=5.232e+09 |
| Grain64 | 31.1 µs | 5.4% | **0.68× [0.62–0.74]** | 32.2 k | checksum=5.233e+09 |
| Grain1024 | 20.9 µs | 1.9% | 0.99× [0.96–1.12] | 47.9 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.5 µs | 4.2% | baseline | 46.5 k | checksum=5.235e+09 |
| Each | 20.7 µs | 1.9% | 1.03× [0.94–1.18] | 48.3 k | checksum=5.235e+09 |
| Grain1 | 216 µs | 2.2% | **0.10× [0.09–0.12]** | 4.63 k | checksum=5.231e+09 |
| Grain64 | 29.3 µs | 7.2% | **0.78× [0.70–0.91]** | 34.1 k | checksum=5.234e+09 |
| Grain1024 | 22.4 µs | 7.4% | 1.01× [0.98–1.11] | 44.7 k | checksum=5.234e+09 |

