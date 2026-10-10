# Benchmark run 20261010-011348-459a0d0-r2-control

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.2% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | 1.01× [0.93–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.5 µs | 0.1% | **1.01× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.5 µs | 0.3% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.1% | **1.01× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.5 µs | 0.1% | 1.01× [0.85–1.02] | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.3% | baseline | 62.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.03× [1.03–1.03]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.6% | baseline | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.04× [1.02–1.05]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.4% | baseline | 62.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.4 µs | 0.1% | **1.03× [1.03–1.04]** | 64.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.35 ms | 5.7% | baseline | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.28 ms | 2.4% | 1.03× [0.93–1.09] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.3 ms | 3.5% | 0.99× [0.92–1.10] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.34 ms | 3.3% | 1.00× [0.91–1.05] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.31 ms | 2.6% | 1.00× [0.95–1.08] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.31 ms | 3.1% | 0.99× [0.91–1.09] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.37 ms | 5.4% | 1.00× [0.90–1.08] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.29 ms | 3.7% | 1.02× [0.91–1.08] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.28 ms | 4.1% | **1.93× [1.69–2.09]** | 439 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.28 ms | 5.4% | **1.88× [1.56–2.15]** | 439 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.35 ms | 5.9% | 1.00× [0.89–1.08] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.38 ms | 3.3% | 1.02× [0.90–1.08] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.24 ms | 5.3% | **3.42× [3.09–3.83]** | 805 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 1.78 ms | 29.8% | **2.37× [1.77–3.75]** | 561 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.18 ms | 2.8% | baseline | 239 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.33 ms | 1.7% | 0.98× [0.95–1.01] | 231 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.37 ms | 3.4% | baseline | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.46 ms | 5.1% | 0.98× [0.90–1.06] | 224 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.33 ms | 4.1% | baseline | 429 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.28 ms | 5.3% | 1.04× [0.89–1.10] | 438 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.45 ms | 6.0% | baseline | 408 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.28 ms | 7.4% | 1.02× [0.95–1.14] | 438 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.25 ms | 6.9% | baseline | 803 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.2 ms | 4.1% | 1.03× [0.93–1.15] | 835 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.33 ms | 18.4% | baseline | 754 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.11 ms | 1.3% | 0.63× [0.52–1.01] | 474 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.3 ms | 2.2% | baseline | 13.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.9 ms | 3.8% | 1.01× [0.92–1.08] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.8 ms | 1.6% | 1.01× [0.94–1.06] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 71.2 ms | 2.2% | 1.01× [0.97–1.05] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70 ms | 1.6% | 1.02× [0.96–1.07] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.7 ms | 3.2% | 1.01× [0.92–1.06] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 37.4 ms | 5.4% | **1.94× [1.61–2.10]** | 26.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 37.6 ms | 9.2% | **1.88× [1.15–2.14]** | 26.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 36.2 ms | 3.1% | **1.98× [1.68–2.14]** | 27.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 36.5 ms | 5.9% | **1.97× [1.07–2.06]** | 27.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.6 ms | 0.3% | **2.23× [2.08–3.72]** | 30.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.5 ms | 0.8% | **2.21× [2.10–3.65]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.6 ms | 6.2% | **3.07× [2.59–3.87]** | 42.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 1.9% | **2.29× [2.13–3.90]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 75.6 ms | 4.0% | baseline | 13.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 75 ms | 3.9% | 1.02× [0.93–1.05] | 13.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.2 ms | 1.0% | baseline | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.7 ms | 2.0% | 1.01× [0.99–1.04] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.6 ms | 3.7% | baseline | 28.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.2 ms | 4.0% | 0.98× [0.89–1.04] | 27.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.8 ms | 1.5% | baseline | 28 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36 ms | 1.1% | 0.99× [0.96–1.01] | 27.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.6 ms | 1.1% | baseline | 56.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.3 ms | 1.3% | **0.97× [0.95–0.99]** | 54.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 30 ms | 9.0% | baseline | 33.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 24.9 ms | 23.7% | 0.98× [0.91–1.27] | 40.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.5 ns | 4.1% | baseline | 68.8 M | checksum=9.081e+07 |
| Each | 21 ns | 2.1% | **0.69× [0.60–0.77]** | 47.6 M | checksum=9.164e+07 |
| Grain1 | 146 ns | 3.9% | **0.10× [0.09–0.11]** | 6.87 M | checksum=9.046e+07 |
| Grain64 | 22.9 ns | 3.7% | **0.62× [0.59–0.68]** | 43.7 M | checksum=9.095e+07 |
| Grain1024 | 17.4 ns | 5.3% | **0.81× [0.69–0.94]** | 57.5 M | checksum=9.128e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 ns | 1.8% | baseline | 64.9 M | checksum=9.243e+07 |
| Each | 20.5 ns | 2.4% | **0.74× [0.65–0.80]** | 48.7 M | checksum=9.235e+07 |
| Grain1 | 148 ns | 3.0% | **0.10× [0.10–0.12]** | 6.75 M | checksum=9.119e+07 |
| Grain64 | 24.6 ns | 2.7% | **0.62× [0.57–0.67]** | 40.6 M | checksum=9.227e+07 |
| Grain1024 | 21.1 ns | 6.4% | **0.72× [0.65–0.79]** | 47.4 M | checksum=9.225e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 729 ns | 2.2% | baseline | 1.37 M | checksum=2.755e+08 |
| Each | 1.21 µs | 5.0% | **0.61× [0.53–0.70]** | 825 k | checksum=2.738e+08 |
| Grain1 | 9.6 µs | 3.0% | **0.07× [0.07–0.08]** | 104 k | checksum=2.714e+08 |
| Grain64 | 1.1 µs | 5.5% | **0.66× [0.59–0.77]** | 913 k | checksum=2.737e+08 |
| Grain1024 | 741 ns | 7.6% | 1.02× [0.85–1.06] | 1.35 M | checksum=2.742e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 685 ns | 0.9% | baseline | 1.46 M | checksum=2.794e+08 |
| Each | 1.2 µs | 3.0% | **0.58× [0.55–0.60]** | 832 k | checksum=2.776e+08 |
| Grain1 | 9.44 µs | 2.4% | **0.07× [0.07–0.08]** | 106 k | checksum=2.753e+08 |
| Grain64 | 1.06 µs | 2.6% | **0.66× [0.59–0.69]** | 941 k | checksum=2.779e+08 |
| Grain1024 | 703 ns | 2.1% | 0.98× [0.83–1.00] | 1.42 M | checksum=2.794e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.8 µs | 1.3% | baseline | 48 k | checksum=5.235e+09 |
| Each | 28.5 µs | 1.9% | **0.72× [0.72–0.76]** | 35 k | checksum=5.234e+09 |
| Grain1 | 221 µs | 2.4% | **0.09× [0.09–0.10]** | 4.52 k | checksum=5.231e+09 |
| Grain64 | 27.8 µs | 4.4% | **0.76× [0.66–0.77]** | 36 k | checksum=5.234e+09 |
| Grain1024 | 21.4 µs | 2.5% | 0.98× [0.79–1.01] | 46.6 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.8 µs | 1.3% | baseline | 48 k | checksum=5.225e+09 |
| Each | 28.5 µs | 1.4% | **0.73× [0.65–0.75]** | 35.1 k | checksum=5.225e+09 |
| Grain1 | 219 µs | 2.1% | **0.09× [0.08–0.10]** | 4.58 k | checksum=5.222e+09 |
| Grain64 | 27.2 µs | 1.7% | **0.77× [0.74–0.82]** | 36.8 k | checksum=5.225e+09 |
| Grain1024 | 22.2 µs | 5.1% | **0.95× [0.86–0.99]** | 45 k | checksum=5.226e+09 |

