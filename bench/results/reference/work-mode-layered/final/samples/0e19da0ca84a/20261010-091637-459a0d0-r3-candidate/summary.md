# Benchmark run 20261010-091637-459a0d0-r3-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.4% | baseline | 63.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | **1.01× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.4 µs | 0.3% | **0.96× [0.96–0.97]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 0.5% | **0.96× [0.96–0.97]** | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.4 µs | 0.5% | **0.96× [0.96–0.99]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.4 µs | 0.3% | **0.96× [0.96–0.99]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.4 µs | 0.8% | 0.96× [0.80–1.00] | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.4 µs | 0.6% | **0.96× [0.95–0.99]** | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.7% | **0.96× [0.95–0.98]** | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.4 µs | 1.0% | **0.96× [0.95–0.98]** | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.4 µs | 0.3% | 0.96× [0.95–1.00] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.5 µs | 0.7% | **0.96× [0.22–0.99]** | 60.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.5 µs | 0.5% | **0.96× [0.60–1.00]** | 60.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.4 µs | 0.6% | **0.96× [0.96–0.99]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.6% | 1.00× [0.98–1.01] | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.4% | **0.94× [0.94–0.95]** | 61.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.4% | baseline | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.7% | **0.99× [0.79–1.00]** | 60.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.6% | **0.94× [0.93–0.95]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.4% | baseline | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.2% | 1.00× [0.99–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.3% | **0.94× [0.93–0.94]** | 61.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.36 ms | 2.8% | baseline | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.56 ms | 4.6% | 0.94× [0.85–1.07] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.46 ms | 5.0% | 0.99× [0.82–1.10] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.77 ms | 4.2% | **0.92× [0.85–0.96]** | 210 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.72 ms | 8.0% | 0.94× [0.78–1.04] | 212 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.87 ms | 4.7% | 0.91× [0.78–1.00] | 205 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.55 ms | 4.8% | 0.96× [0.87–1.07] | 220 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.71 ms | 6.4% | 0.93× [0.85–1.04] | 212 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.33 ms | 4.1% | **1.88× [1.61–2.04]** | 429 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.61 ms | 11.8% | 1.64× [0.98–1.94] | 383 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.53 ms | 4.2% | 0.97× [0.83–1.01] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.62 ms | 4.0% | 0.95× [0.82–1.04] | 216 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.57 ms | 15.7% | **2.94× [2.28–3.84]** | 639 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.24 ms | 5.7% | **1.95× [1.55–2.21]** | 446 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.2 ms | 2.4% | baseline | 238 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.13 ms | 1.0% | 1.00× [0.98–1.04] | 242 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.09 ms | 2.1% | baseline | 245 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.27 ms | 4.2% | **0.98× [0.92–1.00]** | 234 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.19 ms | 3.3% | baseline | 457 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.34 ms | 3.3% | 0.95× [0.90–1.04] | 428 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.28 ms | 2.3% | baseline | 438 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.2 ms | 2.6% | 1.02× [0.98–1.07] | 455 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.58 ms | 1.5% | baseline | 634 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.58 ms | 1.9% | 1.01× [0.96–1.03] | 633 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.1 ms | 0.9% | baseline | 476 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.09 ms | 0.8% | 1.01× [1.00–1.02] | 479 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.8 ms | 1.4% | baseline | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.6 ms | 2.4% | 0.99× [0.95–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 72.2 ms | 4.8% | 0.97× [0.85–1.02] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 71.9 ms | 3.7% | 0.98× [0.90–1.03] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 71 ms | 2.4% | 0.99× [0.89–1.02] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71 ms | 2.5% | 0.98× [0.93–1.03] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 37 ms | 7.3% | **1.87× [1.05–2.05]** | 27 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 35.4 ms | 3.3% | **1.97× [1.14–2.03]** | 28.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.1 ms | 3.3% | **1.96× [1.08–2.06]** | 28.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 35.4 ms | 2.9% | **1.99× [1.75–2.08]** | 28.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.5 ms | 0.6% | **2.18× [2.01–3.32]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.3 ms | 2.8% | **2.17× [2.06–4.02]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.1 ms | 12.6% | **2.97× [2.15–3.84]** | 43.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.5 ms | 12.0% | **2.30× [1.90–3.44]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.8 ms | 3.6% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.7 ms | 3.1% | 0.99× [0.95–1.02] | 13.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.7 ms | 1.3% | baseline | 14.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.8 ms | 1.1% | **0.98× [0.95–0.99]** | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.7 ms | 2.7% | baseline | 28.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 37.2 ms | 4.2% | **0.94× [0.65–1.00]** | 26.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.6 ms | 3.3% | baseline | 28.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.3 ms | 1.4% | 0.99× [0.95–1.00] | 27.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.3 ms | 2.4% | baseline | 54.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.5 ms | 2.0% | 0.99× [0.96–1.01] | 54 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.7 ms | 7.8% | baseline | 50.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 19 ms | 2.0% | 1.01× [0.90–1.12] | 52.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15 ns | 5.7% | baseline | 66.5 M | checksum=9.398e+07 |
| Each | 16.7 ns | 3.5% | **0.89× [0.84–0.93]** | 59.9 M | checksum=9.388e+07 |
| Grain1 | 18.7 ns | 9.8% | **0.81× [0.66–0.90]** | 53.6 M | checksum=9.382e+07 |
| Grain64 | 24.5 ns | 8.3% | **0.62× [0.56–0.66]** | 40.8 M | checksum=9.306e+07 |
| Grain1024 | 22.5 ns | 3.8% | **0.67× [0.60–0.69]** | 44.5 M | checksum=9.391e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 ns | 2.9% | baseline | 63.1 M | checksum=8.51e+07 |
| Each | 18.9 ns | 3.9% | **0.84× [0.76–0.96]** | 53 M | checksum=8.504e+07 |
| Grain1 | 146 ns | 2.4% | **0.11× [0.11–0.12]** | 6.86 M | checksum=8.402e+07 |
| Grain64 | 27.1 ns | 5.4% | **0.60× [0.55–0.69]** | 36.9 M | checksum=8.506e+07 |
| Grain1024 | 25.5 ns | 3.3% | **0.61× [0.56–0.71]** | 39.3 M | checksum=8.499e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 697 ns | 2.9% | baseline | 1.44 M | checksum=2.915e+08 |
| Each | 664 ns | 2.2% | 1.04× [0.97–1.12] | 1.51 M | checksum=2.915e+08 |
| Grain1 | 684 ns | 3.6% | 1.03× [0.98–1.06] | 1.46 M | checksum=2.918e+08 |
| Grain64 | 695 ns | 0.3% | 0.98× [0.95–1.03] | 1.44 M | checksum=2.91e+08 |
| Grain1024 | 693 ns | 5.1% | 1.02× [0.85–1.07] | 1.44 M | checksum=2.917e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 682 ns | 1.4% | baseline | 1.47 M | checksum=2.668e+08 |
| Each | 674 ns | 1.2% | 1.01× [0.99–1.10] | 1.48 M | checksum=2.678e+08 |
| Grain1 | 8.97 µs | 2.3% | **0.08× [0.07–0.08]** | 111 k | checksum=2.639e+08 |
| Grain64 | 1.07 µs | 3.0% | **0.66× [0.64–0.67]** | 934 k | checksum=2.645e+08 |
| Grain1024 | 683 ns | 0.1% | 0.99× [0.98–1.07] | 1.46 M | checksum=2.659e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.5 µs | 1.1% | baseline | 48.7 k | checksum=5.235e+09 |
| Each | 20.5 µs | 1.8% | 1.02× [0.98–1.08] | 48.7 k | checksum=5.234e+09 |
| Grain1 | 20.2 µs | 1.4% | 1.02× [0.97–1.04] | 49.4 k | checksum=5.235e+09 |
| Grain64 | 20.7 µs | 1.6% | 0.99× [0.90–1.01] | 48.3 k | checksum=5.234e+09 |
| Grain1024 | 20.5 µs | 1.7% | 1.01× [0.99–1.05] | 48.7 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.6 µs | 3.1% | baseline | 48.4 k | checksum=5.235e+09 |
| Each | 21.4 µs | 4.9% | 0.99× [0.90–1.04] | 46.7 k | checksum=5.235e+09 |
| Grain1 | 222 µs | 2.4% | **0.09× [0.09–0.10]** | 4.5 k | checksum=5.231e+09 |
| Grain64 | 27.1 µs | 1.2% | **0.77× [0.73–0.82]** | 36.9 k | checksum=5.234e+09 |
| Grain1024 | 21.4 µs | 2.4% | 0.97× [0.90–1.07] | 46.8 k | checksum=5.234e+09 |

