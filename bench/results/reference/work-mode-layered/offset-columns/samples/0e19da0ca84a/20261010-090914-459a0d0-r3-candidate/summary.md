# Benchmark run 20261010-090914-459a0d0-r3-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.3 ns | 3.4% | baseline | 70.1 M | checksum=9.155e+07 |
| Each | 16.7 ns | 5.6% | **0.85× [0.64–0.95]** | 59.8 M | checksum=9.165e+07 |
| Grain1 | 17.6 ns | 4.6% | **0.81× [0.72–0.88]** | 56.9 M | checksum=9.16e+07 |
| Grain64 | 23.7 ns | 6.2% | **0.60× [0.48–0.64]** | 42.2 M | checksum=9.14e+07 |
| Grain1024 | 23.5 ns | 8.0% | **0.61× [0.47–0.67]** | 42.5 M | checksum=9.161e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 ns | 3.7% | baseline | 62.1 M | checksum=9.088e+07 |
| Each | 19.8 ns | 5.9% | **0.82× [0.76–0.99]** | 50.4 M | checksum=9.084e+07 |
| Grain1 | 150 ns | 2.3% | **0.11× [0.10–0.13]** | 6.65 M | checksum=8.969e+07 |
| Grain64 | 27.7 ns | 8.0% | **0.61× [0.52–0.70]** | 36.1 M | checksum=9.067e+07 |
| Grain1024 | 27.2 ns | 7.7% | **0.62× [0.52–0.67]** | 36.7 M | checksum=9.073e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 741 ns | 5.4% | baseline | 1.35 M | checksum=2.938e+08 |
| Each | 685 ns | 5.5% | 1.06× [0.89–1.16] | 1.46 M | checksum=2.953e+08 |
| Grain1 | 667 ns | 1.7% | 1.10× [1.00–1.19] | 1.5 M | checksum=2.952e+08 |
| Grain64 | 712 ns | 2.2% | 1.01× [0.96–1.13] | 1.4 M | checksum=2.95e+08 |
| Grain1024 | 718 ns | 6.7% | 1.06× [0.93–1.13] | 1.39 M | checksum=2.952e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 730 ns | 6.8% | baseline | 1.37 M | checksum=2.826e+08 |
| Each | 700 ns | 5.7% | 1.04× [0.85–1.29] | 1.43 M | checksum=2.8e+08 |
| Grain1 | 9.58 µs | 3.1% | **0.08× [0.07–0.08]** | 104 k | checksum=2.784e+08 |
| Grain64 | 1.15 µs | 6.6% | **0.66× [0.56–0.72]** | 867 k | checksum=2.811e+08 |
| Grain1024 | 744 ns | 7.0% | 0.96× [0.82–1.11] | 1.34 M | checksum=2.826e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.7 µs | 7.1% | baseline | 44.1 k | checksum=5.234e+09 |
| Each | 22.3 µs | 7.3% | 0.98× [0.88–1.19] | 44.9 k | checksum=5.235e+09 |
| Grain1 | 21.5 µs | 3.9% | 1.03× [0.86–1.15] | 46.5 k | checksum=5.235e+09 |
| Grain64 | 21.7 µs | 5.0% | 1.03× [0.95–1.10] | 46.1 k | checksum=5.235e+09 |
| Grain1024 | 22.1 µs | 6.0% | 1.01× [0.93–1.12] | 45.2 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 23 µs | 7.8% | baseline | 43.5 k | checksum=5.23e+09 |
| Each | 23.6 µs | 6.6% | 0.98× [0.90–1.05] | 42.3 k | checksum=5.23e+09 |
| Grain1 | 254 µs | 5.1% | **0.09× [0.08–0.10]** | 3.94 k | checksum=5.227e+09 |
| Grain64 | 32.4 µs | 12.2% | **0.75× [0.53–0.79]** | 30.8 k | checksum=5.228e+09 |
| Grain1024 | 24.3 µs | 9.4% | 0.93× [0.71–1.06] | 41.2 k | checksum=5.229e+09 |

