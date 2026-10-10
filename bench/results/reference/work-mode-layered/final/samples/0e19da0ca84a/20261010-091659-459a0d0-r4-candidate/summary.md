# Benchmark run 20261010-091659-459a0d0-r4-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.3% | baseline | 63.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | **1.02× [1.00–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.7% | **0.97× [0.96–0.99]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.3 µs | 0.6% | **0.97× [0.93–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.4% | **0.97× [0.97–0.99]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.2 µs | 0.4% | **0.97× [0.58–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.5% | **0.97× [0.96–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 0.5% | **0.97× [0.96–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.2% | **0.97× [0.96–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.3% | 0.97× [0.56–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.6% | **0.97× [0.96–1.00]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 0.5% | **0.97× [0.96–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.6% | 0.97× [0.96–1.01] | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.4% | **0.97× [0.96–1.00]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.2% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.2% | 0.99× [0.99–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.3% | **0.99× [0.87–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.1% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.2% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.2% | **0.99× [0.99–1.00]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.95]** | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.32 ms | 4.2% | baseline | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.39 ms | 2.4% | 0.98× [0.92–1.13] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.34 ms | 4.2% | 1.00× [0.91–1.07] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.11 ms | 1.6% | 1.03× [0.94–1.14] | 243 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.16 ms | 3.1% | 1.01× [0.90–1.15] | 241 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.34 ms | 4.3% | 0.99× [0.90–1.12] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.51 ms | 5.4% | 0.95× [0.72–1.12] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.25 ms | 4.1% | 1.00× [0.92–1.19] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.28 ms | 4.2% | **1.89× [1.70–2.00]** | 438 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.34 ms | 3.0% | **1.83× [1.60–2.04]** | 428 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.51 ms | 3.6% | 0.97× [0.90–1.07] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.42 ms | 2.9% | 0.96× [0.90–1.15] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.31 ms | 6.1% | **3.31× [2.69–3.58]** | 764 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.45 ms | 15.4% | **1.84× [1.25–2.86]** | 409 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.54 ms | 7.4% | baseline | 220 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.3 ms | 6.4% | 1.00× [0.97–1.09] | 232 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.2 ms | 3.4% | baseline | 238 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.42 ms | 4.0% | 0.94× [0.91–1.00] | 226 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.15 ms | 4.7% | baseline | 466 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.28 ms | 3.4% | 0.96× [0.90–1.02] | 440 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.21 ms | 4.2% | baseline | 452 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.26 ms | 4.5% | 0.98× [0.91–1.10] | 442 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.23 ms | 3.3% | baseline | 810 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.13 ms | 3.4% | 1.06× [1.00–1.13] | 886 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.76 ms | 22.1% | baseline | 567 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.05 ms | 8.5% | 1.03× [0.65–1.19] | 488 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72 ms | 2.6% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.1 ms | 3.8% | 1.00× [0.91–1.08] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.9 ms | 2.4% | 1.00× [0.85–1.09] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 70.6 ms | 1.1% | 1.01× [0.96–1.07] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 71.9 ms | 4.1% | 0.99× [0.92–1.05] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 72.4 ms | 3.8% | 1.00× [0.93–1.07] | 13.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 39.4 ms | 10.0% | **1.92× [1.11–2.05]** | 25.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 43.4 ms | 25.8% | **1.67× [1.08–2.06]** | 23 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 43 ms | 16.9% | **1.66× [1.12–2.16]** | 23.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 37.5 ms | 5.9% | **1.88× [1.12–2.18]** | 26.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.4 ms | 6.6% | **2.24× [2.07–4.08]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 3.0% | **2.23× [1.86–4.27]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.4 ms | 9.0% | **3.03× [2.24–4.34]** | 42.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.2 ms | 17.1% | **2.17× [2.08–3.80]** | 31.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.7 ms | 2.0% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.1 ms | 1.4% | 1.00× [0.98–1.03] | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.2 ms | 1.0% | baseline | 14.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.5 ms | 1.6% | 0.98× [0.96–1.00] | 14.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.5 ms | 1.8% | baseline | 29 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 34.7 ms | 0.9% | 0.99× [0.98–1.00] | 28.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.5 ms | 1.1% | baseline | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.5 ms | 2.9% | 0.98× [0.95–1.07] | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.9 ms | 1.3% | baseline | 55.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.5 ms | 2.9% | 0.97× [0.94–1.01] | 54.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.3 ms | 6.2% | baseline | 51.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.7 ms | 3.2% | 1.04× [0.96–1.46] | 53.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.7 ns | 7.7% | baseline | 67.8 M | checksum=9.11e+07 |
| Each | 16.9 ns | 6.9% | 0.88× [0.71–1.05] | 59 M | checksum=9.165e+07 |
| Grain1 | 19.6 ns | 12.3% | 0.75× [0.63–1.01] | 51 M | checksum=9.099e+07 |
| Grain64 | 24 ns | 6.0% | **0.63× [0.54–0.71]** | 41.7 M | checksum=9.156e+07 |
| Grain1024 | 22.7 ns | 5.0% | **0.62× [0.50–0.72]** | 44 M | checksum=9.163e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 ns | 2.6% | baseline | 63 M | checksum=8.966e+07 |
| Each | 18.9 ns | 1.2% | **0.83× [0.78–0.88]** | 53 M | checksum=9.013e+07 |
| Grain1 | 141 ns | 2.2% | **0.11× [0.10–0.12]** | 7.09 M | checksum=8.897e+07 |
| Grain64 | 25.7 ns | 2.7% | **0.61× [0.55–0.64]** | 38.9 M | checksum=8.974e+07 |
| Grain1024 | 25.2 ns | 2.1% | **0.62× [0.56–0.69]** | 39.7 M | checksum=8.983e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 688 ns | 1.6% | baseline | 1.45 M | checksum=2.934e+08 |
| Each | 659 ns | 1.3% | 1.05× [0.97–1.23] | 1.52 M | checksum=2.94e+08 |
| Grain1 | 666 ns | 1.6% | 1.05× [0.99–1.14] | 1.5 M | checksum=2.919e+08 |
| Grain64 | 706 ns | 1.8% | 0.98× [0.94–1.08] | 1.42 M | checksum=2.932e+08 |
| Grain1024 | 659 ns | 0.2% | **1.04× [1.03–1.20]** | 1.52 M | checksum=2.939e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 686 ns | 1.7% | baseline | 1.46 M | checksum=2.843e+08 |
| Each | 672 ns | 1.3% | 1.02× [0.96–1.08] | 1.49 M | checksum=2.847e+08 |
| Grain1 | 9.04 µs | 1.0% | **0.08× [0.07–0.08]** | 111 k | checksum=2.805e+08 |
| Grain64 | 1.04 µs | 1.5% | **0.66× [0.60–0.67]** | 960 k | checksum=2.831e+08 |
| Grain1024 | 692 ns | 0.4% | 0.98× [0.95–1.00] | 1.45 M | checksum=2.843e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.7 µs | 1.1% | baseline | 48.2 k | checksum=5.234e+09 |
| Each | 20.1 µs | 1.3% | 1.03× [0.96–1.10] | 49.7 k | checksum=5.235e+09 |
| Grain1 | 20.3 µs | 1.5% | 1.02× [0.97–1.15] | 49.3 k | checksum=5.234e+09 |
| Grain64 | 20.4 µs | 0.4% | 1.01× [1.00–1.14] | 48.9 k | checksum=5.235e+09 |
| Grain1024 | 20.5 µs | 1.7% | 1.01× [0.84–1.11] | 48.7 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.8 µs | 3.6% | baseline | 48 k | checksum=5.235e+09 |
| Each | 20.6 µs | 1.5% | 1.01× [0.98–1.21] | 48.6 k | checksum=5.235e+09 |
| Grain1 | 220 µs | 1.5% | **0.10× [0.09–0.12]** | 4.55 k | checksum=5.231e+09 |
| Grain64 | 27 µs | 1.7% | **0.77× [0.75–0.90]** | 37.1 k | checksum=5.234e+09 |
| Grain1024 | 21.1 µs | 0.9% | 1.01× [0.96–1.12] | 47.5 k | checksum=5.234e+09 |

