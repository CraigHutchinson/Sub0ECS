# Benchmark run 20261010-091744-459a0d0-r5-control

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.2% | baseline | 62.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.4% | **1.02× [1.01–1.04]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.6 µs | 0.3% | **1.02× [1.02–1.02]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.6 µs | 0.3% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.6 µs | 0.3% | 1.02× [0.92–1.03] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.6 µs | 0.3% | **1.02× [1.02–1.03]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.3% | **1.02× [1.01–1.04]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.5 µs | 0.2% | **1.02× [1.02–1.04]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.6 µs | 0.2% | **1.02× [1.01–1.03]** | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.02× [1.02–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.6 µs | 0.2% | **1.02× [1.02–1.05]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.1% | **1.02× [1.02–1.05]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.6 µs | 0.1% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.2% | **1.02× [1.02–1.04]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.6 µs | 0.3% | baseline | 64.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.5% | 1.00× [0.80–1.01] | 64.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.6 µs | 0.5% | baseline | 63.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.01× [1.00–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.3% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.98–0.99]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.6% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.5 ms | 5.3% | baseline | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.44 ms | 2.3% | 1.00× [0.94–1.40] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.38 ms | 3.7% | 1.05× [0.95–1.46] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.38 ms | 2.6% | 1.02× [0.90–1.54] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.37 ms | 3.7% | 1.03× [0.96–1.48] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.33 ms | 4.8% | 1.04× [0.93–1.36] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.37 ms | 4.3% | 1.02× [0.87–1.55] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.46 ms | 4.1% | 1.01× [0.88–1.55] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.3 ms | 3.0% | **1.91× [1.77–2.26]** | 434 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.25 ms | 3.7% | **2.01× [1.04–2.98]** | 444 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.38 ms | 6.2% | 1.04× [0.91–1.59] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.51 ms | 2.7% | 1.02× [0.87–1.09] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.28 ms | 6.3% | **3.61× [3.26–3.90]** | 780 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.14 ms | 11.7% | **2.35× [1.95–3.82]** | 466 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.62 ms | 4.9% | baseline | 216 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.46 ms | 5.0% | 1.01× [0.97–1.09] | 224 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.45 ms | 4.1% | baseline | 225 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.48 ms | 3.6% | 1.02× [0.92–1.13] | 223 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.19 ms | 3.1% | baseline | 458 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.35 ms | 3.4% | **0.92× [0.85–0.96]** | 425 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.66 ms | 5.5% | baseline | 375 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.77 ms | 8.3% | 0.94× [0.73–1.05] | 361 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.45 ms | 17.6% | baseline | 692 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.54 ms | 13.5% | 0.92× [0.85–1.04] | 649 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.33 ms | 9.1% | baseline | 429 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.12 ms | 3.9% | 1.10× [0.86–1.33] | 472 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.5 ms | 1.9% | baseline | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.4 ms | 3.8% | 0.98× [0.87–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70 ms | 4.1% | 0.98× [0.88–1.06] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 69.1 ms | 1.8% | 0.99× [0.92–1.05] | 14.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.9 ms | 3.0% | 0.98× [0.92–1.03] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.2 ms | 2.0% | 0.98× [0.85–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 35.9 ms | 3.7% | **1.92× [1.31–2.01]** | 27.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 37.4 ms | 4.6% | **1.89× [1.60–2.03]** | 26.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 36.5 ms | 4.3% | **1.85× [1.11–2.06]** | 27.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 38.5 ms | 6.7% | **1.80× [1.16–1.95]** | 26 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.5 ms | 0.5% | **2.14× [2.01–3.65]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 3.1% | **2.16× [2.05–3.80]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 24.2 ms | 13.4% | **2.81× [2.18–3.81]** | 41.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 0.2% | **2.15× [2.10–3.41]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.3 ms | 3.4% | baseline | 13.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.5 ms | 3.4% | 1.01× [0.94–1.05] | 13.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.7 ms | 2.3% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.6 ms | 1.7% | 1.00× [0.98–1.04] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.5 ms | 3.2% | baseline | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.6 ms | 1.4% | 0.98× [0.78–1.01] | 27.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.5 ms | 1.3% | baseline | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.4 ms | 1.8% | **0.97× [0.95–1.00]** | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.4 ms | 6.6% | baseline | 54.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 20.1 ms | 11.8% | 0.97× [0.91–1.00] | 49.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.9 ms | 7.4% | baseline | 50.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 20.2 ms | 7.3% | 0.96× [0.83–1.03] | 49.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.5 ns | 5.2% | baseline | 68.8 M | checksum=9.138e+07 |
| Each | 21 ns | 0.6% | **0.68× [0.63–0.72]** | 47.6 M | checksum=9.164e+07 |
| Grain1 | 147 ns | 5.0% | **0.10× [0.08–0.11]** | 6.82 M | checksum=9.046e+07 |
| Grain64 | 22.5 ns | 2.4% | **0.64× [0.60–0.70]** | 44.5 M | checksum=9.159e+07 |
| Grain1024 | 17.9 ns | 3.2% | **0.82× [0.77–0.94]** | 55.8 M | checksum=9.104e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.7 ns | 4.5% | baseline | 59.9 M | checksum=8.903e+07 |
| Each | 21.3 ns | 5.1% | **0.78× [0.53–0.90]** | 46.9 M | checksum=8.882e+07 |
| Grain1 | 144 ns | 3.6% | **0.11× [0.11–0.13]** | 6.92 M | checksum=8.788e+07 |
| Grain64 | 25.1 ns | 4.9% | **0.63× [0.56–0.76]** | 39.8 M | checksum=8.891e+07 |
| Grain1024 | 22.2 ns | 4.8% | **0.78× [0.67–0.93]** | 45.1 M | checksum=8.895e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 685 ns | 1.8% | baseline | 1.46 M | checksum=2.764e+08 |
| Each | 1.21 µs | 5.6% | **0.56× [0.54–0.59]** | 824 k | checksum=2.745e+08 |
| Grain1 | 9.08 µs | 1.6% | **0.07× [0.07–0.08]** | 110 k | checksum=2.723e+08 |
| Grain64 | 1.07 µs | 2.2% | **0.65× [0.62–0.68]** | 931 k | checksum=2.75e+08 |
| Grain1024 | 688 ns | 1.2% | 0.99× [0.93–1.02] | 1.45 M | checksum=2.735e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 691 ns | 2.9% | baseline | 1.45 M | checksum=2.636e+08 |
| Each | 1.17 µs | 0.9% | **0.59× [0.55–0.63]** | 854 k | checksum=2.621e+08 |
| Grain1 | 9.19 µs | 4.0% | **0.08× [0.07–0.08]** | 109 k | checksum=2.598e+08 |
| Grain64 | 1.08 µs | 3.4% | **0.65× [0.59–0.67]** | 927 k | checksum=2.624e+08 |
| Grain1024 | 724 ns | 5.0% | 0.95× [0.87–1.04] | 1.38 M | checksum=2.637e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21 µs | 1.3% | baseline | 47.7 k | checksum=5.235e+09 |
| Each | 29.4 µs | 4.8% | **0.74× [0.67–0.75]** | 34 k | checksum=5.234e+09 |
| Grain1 | 232 µs | 5.7% | **0.09× [0.09–0.10]** | 4.32 k | checksum=5.231e+09 |
| Grain64 | 26.7 µs | 2.5% | **0.79× [0.75–0.83]** | 37.4 k | checksum=5.234e+09 |
| Grain1024 | 22.3 µs | 4.5% | **0.98× [0.91–1.00]** | 44.8 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.1 µs | 2.2% | baseline | 47.4 k | checksum=5.202e+09 |
| Each | 28.5 µs | 0.7% | **0.74× [0.72–0.76]** | 35.1 k | checksum=5.202e+09 |
| Grain1 | 225 µs | 2.5% | **0.09× [0.09–0.10]** | 4.45 k | checksum=5.199e+09 |
| Grain64 | 26.7 µs | 1.3% | **0.79× [0.74–0.82]** | 37.5 k | checksum=5.202e+09 |
| Grain1024 | 22 µs | 2.2% | 0.96× [0.93–1.09] | 45.5 k | checksum=5.202e+09 |

