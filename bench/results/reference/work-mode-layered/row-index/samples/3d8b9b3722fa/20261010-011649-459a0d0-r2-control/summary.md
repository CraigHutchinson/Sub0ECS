# Benchmark run 20261010-011649-459a0d0-r2-control

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 13.9 ns | 2.4% | baseline | 71.7 M | checksum=8.836e+07 |
| Each | 21.1 ns | 2.6% | **0.66× [0.58–0.69]** | 47.4 M | checksum=8.843e+07 |
| Grain1 | 146 ns | 4.4% | **0.10× [0.09–0.10]** | 6.83 M | checksum=8.751e+07 |
| Grain64 | 22.8 ns | 1.8% | **0.60× [0.58–0.63]** | 43.8 M | checksum=8.863e+07 |
| Grain1024 | 17.9 ns | 2.9% | **0.78× [0.73–0.81]** | 55.9 M | checksum=8.864e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.1 ns | 1.1% | baseline | 66.1 M | checksum=9.243e+07 |
| Each | 20.5 ns | 2.3% | **0.73× [0.61–0.77]** | 48.8 M | checksum=9.231e+07 |
| Grain1 | 143 ns | 2.4% | **0.11× [0.10–0.11]** | 7.01 M | checksum=9.123e+07 |
| Grain64 | 24.8 ns | 2.9% | **0.61× [0.56–0.63]** | 40.4 M | checksum=9.227e+07 |
| Grain1024 | 20.8 ns | 3.6% | **0.71× [0.66–0.75]** | 48 M | checksum=9.23e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 683 ns | 1.2% | baseline | 1.46 M | checksum=2.785e+08 |
| Each | 1.16 µs | 1.6% | **0.59× [0.56–0.66]** | 865 k | checksum=2.77e+08 |
| Grain1 | 8.97 µs | 3.1% | **0.08× [0.07–0.08]** | 111 k | checksum=2.748e+08 |
| Grain64 | 1.05 µs | 2.1% | **0.65× [0.59–0.71]** | 951 k | checksum=2.774e+08 |
| Grain1024 | 691 ns | 1.0% | 0.99× [0.90–1.03] | 1.45 M | checksum=2.788e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 670 ns | 0.3% | baseline | 1.49 M | checksum=2.811e+08 |
| Each | 1.17 µs | 1.4% | **0.57× [0.54–0.58]** | 854 k | checksum=2.791e+08 |
| Grain1 | 8.8 µs | 1.2% | **0.08× [0.08–0.08]** | 114 k | checksum=2.77e+08 |
| Grain64 | 1.06 µs | 1.2% | **0.63× [0.56–0.64]** | 945 k | checksum=2.795e+08 |
| Grain1024 | 693 ns | 0.8% | **0.97× [0.93–0.98]** | 1.44 M | checksum=2.81e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.9 µs | 1.6% | baseline | 47.8 k | checksum=5.235e+09 |
| Each | 28.4 µs | 1.5% | **0.74× [0.72–0.77]** | 35.2 k | checksum=5.233e+09 |
| Grain1 | 216 µs | 1.8% | **0.10× [0.09–0.10]** | 4.63 k | checksum=5.231e+09 |
| Grain64 | 27.3 µs | 2.0% | **0.78× [0.74–0.81]** | 36.6 k | checksum=5.234e+09 |
| Grain1024 | 21.4 µs | 1.6% | 0.99× [0.96–1.02] | 46.7 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.6 µs | 1.3% | baseline | 48.5 k | checksum=5.235e+09 |
| Each | 28.1 µs | 0.6% | **0.73× [0.72–0.75]** | 35.6 k | checksum=5.234e+09 |
| Grain1 | 212 µs | 0.7% | **0.10× [0.09–0.10]** | 4.71 k | checksum=5.231e+09 |
| Grain64 | 26.8 µs | 1.1% | **0.77× [0.75–0.79]** | 37.4 k | checksum=5.234e+09 |
| Grain1024 | 21.3 µs | 1.7% | **0.97× [0.92–0.99]** | 46.9 k | checksum=5.234e+09 |

