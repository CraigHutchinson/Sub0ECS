# Benchmark run 20261010-011542-459a0d0-r5-control

- rows: ok in 1 s
- nbody: ok in 21 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.4% | baseline | 62.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.4% | **1.02× [1.01–1.03]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.6 µs | 0.3% | 1.02× [0.98–1.03] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.6 µs | 0.5% | 1.02× [0.88–1.03] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.6 µs | 0.3% | **1.02× [1.01–1.03]** | 64 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.5 µs | 0.3% | 1.02× [0.99–1.03] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.2% | **1.02× [1.00–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.6 µs | 0.1% | **1.02× [1.01–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.6 µs | 0.3% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.6 µs | 0.4% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.6 µs | 0.3% | **1.02× [1.01–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.6 µs | 0.2% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.6 µs | 0.4% | **1.02× [1.01–1.02]** | 63.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.6 µs | 0.2% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 1.1% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.00–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.6% | baseline | 64.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | **0.99× [0.99–1.00]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.6 µs | 0.5% | baseline | 63.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.01× [1.00–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.6% | baseline | 63.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.01× [1.00–1.02]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.3% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | 0.99× [0.99–1.00] | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.64 ms | 8.2% | baseline | 215 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.5 ms | 4.0% | 1.00× [0.81–1.28] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.59 ms | 3.9% | 0.98× [0.89–1.24] | 218 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.63 ms | 2.5% | 0.97× [0.89–1.34] | 216 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.79 ms | 6.5% | 0.98× [0.79–1.27] | 209 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.53 ms | 2.6% | 1.01× [0.90–1.37] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.56 ms | 4.9% | 1.00× [0.86–1.27] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.51 ms | 2.8% | 0.99× [0.91–1.39] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.05 ms | 3.0% | **1.36× [1.01–2.06]** | 247 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 3.05 ms | 26.4% | **1.67× [1.03–2.12]** | 328 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.48 ms | 4.1% | 1.02× [0.89–1.34] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.53 ms | 4.9% | 1.01× [0.89–1.30] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.73 ms | 7.6% | **2.58× [2.01–3.63]** | 579 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.2 ms | 5.5% | **2.07× [1.52–2.77]** | 454 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.36 ms | 4.0% | baseline | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.52 ms | 5.7% | 1.01× [0.91–1.08] | 221 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.39 ms | 4.3% | baseline | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.4 ms | 3.7% | 1.00× [0.92–1.02] | 227 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.38 ms | 5.4% | baseline | 421 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.04 ms | 1.5% | **0.61× [0.58–0.78]** | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.62 ms | 9.5% | baseline | 381 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.37 ms | 3.8% | **1.08× [1.05–1.76]** | 423 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.05 ms | 8.7% | baseline | 488 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.79 ms | 13.0% | 1.01× [0.98–1.14] | 559 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.11 ms | 1.1% | baseline | 474 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.13 ms | 2.8% | 0.99× [0.95–1.02] | 470 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.2 ms | 2.4% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 71 ms | 2.3% | 1.01× [0.96–1.09] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 72.5 ms | 2.4% | 1.00× [0.95–1.10] | 13.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 73.4 ms | 3.3% | 0.99× [0.91–1.07] | 13.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 69.8 ms | 3.8% | 1.02× [0.94–1.10] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71.6 ms | 2.9% | 1.01× [0.98–1.08] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.8 ms | 3.0% | **1.96× [1.43–2.11]** | 27.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.7 ms | 5.6% | **1.97× [1.09–2.23]** | 27.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 37 ms | 4.0% | **1.95× [1.11–2.17]** | 27 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 35.3 ms | 4.9% | **2.03× [1.12–2.26]** | 28.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.5 ms | 0.6% | **2.29× [2.18–3.82]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 0.3% | **2.22× [2.14–3.75]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.6 ms | 3.9% | **3.04× [2.32–3.69]** | 42.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 4.0% | **2.22× [2.08–3.79]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.2 ms | 2.9% | baseline | 13.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.2 ms | 2.3% | 1.00× [0.98–1.06] | 13.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.5 ms | 0.6% | baseline | 13.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.2 ms | 1.0% | 0.99× [0.94–1.01] | 13.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.4 ms | 1.6% | baseline | 29 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 34.2 ms | 1.2% | 1.00× [0.72–1.02] | 29.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 37.2 ms | 6.1% | baseline | 26.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 57.8 ms | 11.6% | 0.79× [0.55–1.02] | 17.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.3 ms | 17.9% | baseline | 47 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 20.9 ms | 17.8% | 0.98× [0.88–1.28] | 47.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32.8 ms | 8.9% | baseline | 30.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.5 ms | 3.6% | **1.02× [1.00–1.36]** | 30.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.2 ns | 3.8% | baseline | 70.6 M | checksum=9.16e+07 |
| Each | 20.2 ns | 1.0% | **0.69× [0.64–0.81]** | 49.4 M | checksum=9.164e+07 |
| Grain1 | 142 ns | 2.3% | **0.10× [0.09–0.11]** | 7.03 M | checksum=9.038e+07 |
| Grain64 | 22.9 ns | 2.7% | **0.60× [0.58–0.63]** | 43.6 M | checksum=9.159e+07 |
| Grain1024 | 17.8 ns | 2.0% | **0.78× [0.75–0.98]** | 56.2 M | checksum=9.142e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.5 ns | 4.5% | baseline | 64.4 M | checksum=8.997e+07 |
| Each | 21.8 ns | 8.6% | **0.74× [0.54–0.86]** | 45.9 M | checksum=9.013e+07 |
| Grain1 | 149 ns | 2.2% | **0.10× [0.06–0.12]** | 6.71 M | checksum=8.897e+07 |
| Grain64 | 25.3 ns | 5.3% | **0.60× [0.40–0.72]** | 39.5 M | checksum=9.002e+07 |
| Grain1024 | 20.7 ns | 2.7% | **0.74× [0.71–0.84]** | 48.3 M | checksum=8.995e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 748 ns | 9.8% | baseline | 1.34 M | checksum=2.797e+08 |
| Each | 1.21 µs | 2.2% | **0.60× [0.55–0.71]** | 830 k | checksum=2.78e+08 |
| Grain1 | 9.24 µs | 1.9% | **0.08× [0.07–0.09]** | 108 k | checksum=2.757e+08 |
| Grain64 | 1.16 µs | 5.6% | **0.64× [0.58–0.75]** | 859 k | checksum=2.781e+08 |
| Grain1024 | 708 ns | 3.4% | 0.99× [0.88–1.13] | 1.41 M | checksum=2.798e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 746 ns | 10.3% | baseline | 1.34 M | checksum=2.832e+08 |
| Each | 1.34 µs | 11.2% | **0.59× [0.49–0.85]** | 744 k | checksum=2.81e+08 |
| Grain1 | 10.4 µs | 8.3% | **0.07× [0.06–0.10]** | 96 k | checksum=2.79e+08 |
| Grain64 | 1.13 µs | 6.7% | **0.64× [0.59–0.81]** | 889 k | checksum=2.816e+08 |
| Grain1024 | 735 ns | 5.1% | 0.98× [0.92–1.32] | 1.36 M | checksum=2.82e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22 µs | 6.7% | baseline | 45.4 k | checksum=5.17e+09 |
| Each | 28.9 µs | 3.3% | **0.76× [0.69–0.80]** | 34.6 k | checksum=5.17e+09 |
| Grain1 | 232 µs | 8.1% | **0.09× [0.08–0.11]** | 4.3 k | checksum=5.168e+09 |
| Grain64 | 27.8 µs | 3.7% | **0.77× [0.64–0.86]** | 36 k | checksum=5.171e+09 |
| Grain1024 | 22.6 µs | 4.1% | 0.98× [0.79–1.04] | 44.3 k | checksum=5.17e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.1 µs | 2.4% | baseline | 47.5 k | checksum=5.224e+09 |
| Each | 28.6 µs | 1.7% | **0.73× [0.70–0.87]** | 34.9 k | checksum=5.225e+09 |
| Grain1 | 223 µs | 4.0% | **0.10× [0.09–0.11]** | 4.48 k | checksum=5.222e+09 |
| Grain64 | 27.6 µs | 3.7% | **0.79× [0.75–0.82]** | 36.2 k | checksum=5.225e+09 |
| Grain1024 | 22 µs | 2.3% | 0.95× [0.86–1.01] | 45.5 k | checksum=5.226e+09 |

