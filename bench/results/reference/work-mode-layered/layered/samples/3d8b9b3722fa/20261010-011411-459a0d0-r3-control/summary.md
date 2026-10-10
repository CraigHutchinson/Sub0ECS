# Benchmark run 20261010-011411-459a0d0-r3-control

- rows: ok in 1 s
- nbody: ok in 19 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.7% | baseline | 62.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.8 µs | 1.7% | 1.01× [0.94–1.19] | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.5 µs | 6.3% | 1.02× [0.56–1.19] | 60.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.5 µs | 0.2% | **1.02× [1.01–1.04]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.6 µs | 0.3% | 1.02× [0.18–1.19] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 4.8% | 1.02× [0.81–1.19] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.2 µs | 4.3% | 1.01× [0.94–1.19] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 3.4% | 1.01× [0.95–1.19] | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.5 µs | 0.4% | 1.02× [0.86–1.19] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.8 µs | 2.1% | 1.01× [0.81–1.19] | 63.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.9 µs | 2.4% | 1.02× [0.47–1.19] | 63 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 3.9% | 0.99× [0.94–1.29] | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.4 µs | 3.2% | 1.01× [0.94–1.19] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.7 µs | 7.9% | 0.95× [0.54–1.06] | 59.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.6% | baseline | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.7% | **1.03× [1.03–1.05]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | **0.98× [0.98–0.99]** | 64.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.6% | baseline | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **1.04× [1.04–1.06]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.1% | **0.99× [0.98–0.99]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.7% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.04× [1.04–1.06]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.99–1.00]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.36 ms | 5.5% | baseline | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.17 ms | 3.2% | 1.02× [0.98–1.14] | 240 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.38 ms | 2.1% | 1.01× [0.94–1.09] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.39 ms | 3.2% | 1.01× [0.96–1.08] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.25 ms | 3.8% | 1.01× [0.93–1.11] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.33 ms | 3.9% | 0.99× [0.92–1.14] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.3 ms | 3.6% | 1.01× [0.92–1.13] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.36 ms | 6.2% | 1.01× [0.95–1.12] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.31 ms | 3.7% | **1.88× [1.76–2.14]** | 434 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.39 ms | 10.5% | **1.83× [1.29–2.12]** | 419 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.2 ms | 4.2% | 1.01× [0.95–1.14] | 238 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.38 ms | 4.5% | 0.96× [0.93–1.10] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.57 ms | 1.5% | **2.76× [2.36–3.41]** | 637 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.12 ms | 1.3% | **2.03× [1.92–2.19]** | 472 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.23 ms | 4.2% | baseline | 237 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.19 ms | 3.7% | **1.01× [1.00–1.04]** | 239 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.2 ms | 4.2% | baseline | 238 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.27 ms | 3.6% | 1.00× [0.97–1.05] | 234 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.13 ms | 3.2% | baseline | 469 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.03 ms | 1.5% | 0.54× [0.51–1.02] | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.24 ms | 2.6% | baseline | 447 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.24 ms | 4.1% | 1.01× [0.97–1.05] | 447 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.53 ms | 0.5% | baseline | 652 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.55 ms | 1.3% | 0.99× [0.97–1.01] | 643 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.13 ms | 2.4% | baseline | 470 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.55 ms | 22.2% | **1.48× [1.01–1.75]** | 645 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.5 ms | 1.4% | baseline | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 71 ms | 2.8% | 0.99× [0.95–1.03] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 69.7 ms | 2.5% | 1.01× [0.97–1.09] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 69.6 ms | 2.6% | 1.00× [0.95–1.03] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 71.8 ms | 2.5% | 0.97× [0.93–1.02] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.9 ms | 2.9% | 0.99× [0.97–1.05] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.3 ms | 3.7% | **1.94× [1.63–2.07]** | 27.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.6 ms | 3.5% | **1.97× [1.57–2.11]** | 27.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 34.7 ms | 2.7% | **2.02× [1.94–2.10]** | 28.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 36 ms | 2.9% | **2.00× [1.18–2.06]** | 27.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 21.2 ms | 16.0% | **3.30× [2.21–4.05]** | 47.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 21.6 ms | 19.1% | **3.29× [2.21–3.84]** | 46.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 18.2 ms | 3.7% | **3.82× [2.91–4.06]** | 54.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 22.1 ms | 12.8% | **3.17× [2.17–3.87]** | 45.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.2 ms | 1.4% | baseline | 14.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 68.9 ms | 1.7% | 1.01× [0.97–1.02] | 14.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.6 ms | 1.3% | baseline | 14.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 68.9 ms | 1.4% | 0.99× [0.98–1.02] | 14.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.8 ms | 1.5% | baseline | 28.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 34.8 ms | 1.1% | 1.00× [0.99–1.02] | 28.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.6 ms | 1.9% | baseline | 28.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.3 ms | 2.0% | 0.97× [0.92–1.01] | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.9 ms | 2.6% | baseline | 55.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.4 ms | 3.0% | 0.98× [0.96–1.01] | 54.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.1 ms | 2.7% | baseline | 55.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.8 ms | 5.5% | 0.97× [0.85–1.02] | 53.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14 ns | 1.5% | baseline | 71.4 M | checksum=9.402e+07 |
| Each | 20.6 ns | 1.7% | **0.68× [0.66–0.71]** | 48.4 M | checksum=9.345e+07 |
| Grain1 | 139 ns | 1.3% | **0.10× [0.10–0.11]** | 7.18 M | checksum=9.273e+07 |
| Grain64 | 22.4 ns | 1.8% | **0.62× [0.55–0.64]** | 44.7 M | checksum=9.393e+07 |
| Grain1024 | 17.4 ns | 1.3% | **0.81× [0.74–0.84]** | 57.5 M | checksum=9.396e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 ns | 3.1% | baseline | 65.2 M | checksum=8.826e+07 |
| Each | 20.4 ns | 1.8% | **0.75× [0.73–0.81]** | 49 M | checksum=8.829e+07 |
| Grain1 | 137 ns | 0.2% | **0.11× [0.11–0.12]** | 7.29 M | checksum=8.716e+07 |
| Grain64 | 24.2 ns | 1.5% | **0.63× [0.59–0.72]** | 41.3 M | checksum=8.823e+07 |
| Grain1024 | 20.9 ns | 3.2% | **0.74× [0.71–0.80]** | 47.8 M | checksum=8.815e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 683 ns | 0.9% | baseline | 1.46 M | checksum=2.543e+08 |
| Each | 1.15 µs | 1.1% | **0.59× [0.56–0.61]** | 872 k | checksum=2.53e+08 |
| Grain1 | 8.86 µs | 1.3% | **0.08× [0.08–0.08]** | 113 k | checksum=2.506e+08 |
| Grain64 | 1.05 µs | 1.9% | **0.65× [0.59–0.66]** | 950 k | checksum=2.532e+08 |
| Grain1024 | 690 ns | 1.3% | 0.99× [0.97–1.01] | 1.45 M | checksum=2.54e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 683 ns | 0.9% | baseline | 1.46 M | checksum=2.82e+08 |
| Each | 1.17 µs | 1.3% | **0.58× [0.56–0.60]** | 858 k | checksum=2.802e+08 |
| Grain1 | 8.75 µs | 1.2% | **0.08× [0.08–0.08]** | 114 k | checksum=2.779e+08 |
| Grain64 | 1.05 µs | 2.1% | **0.65× [0.60–0.67]** | 951 k | checksum=2.805e+08 |
| Grain1024 | 697 ns | 1.4% | **0.97× [0.93–1.00]** | 1.43 M | checksum=2.82e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.7 µs | 0.7% | baseline | 48.3 k | checksum=5.235e+09 |
| Each | 28.1 µs | 1.4% | **0.73× [0.70–0.75]** | 35.6 k | checksum=5.234e+09 |
| Grain1 | 216 µs | 1.7% | **0.10× [0.09–0.10]** | 4.62 k | checksum=5.231e+09 |
| Grain64 | 27 µs | 1.4% | **0.77× [0.73–0.79]** | 37.1 k | checksum=5.234e+09 |
| Grain1024 | 21.4 µs | 2.1% | **0.97× [0.90–1.00]** | 46.7 k | checksum=5.234e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.3 µs | 2.7% | baseline | 46.9 k | checksum=5.235e+09 |
| Each | 30.3 µs | 4.4% | **0.74× [0.67–0.77]** | 33 k | checksum=5.234e+09 |
| Grain1 | 220 µs | 1.3% | **0.09× [0.09–0.10]** | 4.54 k | checksum=5.231e+09 |
| Grain64 | 28.4 µs | 6.7% | **0.74× [0.67–0.81]** | 35.2 k | checksum=5.234e+09 |
| Grain1024 | 21.7 µs | 1.9% | 1.00× [0.94–1.02] | 46.1 k | checksum=5.234e+09 |

