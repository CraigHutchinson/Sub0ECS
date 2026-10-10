# Benchmark run 20261010-091721-459a0d0-r4-control

- rows: ok in 2 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.3% | baseline | 63.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.5% | 1.01× [0.22–1.01] | 64 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.6 µs | 0.5% | 1.00× [0.33–1.02] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.6 µs | 0.3% | 1.01× [1.00–1.01] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.6 µs | 0.4% | 1.01× [0.88–1.02] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.6 µs | 0.7% | 1.01× [0.12–1.02] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.3% | 1.01× [0.28–1.02] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.6 µs | 0.6% | 1.01× [0.48–1.01] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.6 µs | 0.6% | 1.01× [0.48–1.02] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.6 µs | 0.5% | 1.01× [0.43–1.03] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.7 µs | 1.2% | 1.01× [0.82–1.02] | 63.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.6 µs | 0.7% | 1.00× [0.41–1.03] | 63.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.6 µs | 0.2% | **1.01× [1.00–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.3% | 1.01× [0.44–1.05] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.7% | baseline | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.03× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.5% | **0.98× [0.97–0.99]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 µs | 0.2% | baseline | 61 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.06× [1.05–1.06]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.98× [0.98–0.99]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 µs | 0.2% | baseline | 60.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.06× [1.05–1.07]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.98–0.99]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.56 ms | 8.2% | baseline | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.36 ms | 5.9% | 1.03× [0.83–1.24] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.55 ms | 4.2% | 1.05× [0.87–1.24] | 220 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.53 ms | 4.5% | 1.02× [0.89–1.24] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.51 ms | 6.5% | 1.02× [0.86–1.27] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.33 ms | 4.3% | 1.03× [0.97–1.25] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.42 ms | 5.0% | 1.03× [0.87–1.29] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.45 ms | 4.7% | 1.01× [0.93–1.36] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.69 ms | 15.3% | **1.68× [1.33–2.22]** | 372 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.71 ms | 18.0% | **1.59× [1.07–2.34]** | 369 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.86 ms | 4.6% | 1.01× [0.88–1.11] | 206 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.47 ms | 6.0% | 1.03× [0.91–1.29] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.6 ms | 16.6% | **2.96× [1.63–3.76]** | 624 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.21 ms | 7.3% | **2.08× [1.77–3.71]** | 453 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.2 ms | 4.1% | baseline | 238 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.34 ms | 2.4% | 0.96× [0.92–1.02] | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.28 ms | 4.9% | baseline | 234 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.38 ms | 3.9% | **0.97× [0.87–0.99]** | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.29 ms | 2.7% | baseline | 436 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.36 ms | 5.6% | 0.98× [0.94–1.12] | 424 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.46 ms | 5.0% | baseline | 407 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 3.59 ms | 15.8% | 0.81× [0.60–1.05] | 278 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.55 ms | 2.1% | baseline | 645 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.57 ms | 1.2% | 0.98× [0.77–1.05] | 638 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.1 ms | 0.7% | baseline | 476 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.11 ms | 0.8% | 0.99× [0.97–1.01] | 475 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.8 ms | 3.1% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.4 ms | 3.0% | 1.01× [0.90–1.11] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.6 ms | 5.5% | 1.01× [0.97–1.07] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 69.4 ms | 3.2% | 1.02× [0.97–1.09] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.7 ms | 3.3% | 1.03× [0.98–1.07] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 69.5 ms | 2.0% | 1.03× [0.93–1.09] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 38.1 ms | 8.2% | **1.79× [1.13–1.95]** | 26.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 35.7 ms | 2.1% | **1.95× [1.68–2.17]** | 28 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.4 ms | 5.5% | **1.93× [1.40–2.15]** | 28.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 36.7 ms | 5.5% | **2.00× [1.73–2.16]** | 27.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 30 ms | 11.5% | **2.55× [1.95–3.88]** | 33.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 28.7 ms | 14.0% | **2.60× [2.02–3.87]** | 34.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 22.6 ms | 15.1% | **3.28× [2.58–4.02]** | 44.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 0.5% | **2.23× [1.88–3.65]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.7 ms | 2.0% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 71 ms | 0.9% | 1.02× [0.97–1.07] | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.1 ms | 1.4% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.2 ms | 1.8% | 0.99× [0.97–1.01] | 13.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 38 ms | 11.3% | baseline | 26.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.6 ms | 3.2% | 1.03× [0.70–1.77] | 27.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 36.5 ms | 7.1% | baseline | 27.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.4 ms | 3.8% | 1.00× [0.95–1.12] | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.6 ms | 2.5% | baseline | 53.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.8 ms | 3.2% | 0.97× [0.92–1.05] | 53.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.8 ms | 7.9% | baseline | 50.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 20.9 ms | 15.3% | 1.00× [0.67–1.09] | 47.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.5 ns | 2.5% | baseline | 68.8 M | checksum=9.112e+07 |
| Each | 22 ns | 6.9% | **0.66× [0.62–0.72]** | 45.4 M | checksum=9.164e+07 |
| Grain1 | 147 ns | 4.3% | **0.10× [0.09–0.10]** | 6.8 M | checksum=9.045e+07 |
| Grain64 | 22.8 ns | 3.5% | **0.65× [0.56–0.68]** | 43.9 M | checksum=9.158e+07 |
| Grain1024 | 17.9 ns | 1.2% | **0.82× [0.80–0.87]** | 56 M | checksum=9.117e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 ns | 2.4% | baseline | 62.5 M | checksum=8.825e+07 |
| Each | 20.5 ns | 1.0% | **0.78× [0.76–0.87]** | 48.8 M | checksum=8.845e+07 |
| Grain1 | 141 ns | 2.7% | **0.12× [0.10–0.12]** | 7.08 M | checksum=8.731e+07 |
| Grain64 | 24.6 ns | 1.4% | **0.65× [0.62–0.75]** | 40.6 M | checksum=8.836e+07 |
| Grain1024 | 20.6 ns | 1.3% | **0.77× [0.74–0.89]** | 48.5 M | checksum=8.825e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 749 ns | 8.0% | baseline | 1.33 M | checksum=2.751e+08 |
| Each | 1.24 µs | 7.4% | 0.61× [0.52–2.69] | 807 k | checksum=2.733e+08 |
| Grain1 | 10.1 µs | 11.2% | **0.07× [0.06–0.27]** | 98.9 k | checksum=2.711e+08 |
| Grain64 | 1.15 µs | 11.7% | 0.66× [0.50–2.06] | 867 k | checksum=2.73e+08 |
| Grain1024 | 733 ns | 6.8% | 0.98× [0.68–1.25] | 1.36 M | checksum=2.736e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 677 ns | 2.1% | baseline | 1.48 M | checksum=2.766e+08 |
| Each | 1.21 µs | 4.5% | **0.57× [0.51–0.60]** | 828 k | checksum=2.75e+08 |
| Grain1 | 9.15 µs | 3.7% | **0.07× [0.05–0.08]** | 109 k | checksum=2.728e+08 |
| Grain64 | 1.08 µs | 3.7% | **0.64× [0.59–0.69]** | 922 k | checksum=2.752e+08 |
| Grain1024 | 737 ns | 5.7% | **0.93× [0.85–1.00]** | 1.36 M | checksum=2.768e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 24.2 µs | 14.3% | baseline | 41.3 k | checksum=5.23e+09 |
| Each | 38.9 µs | 25.7% | **0.64× [0.53–0.84]** | 25.7 k | checksum=5.229e+09 |
| Grain1 | 272 µs | 21.8% | **0.10× [0.07–0.11]** | 3.68 k | checksum=5.227e+09 |
| Grain64 | 31.3 µs | 9.9% | **0.81× [0.72–0.98]** | 31.9 k | checksum=5.229e+09 |
| Grain1024 | 22 µs | 3.9% | 0.99× [0.96–1.16] | 45.5 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.1 µs | 6.4% | baseline | 45.3 k | checksum=5.18e+09 |
| Each | 31.8 µs | 5.3% | **0.71× [0.62–0.97]** | 31.5 k | checksum=5.179e+09 |
| Grain1 | 270 µs | 13.8% | **0.08× [0.07–0.11]** | 3.71 k | checksum=5.177e+09 |
| Grain64 | 34 µs | 9.9% | **0.64× [0.54–0.83]** | 29.4 k | checksum=5.179e+09 |
| Grain1024 | 25.6 µs | 10.8% | 0.88× [0.74–1.07] | 39.1 k | checksum=5.179e+09 |

