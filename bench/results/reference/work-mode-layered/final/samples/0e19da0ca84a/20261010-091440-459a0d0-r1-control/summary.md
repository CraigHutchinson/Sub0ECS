# Benchmark run 20261010-091440-459a0d0-r1-control

- rows: ok in 1 s
- nbody: ok in 22 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 1.3% | baseline | 62.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.7 µs | 1.2% | 1.01× [0.41–2.59] | 63.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.5 µs | 0.3% | **1.02× [1.01–2.62]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.6 µs | 0.6% | 1.02× [0.67–1.03] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.5 µs | 0.3% | 1.01× [1.00–2.57] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.6 µs | 0.5% | 1.02× [0.99–2.63] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.5% | 1.01× [0.17–2.62] | 64 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.6 µs | 0.7% | 1.02× [0.96–2.59] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.9 µs | 2.2% | 1.01× [0.93–2.45] | 63 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.02× [1.01–2.63]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.6 µs | 0.4% | **1.02× [1.00–2.57]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.4% | **1.02× [1.01–2.59]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.6 µs | 0.9% | **1.02× [1.01–2.58]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.6 µs | 0.8% | 1.02× [0.81–2.60] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.6% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | **1.03× [1.01–1.05]** | 64.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.04× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.6% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | **1.04× [1.03–1.05]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.47 ms | 4.5% | baseline | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.41 ms | 3.1% | 1.01× [0.95–1.17] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.38 ms | 2.8% | 1.03× [0.96–1.07] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.2 ms | 3.3% | **1.07× [1.01–1.19]** | 238 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.39 ms | 4.4% | 1.04× [0.99–1.10] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.31 ms | 1.6% | 1.06× [0.98–1.15] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.45 ms | 2.1% | 1.06× [0.95–1.16] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.35 ms | 3.7% | 1.07× [0.95–1.18] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.44 ms | 6.4% | **1.79× [1.21–2.10]** | 410 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.32 ms | 7.7% | **1.95× [1.02–2.24]** | 431 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.31 ms | 2.2% | 1.06× [0.99–1.16] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.33 ms | 5.4% | 1.07× [0.93–1.13] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.57 ms | 23.2% | **3.07× [2.01–4.09]** | 638 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.12 ms | 8.1% | **2.10× [1.75–3.74]** | 472 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.48 ms | 5.7% | baseline | 223 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.58 ms | 4.7% | 0.95× [0.88–1.06] | 218 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.4 ms | 3.1% | baseline | 227 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.48 ms | 3.0% | 0.96× [0.94–1.06] | 223 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.29 ms | 5.4% | baseline | 437 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.34 ms | 4.2% | 1.01× [0.92–1.69] | 427 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.17 ms | 0.8% | baseline | 240 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.59 ms | 8.9% | 1.43× [0.98–1.76] | 387 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.42 ms | 27.5% | baseline | 413 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.02 ms | 22.1% | 1.32× [0.75–1.85] | 494 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.08 ms | 7.6% | baseline | 481 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.04 ms | 2.6% | 1.04× [0.99–1.13] | 490 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.8 ms | 3.7% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.1 ms | 2.9% | 0.97× [0.88–1.02] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.2 ms | 4.0% | 1.01× [0.84–1.06] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 72 ms | 2.4% | 0.97× [0.87–1.03] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 72.1 ms | 2.0% | 0.98× [0.87–1.05] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 69.2 ms | 2.0% | 1.01× [0.90–1.07] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.4 ms | 4.0% | **1.94× [1.77–2.10]** | 27.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 37.4 ms | 8.2% | **1.87× [1.17–2.06]** | 26.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.9 ms | 2.4% | **1.94× [1.55–2.07]** | 27.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 41.2 ms | 18.9% | **1.84× [1.18–2.03]** | 24.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 29.1 ms | 11.5% | **2.73× [2.11–4.08]** | 34.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 3.3% | **2.26× [2.04–4.07]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 20.9 ms | 11.1% | **3.63× [2.93–3.97]** | 47.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 29.9 ms | 9.8% | **2.58× [2.10–3.83]** | 33.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 74.5 ms | 4.6% | baseline | 13.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.6 ms | 3.0% | 1.01× [0.99–1.05] | 13.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 77.4 ms | 4.6% | baseline | 12.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 75 ms | 1.7% | 1.03× [0.91–1.08] | 13.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 56 ms | 11.1% | baseline | 17.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 42.7 ms | 14.9% | 1.20× [0.94–1.51] | 23.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 55.1 ms | 17.3% | baseline | 18.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 54.2 ms | 16.2% | 1.11× [0.75–1.33] | 18.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32.4 ms | 0.3% | baseline | 30.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.4 ms | 1.5% | 1.00× [0.98–1.20] | 30.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 33 ms | 0.7% | baseline | 30.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.7 ms | 0.7% | **1.01× [1.00–1.03]** | 30.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.1 ns | 7.3% | baseline | 66.1 M | checksum=9.402e+07 |
| Each | 22.6 ns | 9.6% | **0.72× [0.58–0.81]** | 44.3 M | checksum=9.374e+07 |
| Grain1 | 157 ns | 5.4% | **0.10× [0.09–0.13]** | 6.36 M | checksum=9.273e+07 |
| Grain64 | 26.3 ns | 12.4% | **0.59× [0.48–0.82]** | 38 M | checksum=9.392e+07 |
| Grain1024 | 19.9 ns | 10.6% | **0.82× [0.66–0.91]** | 50.4 M | checksum=9.396e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.6 ns | 16.1% | baseline | 53.7 M | checksum=8.845e+07 |
| Each | 20.9 ns | 4.5% | 0.89× [0.68–1.08] | 47.7 M | checksum=8.846e+07 |
| Grain1 | 148 ns | 5.7% | **0.13× [0.11–0.16]** | 6.74 M | checksum=8.721e+07 |
| Grain64 | 25.7 ns | 6.0% | **0.73× [0.62–0.93]** | 39 M | checksum=8.805e+07 |
| Grain1024 | 21.3 ns | 5.2% | 0.83× [0.66–1.08] | 47 M | checksum=8.836e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 723 ns | 5.6% | baseline | 1.38 M | checksum=2.812e+08 |
| Each | 1.31 µs | 11.6% | **0.58× [0.49–0.72]** | 766 k | checksum=2.792e+08 |
| Grain1 | 10.2 µs | 4.9% | **0.07× [0.07–0.08]** | 98.5 k | checksum=2.772e+08 |
| Grain64 | 1.21 µs | 5.2% | **0.64× [0.59–0.67]** | 829 k | checksum=2.797e+08 |
| Grain1024 | 727 ns | 5.4% | 0.99× [0.85–1.19] | 1.37 M | checksum=2.813e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 683 ns | 2.4% | baseline | 1.46 M | checksum=2.431e+08 |
| Each | 1.17 µs | 2.0% | **0.57× [0.49–0.59]** | 852 k | checksum=2.419e+08 |
| Grain1 | 9.59 µs | 5.4% | **0.07× [0.06–0.08]** | 104 k | checksum=2.396e+08 |
| Grain64 | 1.06 µs | 1.3% | **0.64× [0.60–0.68]** | 947 k | checksum=2.422e+08 |
| Grain1024 | 745 ns | 2.6% | **0.92× [0.88–0.98]** | 1.34 M | checksum=2.397e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.1 µs | 3.8% | baseline | 47.3 k | checksum=5.235e+09 |
| Each | 29.4 µs | 4.2% | **0.74× [0.68–0.86]** | 34 k | checksum=5.233e+09 |
| Grain1 | 226 µs | 3.8% | **0.10× [0.09–0.11]** | 4.43 k | checksum=5.231e+09 |
| Grain64 | 28.2 µs | 5.5% | **0.78× [0.69–0.86]** | 35.5 k | checksum=5.234e+09 |
| Grain1024 | 21.9 µs | 3.4% | 0.98× [0.91–1.14] | 45.6 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.4 µs | 3.2% | baseline | 46.8 k | checksum=5.183e+09 |
| Each | 29 µs | 2.0% | **0.74× [0.69–0.99]** | 34.5 k | checksum=5.184e+09 |
| Grain1 | 240 µs | 4.3% | **0.10× [0.09–0.12]** | 4.17 k | checksum=5.182e+09 |
| Grain64 | 28.9 µs | 7.3% | **0.77× [0.64–0.87]** | 34.6 k | checksum=5.184e+09 |
| Grain1024 | 22.5 µs | 3.4% | 0.96× [0.92–1.06] | 44.5 k | checksum=5.181e+09 |

