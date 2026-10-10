# Benchmark run 20261010-011645-459a0d0-r1-candidate

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.3 ns | 3.2% | baseline | 69.8 M | checksum=9.187e+07 |
| Each | 15.9 ns | 2.1% | **0.92× [0.87–0.97]** | 62.9 M | checksum=9.211e+07 |
| Grain1 | 46.3 ns | 4.9% | **0.31× [0.28–0.34]** | 21.6 M | checksum=9.141e+07 |
| Grain64 | 17.7 ns | 4.2% | **0.81× [0.76–0.88]** | 56.5 M | checksum=9.218e+07 |
| Grain1024 | 18.1 ns | 1.3% | **0.80× [0.73–0.83]** | 55.1 M | checksum=9.218e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.2 ns | 2.1% | baseline | 65.7 M | checksum=9.243e+07 |
| Each | 19 ns | 2.5% | **0.79× [0.77–0.83]** | 52.5 M | checksum=9.151e+07 |
| Grain1 | 145 ns | 2.9% | **0.10× [0.10–0.11]** | 6.89 M | checksum=9.114e+07 |
| Grain64 | 24.8 ns | 2.8% | **0.61× [0.57–0.63]** | 40.3 M | checksum=9.193e+07 |
| Grain1024 | 25 ns | 1.6% | **0.60× [0.57–0.63]** | 39.9 M | checksum=9.227e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 679 ns | 1.1% | baseline | 1.47 M | checksum=2.932e+08 |
| Each | 651 ns | 0.3% | 1.04× [1.00–1.11] | 1.54 M | checksum=2.95e+08 |
| Grain1 | 2.53 µs | 3.7% | **0.27× [0.26–0.29]** | 395 k | checksum=2.916e+08 |
| Grain64 | 1.25 µs | 3.1% | **0.55× [0.50–0.58]** | 798 k | checksum=2.928e+08 |
| Grain1024 | 719 ns | 0.6% | **0.94× [0.88–0.99]** | 1.39 M | checksum=2.941e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 677 ns | 0.8% | baseline | 1.48 M | checksum=2.812e+08 |
| Each | 696 ns | 2.7% | 0.98× [0.92–1.00] | 1.44 M | checksum=2.813e+08 |
| Grain1 | 8.99 µs | 1.2% | **0.08× [0.07–0.08]** | 111 k | checksum=2.772e+08 |
| Grain64 | 1.04 µs | 1.1% | **0.65× [0.59–0.67]** | 960 k | checksum=2.795e+08 |
| Grain1024 | 695 ns | 1.4% | 0.98× [0.95–1.01] | 1.44 M | checksum=2.813e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.1 µs | 8.2% | baseline | 45.3 k | checksum=5.23e+09 |
| Each | 23 µs | 10.8% | 0.99× [0.84–1.12] | 43.5 k | checksum=5.23e+09 |
| Grain1 | 64.3 µs | 5.6% | **0.33× [0.26–0.37]** | 15.6 k | checksum=5.228e+09 |
| Grain64 | 30.2 µs | 2.7% | **0.69× [0.55–0.75]** | 33.1 k | checksum=5.229e+09 |
| Grain1024 | 21.8 µs | 6.7% | 0.99× [0.88–1.04] | 45.9 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.5 µs | 3.2% | baseline | 44.4 k | checksum=5.235e+09 |
| Each | 22.5 µs | 7.0% | 1.02× [0.91–1.08] | 44.4 k | checksum=5.234e+09 |
| Grain1 | 220 µs | 1.7% | **0.10× [0.10–0.11]** | 4.54 k | checksum=5.231e+09 |
| Grain64 | 28.4 µs | 2.1% | **0.78× [0.73–0.91]** | 35.2 k | checksum=5.233e+09 |
| Grain1024 | 23.4 µs | 7.3% | 0.94× [0.88–1.03] | 42.7 k | checksum=5.235e+09 |

