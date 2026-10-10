# Benchmark run 20261010-011517-459a0d0-r4-control

- rows: ok in 1 s
- nbody: ok in 22 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.4% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.00–1.66]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.5 µs | 0.2% | 1.01× [0.86–1.68] | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.5 µs | 0.2% | **1.02× [1.01–1.67]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.5 µs | 0.2% | **1.01× [1.01–1.67]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.5 µs | 0.1% | **1.02× [1.01–1.67]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.5 µs | 0.2% | **1.02× [1.01–1.67]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.5 µs | 0.2% | **1.02× [1.01–1.68]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.5 µs | 0.3% | 1.01× [0.63–1.03] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.02× [1.01–1.68]** | 64.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.5 µs | 0.1% | **1.02× [1.01–1.68]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.2% | **1.01× [1.01–1.37]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.5 µs | 0.1% | **1.02× [1.01–1.42]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.1% | **1.01× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.04× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.7% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.04× [1.03–1.05]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.2% | baseline | 65.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.04× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.35 ms | 5.2% | baseline | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.37 ms | 4.2% | 1.01× [0.85–1.24] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.39 ms | 3.2% | 1.01× [0.91–1.22] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.38 ms | 2.0% | 1.00× [0.95–1.18] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.49 ms | 5.2% | 0.99× [0.88–1.26] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.33 ms | 4.7% | 1.01× [0.90–1.29] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.32 ms | 4.9% | 1.02× [0.80–1.24] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.38 ms | 3.0% | 0.99× [0.91–1.17] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.37 ms | 7.1% | **1.86× [1.29–2.07]** | 422 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.25 ms | 2.5% | **1.96× [1.68–2.15]** | 444 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.43 ms | 3.0% | 1.02× [0.79–1.22] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.32 ms | 4.1% | 0.98× [0.82–1.21] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.32 ms | 16.0% | **3.65× [2.59–3.82]** | 759 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 1.38 ms | 16.5% | **3.14× [1.87–3.78]** | 724 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.33 ms | 4.0% | baseline | 231 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.32 ms | 4.2% | 1.01× [0.93–1.09] | 231 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.28 ms | 2.3% | baseline | 233 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.4 ms | 3.7% | 0.98× [0.89–1.03] | 227 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.22 ms | 5.4% | baseline | 451 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.23 ms | 3.7% | 0.98× [0.94–1.10] | 449 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.42 ms | 10.5% | baseline | 413 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.41 ms | 2.9% | 1.03× [0.91–1.55] | 416 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.7 ms | 11.2% | baseline | 588 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.86 ms | 12.4% | 0.98× [0.95–1.03] | 538 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.15 ms | 3.3% | baseline | 464 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.1 ms | 4.6% | 1.00× [0.93–1.28] | 476 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 72 ms | 3.4% | baseline | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.1 ms | 3.6% | 1.01× [0.88–1.05] | 13.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 72.6 ms | 4.1% | 0.98× [0.85–1.07] | 13.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 75 ms | 4.6% | 0.97× [0.87–1.04] | 13.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 76.3 ms | 4.0% | 0.97× [0.85–1.04] | 13.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71.4 ms | 4.1% | 1.03× [0.89–1.08] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 40.2 ms | 9.7% | **1.85× [1.12–2.01]** | 24.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 45.8 ms | 19.9% | **1.63× [1.10–1.95]** | 21.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 54.1 ms | 17.6% | **1.33× [1.11–1.99]** | 18.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 45.3 ms | 24.6% | **1.66× [1.11–2.02]** | 22.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.6 ms | 2.3% | **2.20× [2.04–3.62]** | 30.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.5 ms | 0.9% | **2.21× [2.02–2.38]** | 30.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 26.6 ms | 13.5% | **2.74× [2.05–3.12]** | 37.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 2.9% | **2.20× [2.02–3.34]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 74 ms | 2.5% | baseline | 13.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 75.2 ms | 2.2% | 0.98× [0.96–1.16] | 13.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 74.5 ms | 4.1% | baseline | 13.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 74.8 ms | 4.3% | 0.96× [0.94–1.01] | 13.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 49.4 ms | 23.8% | baseline | 20.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.1 ms | 1.8% | **1.42× [1.05–1.74]** | 28.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 52.4 ms | 21.8% | baseline | 19.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 39.5 ms | 5.8% | 1.11× [0.96–1.43] | 25.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32.2 ms | 0.8% | baseline | 31 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.5 ms | 0.8% | **0.98× [0.91–1.00]** | 30.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 29.1 ms | 11.5% | baseline | 34.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.5 ms | 1.0% | **0.91× [0.74–1.00]** | 30.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.1 ns | 3.2% | baseline | 70.8 M | checksum=9.403e+07 |
| Each | 21.7 ns | 3.8% | **0.66× [0.63–0.79]** | 46.1 M | checksum=9.374e+07 |
| Grain1 | 144 ns | 1.9% | **0.10× [0.10–0.12]** | 6.95 M | checksum=9.281e+07 |
| Grain64 | 23.9 ns | 5.0% | **0.60× [0.53–0.74]** | 41.8 M | checksum=9.395e+07 |
| Grain1024 | 17.8 ns | 5.5% | **0.82× [0.74–0.99]** | 56.3 M | checksum=9.397e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 ns | 6.8% | baseline | 62.8 M | checksum=9.204e+07 |
| Each | 22 ns | 9.3% | **0.73× [0.65–0.86]** | 45.5 M | checksum=9.197e+07 |
| Grain1 | 145 ns | 2.3% | **0.11× [0.11–0.14]** | 6.89 M | checksum=9.085e+07 |
| Grain64 | 25.2 ns | 5.2% | **0.63× [0.56–0.71]** | 39.8 M | checksum=9.183e+07 |
| Grain1024 | 20.9 ns | 4.3% | **0.80× [0.71–0.87]** | 47.8 M | checksum=9.189e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 702 ns | 3.9% | baseline | 1.42 M | checksum=2.693e+08 |
| Each | 1.16 µs | 1.8% | **0.60× [0.55–0.69]** | 863 k | checksum=2.677e+08 |
| Grain1 | 9.52 µs | 2.3% | **0.08× [0.07–0.09]** | 105 k | checksum=2.654e+08 |
| Grain64 | 1.1 µs | 5.0% | **0.65× [0.59–0.72]** | 909 k | checksum=2.678e+08 |
| Grain1024 | 730 ns | 5.2% | 0.99× [0.93–1.09] | 1.37 M | checksum=2.693e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 734 ns | 2.9% | baseline | 1.36 M | checksum=2.776e+08 |
| Each | 1.28 µs | 4.6% | **0.58× [0.55–0.67]** | 784 k | checksum=2.759e+08 |
| Grain1 | 9.04 µs | 1.2% | **0.08× [0.08–0.09]** | 111 k | checksum=2.737e+08 |
| Grain64 | 1.11 µs | 3.3% | **0.69× [0.63–0.75]** | 903 k | checksum=2.757e+08 |
| Grain1024 | 725 ns | 4.7% | 1.02× [0.90–1.09] | 1.38 M | checksum=2.777e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.7 µs | 4.2% | baseline | 46.1 k | checksum=5.23e+09 |
| Each | 29.1 µs | 4.3% | **0.75× [0.67–0.83]** | 34.4 k | checksum=5.229e+09 |
| Grain1 | 223 µs | 2.8% | **0.10× [0.09–0.11]** | 4.48 k | checksum=5.227e+09 |
| Grain64 | 28.1 µs | 6.2% | **0.77× [0.66–0.83]** | 35.6 k | checksum=5.229e+09 |
| Grain1024 | 21.6 µs | 2.3% | 0.97× [0.96–1.04] | 46.2 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 23 µs | 5.8% | baseline | 43.5 k | checksum=5.225e+09 |
| Each | 30.8 µs | 7.6% | **0.76× [0.63–0.82]** | 32.5 k | checksum=5.224e+09 |
| Grain1 | 219 µs | 0.7% | **0.10× [0.09–0.11]** | 4.56 k | checksum=5.222e+09 |
| Grain64 | 29.7 µs | 6.2% | **0.77× [0.67–0.81]** | 33.7 k | checksum=5.225e+09 |
| Grain1024 | 23.8 µs | 5.4% | 0.95× [0.88–1.12] | 42.1 k | checksum=5.226e+09 |

