# Benchmark run 20261010-091806-459a0d0-r5-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.3% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | 1.01× [0.22–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.4% | **0.96× [0.95–0.97]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.3 µs | 0.6% | **0.97× [0.95–0.97]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.3 µs | 0.6% | **0.96× [0.90–0.97]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.5% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.3% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 0.3% | **0.96× [0.95–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.6% | **0.96× [0.91–0.99]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.3 µs | 0.3% | **0.97× [0.89–0.97]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.4% | **0.97× [0.96–0.99]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 0.5% | **0.97× [0.96–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.3 µs | 0.4% | **0.96× [0.96–0.98]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.3% | **0.96× [0.96–0.97]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.4% | baseline | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.99× [0.98–1.00]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.5% | **0.95× [0.94–0.95]** | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.4% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.4% | **0.99× [0.99–1.00]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.94× [0.94–0.95]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.4% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.3% | **0.99× [0.99–1.00]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.12 ms | 1.0% | baseline | 243 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.32 ms | 5.1% | 0.95× [0.85–1.09] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.34 ms | 3.4% | 0.95× [0.91–1.05] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.26 ms | 4.0% | 0.99× [0.94–1.08] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.31 ms | 3.5% | 0.98× [0.92–1.04] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.34 ms | 6.7% | 0.98× [0.86–1.09] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.32 ms | 5.5% | 0.96× [0.86–1.04] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.24 ms | 4.1% | 1.01× [0.93–1.09] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.19 ms | 3.1% | **1.90× [1.73–2.10]** | 456 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.28 ms | 6.9% | 1.91× [0.97–2.06] | 438 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.33 ms | 6.7% | 0.98× [0.88–1.08] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.31 ms | 5.5% | 0.98× [0.85–1.08] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.56 ms | 2.0% | **2.65× [2.56–3.99]** | 642 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.14 ms | 2.1% | **1.93× [1.86–2.32]** | 468 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.13 ms | 2.6% | baseline | 242 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.35 ms | 4.6% | 0.96× [0.92–1.01] | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.15 ms | 3.7% | baseline | 241 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.32 ms | 6.2% | 1.01× [0.94–1.03] | 232 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.25 ms | 4.1% | baseline | 444 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.16 ms | 3.8% | 0.99× [0.98–1.09] | 463 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.32 ms | 6.7% | baseline | 430 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.32 ms | 3.9% | 1.01× [0.89–1.09] | 430 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.57 ms | 3.9% | baseline | 637 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.72 ms | 12.0% | 0.94× [0.69–1.29] | 583 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.27 ms | 3.4% | baseline | 788 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.1 ms | 5.9% | **0.70× [0.54–0.99]** | 477 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.3 ms | 3.5% | baseline | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.5 ms | 2.2% | 0.98× [0.95–1.05] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 72.1 ms | 8.9% | 0.94× [0.72–1.05] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 71.5 ms | 4.4% | 0.99× [0.92–1.05] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 71.4 ms | 4.2% | 0.97× [0.74–1.02] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.4 ms | 4.9% | 1.00× [0.89–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 39 ms | 13.6% | **1.87× [1.22–2.03]** | 25.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 37.2 ms | 8.1% | **1.83× [1.15–2.02]** | 26.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 36 ms | 6.2% | **1.95× [1.38–2.01]** | 27.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 36.7 ms | 3.6% | **1.87× [1.75–2.05]** | 27.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.5 ms | 8.6% | **2.14× [1.73–2.52]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.5 ms | 0.3% | **2.17× [2.05–3.82]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.4 ms | 11.6% | **2.97× [2.23–3.79]** | 42.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.5 ms | 0.5% | **2.15× [1.97–3.62]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.2 ms | 3.0% | baseline | 14 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.2 ms | 2.4% | 0.99× [0.94–1.07] | 14 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.7 ms | 1.7% | baseline | 14.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.5 ms | 1.7% | 0.98× [0.96–1.02] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.2 ms | 2.2% | baseline | 29.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.2 ms | 1.4% | 0.98× [0.93–1.02] | 28.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.5 ms | 1.1% | baseline | 29 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 34.6 ms | 0.8% | 0.99× [0.99–1.01] | 28.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.6 ms | 8.4% | baseline | 53.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 22.1 ms | 7.5% | **0.95× [0.81–1.00]** | 45.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.5 ms | 1.7% | baseline | 54.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.5 ms | 2.9% | 1.00× [0.94–1.02] | 53.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.2 ns | 3.1% | baseline | 70.4 M | checksum=9.122e+07 |
| Each | 16.4 ns | 1.7% | **0.86× [0.84–0.96]** | 61 M | checksum=9.127e+07 |
| Grain1 | 17.5 ns | 4.1% | **0.81× [0.75–0.89]** | 57 M | checksum=9.117e+07 |
| Grain64 | 22.1 ns | 7.0% | **0.66× [0.61–0.72]** | 45.3 M | checksum=9.119e+07 |
| Grain1024 | 23 ns | 5.0% | **0.62× [0.58–0.68]** | 43.5 M | checksum=9.125e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 ns | 1.5% | baseline | 63.2 M | checksum=8.807e+07 |
| Each | 18.9 ns | 2.6% | **0.85× [0.74–0.88]** | 53 M | checksum=8.8e+07 |
| Grain1 | 141 ns | 2.7% | **0.11× [0.11–0.12]** | 7.09 M | checksum=8.692e+07 |
| Grain64 | 25.9 ns | 1.2% | **0.61× [0.56–0.63]** | 38.6 M | checksum=8.797e+07 |
| Grain1024 | 25.1 ns | 1.9% | **0.63× [0.60–0.65]** | 39.8 M | checksum=8.796e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 697 ns | 0.7% | baseline | 1.43 M | checksum=2.904e+08 |
| Each | 651 ns | 0.3% | **1.07× [1.05–1.16]** | 1.54 M | checksum=2.921e+08 |
| Grain1 | 665 ns | 1.6% | **1.06× [1.00–1.12]** | 1.5 M | checksum=2.922e+08 |
| Grain64 | 699 ns | 1.0% | 1.00× [0.96–1.08] | 1.43 M | checksum=2.918e+08 |
| Grain1024 | 675 ns | 2.6% | 1.03× [0.99–1.14] | 1.48 M | checksum=2.915e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 713 ns | 0.5% | baseline | 1.4 M | checksum=2.841e+08 |
| Each | 680 ns | 1.6% | 1.06× [0.99–1.09] | 1.47 M | checksum=2.847e+08 |
| Grain1 | 9.14 µs | 1.4% | **0.08× [0.08–0.08]** | 109 k | checksum=2.805e+08 |
| Grain64 | 1.05 µs | 0.7% | **0.69× [0.67–0.71]** | 957 k | checksum=2.829e+08 |
| Grain1024 | 691 ns | 1.2% | 1.04× [0.97–1.07] | 1.45 M | checksum=2.846e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.8 µs | 5.0% | baseline | 45.8 k | checksum=5.235e+09 |
| Each | 20.5 µs | 2.4% | 1.01× [0.99–1.11] | 48.7 k | checksum=5.234e+09 |
| Grain1 | 20.6 µs | 2.5% | 1.03× [0.97–1.10] | 48.7 k | checksum=5.235e+09 |
| Grain64 | 21.1 µs | 1.8% | 1.02× [0.98–1.04] | 47.4 k | checksum=5.235e+09 |
| Grain1024 | 20.8 µs | 1.4% | 1.01× [0.98–1.10] | 48.1 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.6 µs | 0.8% | baseline | 48.6 k | checksum=5.235e+09 |
| Each | 21.1 µs | 1.5% | 0.98× [0.93–1.17] | 47.5 k | checksum=5.235e+09 |
| Grain1 | 219 µs | 2.7% | **0.10× [0.09–0.11]** | 4.57 k | checksum=5.231e+09 |
| Grain64 | 26.8 µs | 0.5% | **0.77× [0.76–0.91]** | 37.4 k | checksum=5.232e+09 |
| Grain1024 | 21.3 µs | 2.1% | 0.97× [0.90–1.09] | 47 k | checksum=5.235e+09 |

