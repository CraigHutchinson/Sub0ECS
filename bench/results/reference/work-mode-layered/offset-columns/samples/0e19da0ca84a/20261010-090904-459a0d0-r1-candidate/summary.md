# Benchmark run 20261010-090904-459a0d0-r1-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.6 ns | 18.0% | baseline | 60.2 M | checksum=7.765e+07 |
| Each | 19.4 ns | 10.6% | 0.83× [0.43–1.15] | 51.4 M | checksum=7.707e+07 |
| Grain1 | 19.9 ns | 10.1% | 0.81× [0.65–1.19] | 50.2 M | checksum=7.754e+07 |
| Grain64 | 24.3 ns | 10.6% | **0.63× [0.50–0.93]** | 41.1 M | checksum=7.712e+07 |
| Grain1024 | 24.4 ns | 8.3% | 0.64× [0.52–1.04] | 41.1 M | checksum=7.742e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.2 ns | 13.0% | baseline | 52 M | checksum=9.204e+07 |
| Each | 22.3 ns | 6.7% | **0.87× [0.74–1.00]** | 44.8 M | checksum=9.136e+07 |
| Grain1 | 153 ns | 4.6% | **0.12× [0.10–0.14]** | 6.53 M | checksum=9.081e+07 |
| Grain64 | 30.1 ns | 11.9% | **0.61× [0.48–0.77]** | 33.3 M | checksum=9.189e+07 |
| Grain1024 | 27.9 ns | 9.4% | **0.66× [0.54–0.79]** | 35.9 M | checksum=9.187e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 823 ns | 5.8% | baseline | 1.22 M | checksum=2.91e+08 |
| Each | 757 ns | 8.9% | 1.07× [0.87–1.26] | 1.32 M | checksum=2.896e+08 |
| Grain1 | 736 ns | 8.8% | 1.14× [0.88–1.26] | 1.36 M | checksum=2.922e+08 |
| Grain64 | 783 ns | 9.6% | 1.06× [0.71–1.22] | 1.28 M | checksum=2.911e+08 |
| Grain1024 | 747 ns | 11.2% | 1.11× [0.92–1.24] | 1.34 M | checksum=2.913e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 845 ns | 17.5% | baseline | 1.18 M | checksum=2.86e+08 |
| Each | 696 ns | 6.3% | 1.19× [0.95–1.50] | 1.44 M | checksum=2.84e+08 |
| Grain1 | 10.9 µs | 7.7% | **0.08× [0.07–0.09]** | 91.4 k | checksum=2.818e+08 |
| Grain64 | 1.24 µs | 9.0% | **0.75× [0.59–0.81]** | 808 k | checksum=2.83e+08 |
| Grain1024 | 767 ns | 11.0% | 1.05× [0.94–1.22] | 1.3 M | checksum=2.853e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 25 µs | 10.3% | baseline | 40 k | checksum=5.221e+09 |
| Each | 24.3 µs | 10.3% | 0.98× [0.81–1.18] | 41.1 k | checksum=5.221e+09 |
| Grain1 | 25.7 µs | 18.0% | 1.04× [0.84–1.11] | 38.9 k | checksum=5.217e+09 |
| Grain64 | 23.7 µs | 9.7% | 1.03× [0.88–1.24] | 42.1 k | checksum=5.22e+09 |
| Grain1024 | 23.3 µs | 4.9% | 1.03× [0.80–1.18] | 43 k | checksum=5.22e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 26.8 µs | 7.3% | baseline | 37.3 k | checksum=5.235e+09 |
| Each | 23.5 µs | 12.0% | 1.12× [0.97–1.35] | 42.5 k | checksum=5.235e+09 |
| Grain1 | 268 µs | 5.2% | **0.09× [0.08–0.11]** | 3.73 k | checksum=5.231e+09 |
| Grain64 | 31.5 µs | 7.4% | 0.83× [0.72–1.00] | 31.8 k | checksum=5.234e+09 |
| Grain1024 | 24.5 µs | 12.9% | 1.03× [0.90–1.21] | 40.9 k | checksum=5.234e+09 |

