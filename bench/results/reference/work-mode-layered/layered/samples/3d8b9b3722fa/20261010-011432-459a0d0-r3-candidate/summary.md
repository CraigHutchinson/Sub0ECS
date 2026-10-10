# Benchmark run 20261010-011432-459a0d0-r3-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.2% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | 1.01× [1.00–1.02] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.7% | **0.97× [0.96–0.98]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 1.0% | **0.96× [0.95–0.98]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.5% | **0.97× [0.96–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.8% | **0.97× [0.95–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.6% | **0.97× [0.21–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 0.6% | **0.97× [0.44–0.99]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.3% | **0.97× [0.96–0.98]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.3% | **0.97× [0.96–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.5% | **0.97× [0.96–0.99]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 0.4% | **0.97× [0.87–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.3 µs | 0.3% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.5% | **0.97× [0.95–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.6% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.5% | 1.00× [0.99–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.5% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.6% | **0.98× [0.98–0.99]** | 61.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.9% | baseline | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 1.3% | 0.99× [0.98–1.00] | 61.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.6 µs | 0.1% | baseline | 56.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 18.8 µs | 0.4% | **0.94× [0.94–0.94]** | 53.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.46 ms | 9.0% | baseline | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.51 ms | 4.1% | 0.99× [0.83–1.26] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.41 ms | 5.4% | 0.99× [0.86–1.09] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.31 ms | 3.4% | 1.01× [0.93–1.28] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.61 ms | 7.3% | 0.94× [0.87–1.11] | 217 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.5 ms | 3.4% | 0.97× [0.88–1.22] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.54 ms | 6.7% | 0.97× [0.87–1.07] | 220 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.45 ms | 4.5% | 1.01× [0.91–1.09] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.35 ms | 7.8% | **1.87× [1.76–2.41]** | 425 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.47 ms | 6.1% | **1.70× [1.07–2.06]** | 405 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.84 ms | 12.8% | 0.96× [0.54–1.22] | 206 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.46 ms | 5.4% | 0.95× [0.77–1.21] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.56 ms | 22.6% | **2.84× [1.92–3.74]** | 639 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.27 ms | 4.4% | **1.95× [1.40–2.13]** | 441 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.39 ms | 3.0% | baseline | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.26 ms | 3.3% | 1.03× [0.96–1.09] | 235 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.26 ms | 4.6% | baseline | 234 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.42 ms | 3.5% | 0.96× [0.89–1.02] | 226 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.33 ms | 11.6% | baseline | 429 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.06 ms | 1.1% | **0.61× [0.51–0.97]** | 246 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.24 ms | 3.1% | baseline | 446 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.37 ms | 7.4% | 1.00× [0.93–1.08] | 423 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.05 ms | 1.7% | baseline | 489 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.07 ms | 1.5% | 0.99× [0.96–1.01] | 483 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.11 ms | 1.1% | baseline | 475 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.07 ms | 0.5% | **1.01× [1.01–1.03]** | 484 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.7 ms | 1.8% | baseline | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.7 ms | 2.1% | 0.98× [0.92–1.05] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 69.4 ms | 3.1% | 1.00× [0.86–1.03] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 69.5 ms | 1.9% | 1.00× [0.94–1.02] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 69.8 ms | 2.2% | 1.00× [0.95–1.06] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 68.8 ms | 2.3% | 1.00× [0.97–1.09] | 14.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.3 ms | 4.0% | **1.90× [1.22–2.02]** | 27.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.1 ms | 3.3% | **1.90× [1.37–2.07]** | 27.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.2 ms | 2.6% | **1.97× [1.39–2.11]** | 28.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 36.8 ms | 3.6% | **1.91× [1.60–1.98]** | 27.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.6 ms | 17.1% | **2.15× [1.86–3.72]** | 30.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 22.4 ms | 23.2% | **3.04× [2.16–3.85]** | 44.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23 ms | 8.0% | **3.01× [2.79–3.83]** | 43.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 23.1 ms | 27.8% | **2.96× [2.16–3.82]** | 43.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.8 ms | 2.6% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.3 ms | 2.3% | 0.99× [0.96–1.06] | 14 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72.1 ms | 2.3% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.4 ms | 2.4% | 1.00× [0.95–1.02] | 13.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.6 ms | 1.0% | baseline | 28.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.2 ms | 4.8% | **0.97× [0.68–0.99]** | 27.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 38.1 ms | 8.9% | baseline | 26.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 47.4 ms | 24.7% | **0.90× [0.67–0.97]** | 21.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.2 ms | 22.1% | baseline | 45.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 20.5 ms | 16.5% | **0.97× [0.80–0.99]** | 48.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.8 ms | 6.1% | baseline | 53.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 19.2 ms | 4.4% | 0.97× [0.95–1.02] | 52 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 ns | 6.9% | baseline | 65.5 M | checksum=9.361e+07 |
| Each | 16.2 ns | 2.5% | 0.92× [0.89–1.15] | 61.7 M | checksum=9.324e+07 |
| Grain1 | 22.5 ns | 3.5% | **0.67× [0.60–0.82]** | 44.4 M | checksum=9.356e+07 |
| Grain64 | 18.8 ns | 4.0% | **0.84× [0.74–0.93]** | 53.3 M | checksum=9.36e+07 |
| Grain1024 | 24.1 ns | 10.2% | **0.66× [0.56–0.76]** | 41.6 M | checksum=9.36e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.1 ns | 12.5% | baseline | 58.4 M | checksum=8.391e+07 |
| Each | 19.8 ns | 8.2% | **0.80× [0.67–0.99]** | 50.5 M | checksum=8.387e+07 |
| Grain1 | 152 ns | 4.0% | **0.10× [0.10–0.13]** | 6.6 M | checksum=8.282e+07 |
| Grain64 | 23.9 ns | 15.3% | **0.69× [0.51–0.81]** | 41.9 M | checksum=8.385e+07 |
| Grain1024 | 21.4 ns | 4.5% | **0.72× [0.51–0.84]** | 46.7 M | checksum=8.385e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 748 ns | 7.4% | baseline | 1.34 M | checksum=2.945e+08 |
| Each | 794 ns | 9.1% | 1.03× [0.85–1.44] | 1.26 M | checksum=2.948e+08 |
| Grain1 | 1.3 µs | 5.1% | **0.59× [0.53–0.73]** | 771 k | checksum=2.926e+08 |
| Grain64 | 905 ns | 10.8% | 0.84× [0.74–1.15] | 1.1 M | checksum=2.929e+08 |
| Grain1024 | 1.29 µs | 3.7% | **0.58× [0.53–0.67]** | 777 k | checksum=2.927e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 782 ns | 6.5% | baseline | 1.28 M | checksum=2.802e+08 |
| Each | 752 ns | 5.6% | 1.00× [0.80–1.14] | 1.33 M | checksum=2.803e+08 |
| Grain1 | 9.73 µs | 3.8% | **0.08× [0.07–0.09]** | 103 k | checksum=2.763e+08 |
| Grain64 | 1.15 µs | 6.8% | **0.66× [0.61–0.76]** | 870 k | checksum=2.783e+08 |
| Grain1024 | 1.26 µs | 2.7% | **0.60× [0.54–0.69]** | 793 k | checksum=2.785e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.7 µs | 10.1% | baseline | 44 k | checksum=5.234e+09 |
| Each | 23.2 µs | 6.6% | 0.99× [0.87–1.17] | 43.1 k | checksum=5.235e+09 |
| Grain1 | 31.1 µs | 4.3% | **0.73× [0.66–0.86]** | 32.1 k | checksum=5.233e+09 |
| Grain64 | 22.4 µs | 6.3% | 0.99× [0.86–1.22] | 44.7 k | checksum=5.235e+09 |
| Grain1024 | 30.3 µs | 3.8% | **0.74× [0.67–0.85]** | 33 k | checksum=5.232e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.7 µs | 4.5% | baseline | 46.2 k | checksum=5.23e+09 |
| Each | 23 µs | 8.4% | 0.94× [0.85–1.11] | 43.5 k | checksum=5.23e+09 |
| Grain1 | 229 µs | 5.5% | **0.09× [0.09–0.11]** | 4.36 k | checksum=5.227e+09 |
| Grain64 | 27.8 µs | 2.0% | **0.80× [0.72–0.97]** | 35.9 k | checksum=5.229e+09 |
| Grain1024 | 32.3 µs | 7.2% | **0.69× [0.60–0.80]** | 30.9 k | checksum=5.229e+09 |

