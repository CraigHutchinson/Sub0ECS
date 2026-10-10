# Benchmark run 20261010-090902-459a0d0-r1-control

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 ns | 8.7% | baseline | 64.8 M | checksum=9.399e+07 |
| Each | 22.9 ns | 9.4% | **0.69× [0.56–0.82]** | 43.7 M | checksum=9.379e+07 |
| Grain1 | 152 ns | 3.2% | **0.10× [0.10–0.12]** | 6.59 M | checksum=9.26e+07 |
| Grain64 | 24.2 ns | 8.2% | **0.69× [0.54–0.76]** | 41.4 M | checksum=9.389e+07 |
| Grain1024 | 18.2 ns | 2.3% | 0.85× [0.77–1.00] | 55 M | checksum=9.392e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.3 ns | 4.1% | baseline | 61.5 M | checksum=8.74e+07 |
| Each | 24 ns | 5.9% | **0.68× [0.61–0.94]** | 41.6 M | checksum=8.665e+07 |
| Grain1 | 153 ns | 6.0% | **0.11× [0.10–0.11]** | 6.52 M | checksum=8.625e+07 |
| Grain64 | 25.1 ns | 3.5% | **0.65× [0.58–0.76]** | 39.9 M | checksum=8.734e+07 |
| Grain1024 | 22.9 ns | 7.9% | **0.71× [0.61–0.90]** | 43.7 M | checksum=8.721e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 799 ns | 10.4% | baseline | 1.25 M | checksum=2.803e+08 |
| Each | 1.26 µs | 6.7% | **0.59× [0.48–0.67]** | 796 k | checksum=2.784e+08 |
| Grain1 | 10 µs | 7.2% | **0.08× [0.07–0.08]** | 99.6 k | checksum=2.763e+08 |
| Grain64 | 1.19 µs | 11.0% | **0.63× [0.56–0.77]** | 840 k | checksum=2.788e+08 |
| Grain1024 | 731 ns | 5.6% | 0.99× [0.85–1.09] | 1.37 M | checksum=2.803e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 675 ns | 1.2% | baseline | 1.48 M | checksum=2.854e+08 |
| Each | 1.17 µs | 1.2% | **0.58× [0.57–0.59]** | 857 k | checksum=2.835e+08 |
| Grain1 | 8.66 µs | 1.0% | **0.08× [0.08–0.09]** | 115 k | checksum=2.812e+08 |
| Grain64 | 1.05 µs | 1.8% | **0.64× [0.63–0.67]** | 953 k | checksum=2.836e+08 |
| Grain1024 | 691 ns | 1.0% | 0.98× [0.96–1.13] | 1.45 M | checksum=2.846e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 25 µs | 10.2% | baseline | 39.9 k | checksum=5.235e+09 |
| Each | 33.6 µs | 14.5% | **0.68× [0.53–0.80]** | 29.8 k | checksum=5.233e+09 |
| Grain1 | 249 µs | 11.1% | **0.09× [0.08–0.10]** | 4.02 k | checksum=5.231e+09 |
| Grain64 | 28.2 µs | 8.2% | **0.77× [0.68–0.97]** | 35.5 k | checksum=5.234e+09 |
| Grain1024 | 24.1 µs | 11.3% | 0.99× [0.90–1.14] | 41.5 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.8 µs | 6.4% | baseline | 43.9 k | checksum=5.17e+09 |
| Each | 32.3 µs | 7.7% | **0.75× [0.58–0.95]** | 31 k | checksum=5.171e+09 |
| Grain1 | 255 µs | 5.7% | **0.09× [0.08–0.13]** | 3.92 k | checksum=5.168e+09 |
| Grain64 | 31.9 µs | 12.6% | **0.76× [0.62–0.89]** | 31.4 k | checksum=5.171e+09 |
| Grain1024 | 25.7 µs | 8.9% | 0.99× [0.75–1.22] | 38.9 k | checksum=5.168e+09 |

