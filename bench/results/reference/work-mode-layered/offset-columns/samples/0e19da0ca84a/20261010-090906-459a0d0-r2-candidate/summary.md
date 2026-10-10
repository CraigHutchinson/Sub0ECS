# Benchmark run 20261010-090906-459a0d0-r2-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.6 ns | 4.7% | baseline | 68.7 M | checksum=8.955e+07 |
| Each | 16.6 ns | 4.6% | **0.88× [0.83–0.98]** | 60.4 M | checksum=9.009e+07 |
| Grain1 | 18.6 ns | 10.2% | **0.78× [0.67–0.94]** | 53.9 M | checksum=8.951e+07 |
| Grain64 | 23.7 ns | 6.0% | **0.63× [0.54–0.67]** | 42.2 M | checksum=9.013e+07 |
| Grain1024 | 22.4 ns | 7.0% | **0.67× [0.46–0.77]** | 44.7 M | checksum=8.93e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.3 ns | 4.6% | baseline | 57.9 M | checksum=9.05e+07 |
| Each | 19.8 ns | 6.2% | **0.86× [0.69–0.96]** | 50.4 M | checksum=9.046e+07 |
| Grain1 | 158 ns | 3.9% | **0.11× [0.10–0.13]** | 6.32 M | checksum=8.931e+07 |
| Grain64 | 28.1 ns | 8.0% | **0.61× [0.53–0.72]** | 35.6 M | checksum=9.025e+07 |
| Grain1024 | 26.9 ns | 7.0% | **0.65× [0.57–0.73]** | 37.2 M | checksum=9.033e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 729 ns | 5.0% | baseline | 1.37 M | checksum=2.945e+08 |
| Each | 696 ns | 5.5% | 1.02× [0.95–1.19] | 1.44 M | checksum=2.948e+08 |
| Grain1 | 734 ns | 9.0% | 1.03× [0.92–1.11] | 1.36 M | checksum=2.932e+08 |
| Grain64 | 754 ns | 5.8% | 1.00× [0.75–1.09] | 1.33 M | checksum=2.944e+08 |
| Grain1024 | 729 ns | 7.9% | 1.04× [0.89–1.07] | 1.37 M | checksum=2.947e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 785 ns | 9.3% | baseline | 1.27 M | checksum=2.845e+08 |
| Each | 805 ns | 4.9% | 0.98× [0.82–1.05] | 1.24 M | checksum=2.838e+08 |
| Grain1 | 10 µs | 5.2% | **0.08× [0.07–0.09]** | 99.8 k | checksum=2.802e+08 |
| Grain64 | 1.25 µs | 6.6% | **0.67× [0.56–0.78]** | 802 k | checksum=2.828e+08 |
| Grain1024 | 751 ns | 7.9% | 1.04× [0.91–1.14] | 1.33 M | checksum=2.833e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.3 µs | 7.4% | baseline | 44.8 k | checksum=5.233e+09 |
| Each | 22.5 µs | 10.9% | 1.01× [0.67–1.33] | 44.5 k | checksum=5.234e+09 |
| Grain1 | 22.5 µs | 5.4% | 0.98× [0.86–1.30] | 44.5 k | checksum=5.235e+09 |
| Grain64 | 22.4 µs | 6.6% | 1.01× [0.86–1.34] | 44.7 k | checksum=5.234e+09 |
| Grain1024 | 24.3 µs | 14.7% | 0.92× [0.66–1.10] | 41.2 k | checksum=5.233e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.4 µs | 7.8% | baseline | 44.7 k | checksum=5.23e+09 |
| Each | 22.3 µs | 5.5% | 1.04× [0.89–1.12] | 44.7 k | checksum=5.23e+09 |
| Grain1 | 237 µs | 2.1% | **0.09× [0.09–0.11]** | 4.21 k | checksum=5.227e+09 |
| Grain64 | 29.1 µs | 8.9% | **0.74× [0.59–0.87]** | 34.3 k | checksum=5.229e+09 |
| Grain1024 | 23 µs | 7.4% | 1.01× [0.88–1.12] | 43.6 k | checksum=5.23e+09 |

