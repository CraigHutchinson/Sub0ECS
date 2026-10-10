# Benchmark run 20261010-090911-459a0d0-r3-control

- rows: ok in 2 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 ns | 11.9% | baseline | 62.8 M | checksum=9.132e+07 |
| Each | 28.6 ns | 27.9% | **0.63× [0.42–0.89]** | 34.9 M | checksum=9.164e+07 |
| Grain1 | 160 ns | 5.9% | **0.10× [0.07–0.13]** | 6.25 M | checksum=9.043e+07 |
| Grain64 | 25.5 ns | 11.5% | **0.61× [0.51–0.79]** | 39.3 M | checksum=9.161e+07 |
| Grain1024 | 20.2 ns | 12.8% | **0.80× [0.54–0.98]** | 49.6 M | checksum=9.119e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.2 ns | 6.7% | baseline | 58 M | checksum=8.649e+07 |
| Each | 21.3 ns | 3.4% | **0.77× [0.73–0.83]** | 46.9 M | checksum=8.65e+07 |
| Grain1 | 154 ns | 5.0% | **0.11× [0.10–0.12]** | 6.5 M | checksum=8.54e+07 |
| Grain64 | 26.6 ns | 5.7% | **0.62× [0.55–0.73]** | 37.6 M | checksum=8.637e+07 |
| Grain1024 | 21.5 ns | 4.5% | **0.76× [0.72–0.91]** | 46.4 M | checksum=8.632e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 762 ns | 9.5% | baseline | 1.31 M | checksum=2.628e+08 |
| Each | 1.25 µs | 9.5% | **0.62× [0.48–0.80]** | 802 k | checksum=2.618e+08 |
| Grain1 | 10 µs | 2.9% | **0.08× [0.07–0.10]** | 99.8 k | checksum=2.595e+08 |
| Grain64 | 1.12 µs | 6.4% | **0.68× [0.62–0.81]** | 895 k | checksum=2.616e+08 |
| Grain1024 | 738 ns | 6.7% | 1.09× [0.81–1.43] | 1.35 M | checksum=2.633e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 761 ns | 12.4% | baseline | 1.31 M | checksum=2.852e+08 |
| Each | 1.37 µs | 6.7% | **0.56× [0.49–0.66]** | 731 k | checksum=2.829e+08 |
| Grain1 | 9.91 µs | 3.0% | **0.07× [0.07–0.10]** | 101 k | checksum=2.81e+08 |
| Grain64 | 1.24 µs | 6.2% | **0.62× [0.53–0.80]** | 809 k | checksum=2.819e+08 |
| Grain1024 | 848 ns | 7.6% | 0.84× [0.68–1.20] | 1.18 M | checksum=2.85e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 26.2 µs | 8.0% | baseline | 38.1 k | checksum=5.235e+09 |
| Each | 35.1 µs | 25.2% | **0.66× [0.50–0.83]** | 28.5 k | checksum=5.233e+09 |
| Grain1 | 310 µs | 15.5% | **0.08× [0.08–0.11]** | 3.22 k | checksum=5.231e+09 |
| Grain64 | 34.1 µs | 15.6% | 0.81× [0.59–1.01] | 29.3 k | checksum=5.232e+09 |
| Grain1024 | 25 µs | 12.1% | 0.99× [0.90–1.27] | 40 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 25.3 µs | 9.0% | baseline | 39.5 k | checksum=5.235e+09 |
| Each | 33.9 µs | 14.7% | **0.75× [0.53–0.89]** | 29.5 k | checksum=5.233e+09 |
| Grain1 | 257 µs | 11.2% | **0.10× [0.08–0.11]** | 3.9 k | checksum=5.231e+09 |
| Grain64 | 33.5 µs | 15.2% | **0.76× [0.66–0.81]** | 29.9 k | checksum=5.233e+09 |
| Grain1024 | 25.2 µs | 10.3% | 0.99× [0.72–1.10] | 39.7 k | checksum=5.234e+09 |

