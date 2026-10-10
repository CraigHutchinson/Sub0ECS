# Benchmark run 20261010-011643-459a0d0-r1-control

- rows: ok in 1 s

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 ns | 7.3% | baseline | 65 M | checksum=9.157e+07 |
| Each | 21.4 ns | 2.1% | **0.72× [0.64–0.85]** | 46.8 M | checksum=9.165e+07 |
| Grain1 | 156 ns | 9.5% | **0.11× [0.09–0.12]** | 6.4 M | checksum=9.038e+07 |
| Grain64 | 22.7 ns | 3.3% | **0.67× [0.56–0.73]** | 44.1 M | checksum=9.158e+07 |
| Grain1024 | 18.1 ns | 3.3% | **0.83× [0.78–0.97]** | 55.3 M | checksum=9.127e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.5 ns | 1.4% | baseline | 64.7 M | checksum=9.242e+07 |
| Each | 20.4 ns | 2.0% | **0.74× [0.72–0.78]** | 48.9 M | checksum=9.234e+07 |
| Grain1 | 143 ns | 1.9% | **0.11× [0.11–0.11]** | 7.01 M | checksum=9.12e+07 |
| Grain64 | 24.6 ns | 3.3% | **0.62× [0.56–0.66]** | 40.6 M | checksum=9.225e+07 |
| Grain1024 | 20.6 ns | 2.4% | **0.74× [0.71–0.79]** | 48.5 M | checksum=9.229e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 678 ns | 0.7% | baseline | 1.47 M | checksum=2.797e+08 |
| Each | 1.16 µs | 1.7% | **0.59× [0.55–0.65]** | 866 k | checksum=2.775e+08 |
| Grain1 | 9.01 µs | 2.4% | **0.08× [0.07–0.08]** | 111 k | checksum=2.757e+08 |
| Grain64 | 1.07 µs | 2.9% | **0.66× [0.59–0.72]** | 939 k | checksum=2.783e+08 |
| Grain1024 | 724 ns | 5.9% | 0.99× [0.87–1.11] | 1.38 M | checksum=2.798e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 663 ns | 1.4% | baseline | 1.51 M | checksum=2.871e+08 |
| Each | 1.2 µs | 3.8% | **0.56× [0.51–0.58]** | 835 k | checksum=2.852e+08 |
| Grain1 | 9.14 µs | 3.2% | **0.07× [0.07–0.08]** | 109 k | checksum=2.829e+08 |
| Grain64 | 1.15 µs | 9.3% | **0.61× [0.54–0.64]** | 872 k | checksum=2.846e+08 |
| Grain1024 | 703 ns | 1.6% | **0.94× [0.78–0.97]** | 1.42 M | checksum=2.869e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.2 µs | 1.0% | baseline | 47.3 k | checksum=5.23e+09 |
| Each | 30 µs | 6.3% | **0.71× [0.65–0.76]** | 33.3 k | checksum=5.229e+09 |
| Grain1 | 226 µs | 3.9% | **0.10× [0.09–0.10]** | 4.42 k | checksum=5.226e+09 |
| Grain64 | 26.9 µs | 1.7% | **0.79× [0.67–0.82]** | 37.1 k | checksum=5.229e+09 |
| Grain1024 | 21.7 µs | 3.2% | 0.98× [0.94–1.04] | 46.2 k | checksum=5.226e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.5 µs | 1.9% | baseline | 48.8 k | checksum=5.235e+09 |
| Each | 29.8 µs | 6.0% | **0.71× [0.65–0.74]** | 33.6 k | checksum=5.234e+09 |
| Grain1 | 217 µs | 2.4% | **0.09× [0.09–0.10]** | 4.61 k | checksum=5.231e+09 |
| Grain64 | 26.9 µs | 1.3% | **0.76× [0.72–0.79]** | 37.2 k | checksum=5.234e+09 |
| Grain1024 | 21.7 µs | 3.0% | **0.95× [0.91–0.98]** | 46 k | checksum=5.234e+09 |

