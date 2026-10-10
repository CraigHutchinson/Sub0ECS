# Benchmark run 20261010-091528-459a0d0-r2-candidate

- rows: ok in 1 s
- nbody: ok in 21 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.8% | baseline | 62.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.7% | 1.02× [0.99–1.04] | 63.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.4 µs | 1.0% | 0.98× [0.96–1.01] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 2.1% | 0.98× [0.94–1.01] | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.4 µs | 1.2% | 0.98× [0.96–1.02] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.4 µs | 1.9% | 0.98× [0.93–1.03] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.9% | 0.98× [0.95–1.05] | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.4 µs | 0.9% | 0.97× [0.95–1.05] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.7% | 0.97× [0.96–1.05] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.3 µs | 1.5% | 0.97× [0.96–1.04] | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.4 µs | 1.8% | 0.97× [0.85–1.02] | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.4 µs | 1.4% | 0.97× [0.44–1.05] | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.4 µs | 1.9% | 0.97× [0.80–1.03] | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.4 µs | 1.1% | 0.97× [0.79–1.02] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.8% | baseline | 63 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.7 µs | 0.4% | 1.01× [1.00–1.01] | 63.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.8% | **0.94× [0.93–0.95]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 1.9% | baseline | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.7% | 0.99× [0.96–1.01] | 61.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.5% | **0.94× [0.93–0.95]** | 61.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 µs | 0.8% | baseline | 60.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.2% | 1.00× [1.00–1.01] | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.6% | **0.93× [0.92–0.94]** | 60.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.16 ms | 1.7% | baseline | 240 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.26 ms | 3.6% | 0.99× [0.94–1.11] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.4 ms | 3.7% | 0.97× [0.90–1.04] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.26 ms | 3.5% | 1.00× [0.89–1.08] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.14 ms | 2.4% | 1.00× [0.90–1.07] | 242 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.34 ms | 2.2% | 0.98× [0.92–1.07] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.18 ms | 3.8% | 0.98× [0.92–1.04] | 239 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.33 ms | 4.7% | 0.99× [0.94–1.17] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.06 ms | 0.5% | **1.02× [1.00–1.67]** | 247 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.37 ms | 8.3% | **1.84× [1.06–2.46]** | 422 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.18 ms | 3.5% | 1.01× [0.95–1.10] | 239 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.27 ms | 4.7% | 0.99× [0.87–1.06] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.58 ms | 2.0% | **2.64× [2.06–2.67]** | 633 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.12 ms | 0.9% | **1.96× [1.86–2.47]** | 473 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.04 ms | 0.3% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.02 ms | 0.1% | 1.00× [1.00–1.01] | 249 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.07 ms | 1.5% | baseline | 246 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.04 ms | 0.3% | 1.00× [0.99–1.05] | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 0.5% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.21 ms | 3.1% | **1.70× [1.05–1.80]** | 452 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.17 ms | 2.6% | baseline | 460 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.33 ms | 6.0% | 0.98× [0.91–1.02] | 430 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.12 ms | 5.0% | baseline | 472 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.53 ms | 1.1% | **1.40× [1.35–1.59]** | 652 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.1 ms | 0.7% | baseline | 477 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.08 ms | 0.4% | **1.01× [1.00–1.03]** | 481 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.8 ms | 2.8% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.9 ms | 1.7% | 1.01× [0.96–1.04] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.3 ms | 0.7% | 1.01× [0.95–1.06] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 71 ms | 2.0% | 0.99× [0.92–1.04] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.9 ms | 3.7% | 1.01× [0.89–1.09] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71.8 ms | 1.6% | 0.98× [0.97–1.06] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.4 ms | 7.2% | **1.89× [1.13–2.08]** | 27.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.5 ms | 6.8% | **1.99× [1.15–2.07]** | 27.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.6 ms | 2.2% | **1.97× [1.10–2.12]** | 28.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 38.2 ms | 6.0% | **1.85× [1.66–2.05]** | 26.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.4 ms | 1.0% | **2.22× [2.09–2.65]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 2.3% | **2.21× [1.94–2.92]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 25.7 ms | 10.5% | **2.80× [2.16–3.21]** | 39 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 0.4% | **2.22× [2.12–3.14]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 73.1 ms | 3.1% | baseline | 13.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.8 ms | 2.1% | 1.01× [0.98–1.03] | 13.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.2 ms | 2.6% | baseline | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.5 ms | 1.8% | 0.99× [0.97–1.30] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 39.2 ms | 10.6% | baseline | 25.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.4 ms | 5.7% | 1.00× [0.94–1.64] | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 47.2 ms | 27.7% | baseline | 21.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 58.6 ms | 10.4% | 1.00× [0.80–1.10] | 17.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.4 ms | 10.4% | baseline | 44.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 22.3 ms | 7.1% | 1.00× [0.96–1.05] | 44.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 26.8 ms | 24.7% | baseline | 37.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 25.8 ms | 21.3% | 1.02× [0.91–1.09] | 38.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.5 ns | 1.9% | baseline | 68.9 M | checksum=9.361e+07 |
| Each | 16.5 ns | 3.0% | **0.86× [0.80–0.91]** | 60.8 M | checksum=9.358e+07 |
| Grain1 | 17.1 ns | 2.2% | **0.84× [0.82–0.85]** | 58.5 M | checksum=9.34e+07 |
| Grain64 | 22.7 ns | 2.7% | **0.64× [0.58–0.66]** | 44.1 M | checksum=9.306e+07 |
| Grain1024 | 22.7 ns | 4.9% | **0.64× [0.58–0.67]** | 44 M | checksum=9.355e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 ns | 4.2% | baseline | 61 M | checksum=9.206e+07 |
| Each | 19 ns | 1.3% | **0.87× [0.80–0.90]** | 52.7 M | checksum=9.199e+07 |
| Grain1 | 146 ns | 3.6% | **0.11× [0.11–0.12]** | 6.85 M | checksum=9.085e+07 |
| Grain64 | 26.1 ns | 2.3% | **0.63× [0.57–0.65]** | 38.4 M | checksum=9.179e+07 |
| Grain1024 | 25.3 ns | 1.1% | **0.62× [0.57–0.67]** | 39.6 M | checksum=9.19e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 729 ns | 6.3% | baseline | 1.37 M | checksum=2.917e+08 |
| Each | 663 ns | 2.0% | 1.10× [0.98–1.19] | 1.51 M | checksum=2.911e+08 |
| Grain1 | 699 ns | 6.5% | 1.04× [0.87–1.10] | 1.43 M | checksum=2.923e+08 |
| Grain64 | 729 ns | 5.1% | 1.01× [0.90–1.12] | 1.37 M | checksum=2.92e+08 |
| Grain1024 | 669 ns | 1.8% | 1.04× [0.88–1.14] | 1.5 M | checksum=2.919e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 730 ns | 3.1% | baseline | 1.37 M | checksum=2.91e+08 |
| Each | 721 ns | 7.6% | 1.02× [0.89–1.20] | 1.39 M | checksum=2.914e+08 |
| Grain1 | 9.93 µs | 3.4% | **0.08× [0.07–0.09]** | 101 k | checksum=2.871e+08 |
| Grain64 | 1.11 µs | 4.7% | **0.65× [0.61–0.72]** | 900 k | checksum=2.897e+08 |
| Grain1024 | 744 ns | 4.9% | 0.98× [0.83–1.13] | 1.34 M | checksum=2.912e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.6 µs | 9.6% | baseline | 44.3 k | checksum=5.235e+09 |
| Each | 22 µs | 5.1% | 0.96× [0.80–1.21] | 45.6 k | checksum=5.234e+09 |
| Grain1 | 22 µs | 8.5% | 0.99× [0.86–1.13] | 45.5 k | checksum=5.234e+09 |
| Grain64 | 22.1 µs | 5.1% | 1.04× [0.89–1.20] | 45.3 k | checksum=5.235e+09 |
| Grain1024 | 20.8 µs | 3.5% | 1.00× [0.85–1.20] | 48 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.3 µs | 6.3% | baseline | 44.9 k | checksum=5.235e+09 |
| Each | 21.9 µs | 6.5% | 0.98× [0.86–1.27] | 45.7 k | checksum=5.235e+09 |
| Grain1 | 252 µs | 4.1% | **0.09× [0.08–0.10]** | 3.97 k | checksum=5.231e+09 |
| Grain64 | 28.4 µs | 3.8% | **0.74× [0.59–0.90]** | 35.2 k | checksum=5.234e+09 |
| Grain1024 | 23.1 µs | 6.5% | 0.99× [0.86–1.15] | 43.3 k | checksum=5.235e+09 |

