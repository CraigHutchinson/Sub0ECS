# Benchmark run 20261010-091551-459a0d0-r2-control

- rows: ok in 2 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.2% | baseline | 63 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.1% | 1.02× [0.95–1.02] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.6 µs | 0.2% | 1.02× [1.00–1.03] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.6 µs | 0.2% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.6 µs | 0.3% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.6 µs | 0.3% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.4% | **1.02× [1.01–1.02]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.6 µs | 0.3% | 1.02× [0.47–1.02] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.6 µs | 0.3% | **1.02× [1.01–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.6 µs | 0.4% | 1.02× [1.00–1.03] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.6 µs | 0.2% | **1.02× [1.01–1.03]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.6 µs | 0.2% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.6 µs | 0.4% | **1.02× [1.01–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.6 µs | 0.3% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.6% | baseline | 63.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.00–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.2% | baseline | 65 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–1.00]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.6 µs | 0.3% | baseline | 64.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.00× [1.00–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.2% | baseline | 65.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.5% | baseline | 63.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.1% | baseline | 65 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.33 ms | 3.5% | baseline | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.17 ms | 3.3% | 1.02× [0.93–1.12] | 240 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.41 ms | 2.9% | 0.96× [0.85–1.05] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.31 ms | 2.3% | 1.00× [0.94–1.10] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.35 ms | 3.1% | 1.00× [0.87–1.11] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.26 ms | 4.2% | 1.01× [0.79–1.14] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.35 ms | 2.9% | 1.00× [0.86–1.08] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.33 ms | 0.8% | 0.99× [0.93–1.12] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.37 ms | 4.1% | **1.83× [1.62–2.00]** | 422 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.34 ms | 7.5% | **1.87× [1.03–2.02]** | 427 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.28 ms | 3.7% | 1.04× [0.90–1.10] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.28 ms | 3.9% | 0.99× [0.93–1.12] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.37 ms | 6.7% | **3.13× [2.58–3.88]** | 732 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.2 ms | 31.3% | **1.96× [1.43–3.50]** | 455 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.32 ms | 4.0% | baseline | 232 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.44 ms | 4.8% | 0.98× [0.92–1.04] | 225 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.37 ms | 6.0% | baseline | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.44 ms | 2.9% | 0.96× [0.92–1.08] | 225 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.23 ms | 3.3% | baseline | 448 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 3.15 ms | 30.8% | 0.79× [0.53–1.02] | 318 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.15 ms | 0.5% | baseline | 241 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.26 ms | 6.4% | **1.67× [1.03–1.87]** | 443 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.99 ms | 6.3% | baseline | 502 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.86 ms | 18.8% | 1.00× [0.84–1.14] | 536 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.09 ms | 0.7% | baseline | 478 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.1 ms | 1.0% | 0.99× [0.98–1.01] | 476 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.2 ms | 2.3% | baseline | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.7 ms | 2.6% | 1.04× [0.92–1.13] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.9 ms | 2.5% | 1.01× [0.95–1.17] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 73 ms | 2.7% | 0.97× [0.91–1.18] | 13.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 69.8 ms | 2.3% | 1.01× [0.93–1.16] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71.7 ms | 2.4% | 1.00× [0.95–1.15] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.6 ms | 3.6% | **1.92× [1.46–2.31]** | 27.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.4 ms | 5.9% | **1.94× [1.19–2.17]** | 27.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.7 ms | 2.2% | **2.02× [1.92–2.15]** | 28 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 37.2 ms | 9.8% | **1.95× [1.10–2.18]** | 26.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.4 ms | 8.8% | **2.36× [2.15–4.07]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 31 ms | 4.6% | **2.56× [2.19–3.85]** | 32.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 20.7 ms | 9.6% | **3.38× [2.92–4.23]** | 48.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 20.7 ms | 17.2% | **3.35× [2.19–3.97]** | 48.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 77.5 ms | 7.1% | baseline | 12.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 79.6 ms | 10.9% | 0.98× [0.88–1.02] | 12.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.4 ms | 2.7% | baseline | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.4 ms | 0.8% | 1.00× [0.97–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.5 ms | 3.7% | baseline | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36 ms | 4.0% | 0.99× [0.73–1.05] | 27.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.5 ms | 2.7% | baseline | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.4 ms | 5.4% | 1.01× [0.94–1.09] | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22 ms | 9.1% | baseline | 45.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 22 ms | 15.4% | 0.98× [0.94–1.04] | 45.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.4 ms | 2.9% | baseline | 54.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.4 ms | 1.2% | 0.99× [0.94–1.02] | 54.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 ns | 5.6% | baseline | 63.3 M | checksum=9.402e+07 |
| Each | 22.9 ns | 7.1% | **0.72× [0.59–0.78]** | 43.6 M | checksum=9.398e+07 |
| Grain1 | 155 ns | 6.5% | **0.10× [0.09–0.11]** | 6.46 M | checksum=9.272e+07 |
| Grain64 | 23.7 ns | 4.2% | **0.66× [0.59–0.74]** | 42.3 M | checksum=9.377e+07 |
| Grain1024 | 18.9 ns | 6.5% | 0.81× [0.59–1.02] | 52.8 M | checksum=9.388e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18 ns | 11.4% | baseline | 55.7 M | checksum=8.215e+07 |
| Each | 21.9 ns | 6.1% | **0.83× [0.68–1.00]** | 45.7 M | checksum=8.231e+07 |
| Grain1 | 153 ns | 3.8% | **0.11× [0.10–0.16]** | 6.52 M | checksum=8.132e+07 |
| Grain64 | 25.4 ns | 4.2% | **0.65× [0.59–1.00]** | 39.3 M | checksum=8.239e+07 |
| Grain1024 | 21.4 ns | 2.2% | 0.83× [0.69–1.01] | 46.6 M | checksum=8.141e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 721 ns | 6.7% | baseline | 1.39 M | checksum=2.762e+08 |
| Each | 1.24 µs | 7.5% | **0.58× [0.45–0.63]** | 806 k | checksum=2.743e+08 |
| Grain1 | 9.74 µs | 5.2% | **0.07× [0.07–0.08]** | 103 k | checksum=2.721e+08 |
| Grain64 | 1.12 µs | 5.5% | **0.62× [0.60–0.72]** | 895 k | checksum=2.748e+08 |
| Grain1024 | 731 ns | 5.2% | 0.99× [0.87–1.07] | 1.37 M | checksum=2.751e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 708 ns | 6.3% | baseline | 1.41 M | checksum=2.86e+08 |
| Each | 1.21 µs | 3.3% | **0.57× [0.52–0.67]** | 826 k | checksum=2.841e+08 |
| Grain1 | 9.54 µs | 3.2% | **0.08× [0.07–0.08]** | 105 k | checksum=2.818e+08 |
| Grain64 | 1.12 µs | 3.0% | **0.65× [0.60–0.71]** | 894 k | checksum=2.842e+08 |
| Grain1024 | 790 ns | 4.9% | 0.91× [0.81–1.03] | 1.27 M | checksum=2.856e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 24.5 µs | 10.6% | baseline | 40.8 k | checksum=5.23e+09 |
| Each | 33.1 µs | 8.3% | **0.70× [0.53–0.75]** | 30.2 k | checksum=5.229e+09 |
| Grain1 | 269 µs | 18.3% | **0.09× [0.07–0.11]** | 3.72 k | checksum=5.226e+09 |
| Grain64 | 31.6 µs | 6.9% | **0.83× [0.71–0.87]** | 31.6 k | checksum=5.228e+09 |
| Grain1024 | 27.9 µs | 9.6% | 0.99× [0.85–1.07] | 35.9 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 23 µs | 11.0% | baseline | 43.5 k | checksum=5.216e+09 |
| Each | 33.6 µs | 12.5% | **0.74× [0.51–0.98]** | 29.7 k | checksum=5.215e+09 |
| Grain1 | 254 µs | 9.2% | **0.09× [0.08–0.12]** | 3.94 k | checksum=5.213e+09 |
| Grain64 | 30.6 µs | 10.9% | **0.80× [0.64–1.00]** | 32.7 k | checksum=5.216e+09 |
| Grain1024 | 24.2 µs | 10.5% | 0.91× [0.74–1.18] | 41.3 k | checksum=5.216e+09 |

