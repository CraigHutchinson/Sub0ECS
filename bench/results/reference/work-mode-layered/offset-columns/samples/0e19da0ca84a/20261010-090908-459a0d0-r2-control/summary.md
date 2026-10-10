# Benchmark run 20261010-090908-459a0d0-r2-control

- rows: ok in 2 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.2 ns | 17.4% | baseline | 52.1 M | checksum=9.16e+07 |
| Each | 37 ns | 54.4% | **0.57× [0.27–0.71]** | 27 M | checksum=9.164e+07 |
| Grain1 | 223 ns | 18.2% | **0.09× [0.06–0.12]** | 4.48 M | checksum=9.046e+07 |
| Grain64 | 26.5 ns | 18.3% | **0.56× [0.49–0.79]** | 37.8 M | checksum=9.144e+07 |
| Grain1024 | 20.5 ns | 7.3% | **0.78× [0.65–0.97]** | 48.8 M | checksum=9.155e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.3 ns | 4.2% | baseline | 61.3 M | checksum=8.86e+07 |
| Each | 22.4 ns | 9.1% | 0.77× [0.62–1.02] | 44.7 M | checksum=8.865e+07 |
| Grain1 | 160 ns | 5.0% | **0.11× [0.10–0.13]** | 6.25 M | checksum=8.751e+07 |
| Grain64 | 26.2 ns | 8.1% | **0.62× [0.55–0.84]** | 38.1 M | checksum=8.818e+07 |
| Grain1024 | 21.4 ns | 5.8% | **0.78× [0.68–0.98]** | 46.7 M | checksum=8.845e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 783 ns | 8.0% | baseline | 1.28 M | checksum=2.814e+08 |
| Each | 1.3 µs | 8.0% | **0.60× [0.53–0.65]** | 771 k | checksum=2.797e+08 |
| Grain1 | 9.77 µs | 7.6% | **0.08× [0.07–0.09]** | 102 k | checksum=2.775e+08 |
| Grain64 | 1.18 µs | 8.6% | **0.69× [0.62–0.78]** | 845 k | checksum=2.787e+08 |
| Grain1024 | 773 ns | 10.8% | 0.96× [0.76–1.10] | 1.29 M | checksum=2.817e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 748 ns | 2.9% | baseline | 1.34 M | checksum=2.764e+08 |
| Each | 1.22 µs | 3.8% | **0.62× [0.58–0.67]** | 819 k | checksum=2.754e+08 |
| Grain1 | 9.07 µs | 1.7% | **0.08× [0.08–0.09]** | 110 k | checksum=2.731e+08 |
| Grain64 | 1.05 µs | 2.2% | **0.72× [0.67–0.74]** | 952 k | checksum=2.755e+08 |
| Grain1024 | 711 ns | 3.3% | 1.04× [0.97–1.14] | 1.41 M | checksum=2.772e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 23 µs | 4.8% | baseline | 43.5 k | checksum=5.23e+09 |
| Each | 29.4 µs | 2.9% | **0.81× [0.74–0.87]** | 34.1 k | checksum=5.229e+09 |
| Grain1 | 216 µs | 1.1% | **0.11× [0.10–0.12]** | 4.63 k | checksum=5.226e+09 |
| Grain64 | 27.4 µs | 1.6% | **0.83× [0.79–0.90]** | 36.5 k | checksum=5.229e+09 |
| Grain1024 | 21.9 µs | 2.4% | 1.04× [0.98–1.17] | 45.7 k | checksum=5.229e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.3 µs | 4.2% | baseline | 44.8 k | checksum=5.225e+09 |
| Each | 29.1 µs | 2.7% | **0.76× [0.71–0.79]** | 34.3 k | checksum=5.225e+09 |
| Grain1 | 216 µs | 1.9% | **0.10× [0.09–0.11]** | 4.64 k | checksum=5.222e+09 |
| Grain64 | 27.6 µs | 2.9% | **0.79× [0.71–0.84]** | 36.2 k | checksum=5.225e+09 |
| Grain1024 | 23.5 µs | 3.8% | 0.94× [0.89–1.01] | 42.6 k | checksum=5.226e+09 |

