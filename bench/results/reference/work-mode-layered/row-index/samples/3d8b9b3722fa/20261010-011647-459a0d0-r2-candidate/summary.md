# Benchmark run 20261010-011647-459a0d0-r2-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14 ns | 3.8% | baseline | 71.6 M | checksum=6.808e+07 |
| Each | 16.2 ns | 4.3% | **0.87× [0.80–0.94]** | 61.9 M | checksum=6.786e+07 |
| Grain1 | 50.6 ns | 4.5% | **0.28× [0.24–0.32]** | 19.8 M | checksum=6.757e+07 |
| Grain64 | 18.1 ns | 7.0% | **0.79× [0.67–0.84]** | 55.3 M | checksum=6.808e+07 |
| Grain1024 | 17.9 ns | 3.2% | **0.78× [0.65–0.85]** | 56 M | checksum=6.808e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 ns | 4.5% | baseline | 64.7 M | checksum=8.311e+07 |
| Each | 19.5 ns | 5.4% | **0.81× [0.75–0.89]** | 51.4 M | checksum=8.308e+07 |
| Grain1 | 147 ns | 2.7% | **0.11× [0.10–0.16]** | 6.8 M | checksum=8.214e+07 |
| Grain64 | 25.2 ns | 4.3% | **0.63× [0.58–0.68]** | 39.8 M | checksum=8.314e+07 |
| Grain1024 | 26.1 ns | 5.1% | **0.60× [0.57–0.62]** | 38.3 M | checksum=8.318e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 697 ns | 3.6% | baseline | 1.43 M | checksum=2.118e+08 |
| Each | 659 ns | 2.7% | 1.05× [0.85–1.48] | 1.52 M | checksum=2.121e+08 |
| Grain1 | 2.57 µs | 4.0% | **0.27× [0.25–0.35]** | 389 k | checksum=2.097e+08 |
| Grain64 | 1.22 µs | 1.6% | **0.57× [0.54–0.70]** | 818 k | checksum=2.101e+08 |
| Grain1024 | 734 ns | 2.2% | **0.96× [0.91–1.00]** | 1.36 M | checksum=2.119e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 734 ns | 2.0% | baseline | 1.36 M | checksum=2.033e+08 |
| Each | 684 ns | 1.7% | **1.08× [1.03–1.11]** | 1.46 M | checksum=2.045e+08 |
| Grain1 | 9.08 µs | 3.0% | **0.08× [0.07–0.09]** | 110 k | checksum=2.015e+08 |
| Grain64 | 1.12 µs | 4.1% | **0.67× [0.59–0.69]** | 896 k | checksum=2.033e+08 |
| Grain1024 | 731 ns | 3.4% | 1.03× [0.95–1.08] | 1.37 M | checksum=2.035e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.9 µs | 2.6% | baseline | 47.9 k | checksum=5.23e+09 |
| Each | 20.5 µs | 2.3% | 1.03× [1.00–1.14] | 48.7 k | checksum=5.23e+09 |
| Grain1 | 62.8 µs | 4.1% | **0.34× [0.30–0.38]** | 15.9 k | checksum=5.228e+09 |
| Grain64 | 29.7 µs | 1.5% | **0.70× [0.69–0.78]** | 33.6 k | checksum=5.229e+09 |
| Grain1024 | 20.9 µs | 2.4% | 1.01× [0.80–1.11] | 47.8 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.6 µs | 1.7% | baseline | 48.5 k | checksum=5.239e+09 |
| Each | 20.8 µs | 1.5% | 0.98× [0.96–1.04] | 48 k | checksum=5.239e+09 |
| Grain1 | 219 µs | 1.6% | **0.09× [0.09–0.11]** | 4.57 k | checksum=5.236e+09 |
| Grain64 | 26.9 µs | 2.5% | **0.76× [0.70–0.89]** | 37.1 k | checksum=5.238e+09 |
| Grain1024 | 21.2 µs | 1.1% | 0.97× [0.94–1.05] | 47.1 k | checksum=5.238e+09 |

