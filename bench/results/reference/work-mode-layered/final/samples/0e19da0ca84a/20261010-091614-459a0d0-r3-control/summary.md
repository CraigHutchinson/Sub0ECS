# Benchmark run 20261010-091614-459a0d0-r3-control

- rows: ok in 1 s
- nbody: ok in 21 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.2% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.5 µs | 0.2% | **1.01× [1.00–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.5 µs | 0.2% | **1.01× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.5 µs | 0.1% | **1.01× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.6 µs | 0.2% | **1.01× [1.01–1.02]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.5 µs | 0.3% | **1.01× [1.01–1.02]** | 64.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.5 µs | 0.2% | 1.01× [0.98–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.3% | **1.01× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.2% | baseline | 62.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.03× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.4% | baseline | 62.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.03× [1.03–1.05]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.98–0.99]** | 64.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.3% | baseline | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.03× [1.03–1.04]** | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.99–0.99]** | 64.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.12 ms | 1.1% | baseline | 243 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.31 ms | 2.9% | 0.95× [0.92–1.07] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.32 ms | 3.4% | 0.95× [0.90–1.08] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.31 ms | 6.1% | 0.98× [0.91–1.05] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.3 ms | 3.9% | 0.97× [0.93–1.07] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.3 ms | 2.8% | 0.96× [0.79–1.01] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.3 ms | 2.4% | 0.97× [0.92–1.08] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.1 ms | 1.7% | 1.01× [0.90–1.07] | 244 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.23 ms | 4.2% | **1.86× [1.01–2.08]** | 449 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.24 ms | 2.8% | **1.86× [1.74–2.01]** | 446 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.33 ms | 3.1% | 0.97× [0.91–1.04] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.19 ms | 3.9% | 0.99× [0.90–1.09] | 239 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.23 ms | 3.8% | **3.32× [2.94–3.89]** | 812 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 1.4 ms | 19.3% | **3.02× [1.83–3.68]** | 714 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.22 ms | 3.6% | baseline | 237 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.35 ms | 1.5% | 0.96× [0.93–1.01] | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.11 ms | 2.2% | baseline | 243 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.13 ms | 2.4% | 1.00× [0.96–1.04] | 242 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.13 ms | 2.7% | baseline | 469 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.05 ms | 0.1% | **0.53× [0.51–0.56]** | 247 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.24 ms | 3.2% | baseline | 446 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.3 ms | 7.2% | 0.95× [0.88–1.05] | 434 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.56 ms | 2.8% | baseline | 642 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.56 ms | 1.5% | 1.00× [0.98–1.06] | 640 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.12 ms | 1.0% | baseline | 472 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.19 ms | 4.6% | 0.98× [0.91–1.02] | 457 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 68.5 ms | 1.5% | baseline | 14.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.5 ms | 2.4% | 0.99× [0.91–1.06] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 69.3 ms | 1.3% | 0.99× [0.97–1.05] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 69.7 ms | 1.8% | 0.99× [0.95–1.01] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 69.5 ms | 2.3% | 1.00× [0.92–1.03] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.5 ms | 2.8% | 0.97× [0.91–1.05] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36 ms | 4.0% | **1.91× [1.29–2.03]** | 27.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 36.1 ms | 4.4% | **1.92× [1.79–2.12]** | 27.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.2 ms | 3.3% | **1.98× [1.13–2.10]** | 28.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 34.6 ms | 2.2% | **1.96× [1.26–2.18]** | 28.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.5 ms | 0.4% | **2.12× [2.05–3.82]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 0.1% | **2.13× [2.04–2.34]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 22.9 ms | 24.5% | **3.00× [2.13–4.09]** | 43.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 0.3% | **2.12× [2.04–3.87]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 74.9 ms | 3.1% | baseline | 13.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 72 ms | 1.7% | 1.01× [0.98–1.09] | 13.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 73.1 ms | 4.0% | baseline | 13.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 74.3 ms | 5.0% | 0.98× [0.84–1.01] | 13.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 47.8 ms | 26.1% | baseline | 20.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 37.9 ms | 9.2% | 1.10× [0.97–1.22] | 26.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 37.5 ms | 5.1% | baseline | 26.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 38.8 ms | 9.7% | 0.94× [0.74–1.04] | 25.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 23.9 ms | 16.0% | baseline | 41.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 24 ms | 25.7% | 0.99× [0.94–1.16] | 41.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32 ms | 7.2% | baseline | 31.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 21 ms | 14.4% | 1.12× [0.99–1.56] | 47.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.6 ns | 3.7% | baseline | 68.6 M | checksum=8.934e+07 |
| Each | 20.4 ns | 2.5% | **0.70× [0.68–0.77]** | 48.9 M | checksum=8.93e+07 |
| Grain1 | 140 ns | 1.8% | **0.10× [0.10–0.11]** | 7.15 M | checksum=8.822e+07 |
| Grain64 | 22.4 ns | 1.6% | **0.65× [0.61–0.69]** | 44.6 M | checksum=8.937e+07 |
| Grain1024 | 17.7 ns | 2.0% | **0.82× [0.78–0.87]** | 56.4 M | checksum=8.928e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17 ns | 3.1% | baseline | 58.8 M | checksum=9.243e+07 |
| Each | 20.5 ns | 2.3% | **0.83× [0.73–0.95]** | 48.8 M | checksum=9.229e+07 |
| Grain1 | 143 ns | 2.8% | **0.12× [0.11–0.13]** | 7 M | checksum=9.124e+07 |
| Grain64 | 24.7 ns | 3.0% | **0.69× [0.67–0.76]** | 40.5 M | checksum=9.226e+07 |
| Grain1024 | 22.4 ns | 3.1% | **0.76× [0.68–0.86]** | 44.6 M | checksum=9.192e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 681 ns | 0.8% | baseline | 1.47 M | checksum=2.76e+08 |
| Each | 1.15 µs | 1.1% | **0.59× [0.58–0.60]** | 866 k | checksum=2.742e+08 |
| Grain1 | 8.82 µs | 2.3% | **0.08× [0.08–0.08]** | 113 k | checksum=2.72e+08 |
| Grain64 | 1.04 µs | 0.8% | **0.66× [0.64–0.67]** | 965 k | checksum=2.742e+08 |
| Grain1024 | 688 ns | 0.8% | 0.98× [0.88–1.02] | 1.45 M | checksum=2.754e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 668 ns | 1.2% | baseline | 1.5 M | checksum=2.828e+08 |
| Each | 1.21 µs | 6.0% | **0.55× [0.51–0.62]** | 824 k | checksum=2.809e+08 |
| Grain1 | 9 µs | 3.0% | **0.08× [0.07–0.09]** | 111 k | checksum=2.786e+08 |
| Grain64 | 1.09 µs | 1.6% | **0.64× [0.60–0.78]** | 914 k | checksum=2.811e+08 |
| Grain1024 | 703 ns | 1.9% | 0.96× [0.93–1.10] | 1.42 M | checksum=2.824e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.9 µs | 1.7% | baseline | 47.9 k | checksum=5.23e+09 |
| Each | 28.6 µs | 2.6% | **0.73× [0.65–0.78]** | 34.9 k | checksum=5.229e+09 |
| Grain1 | 217 µs | 1.3% | **0.10× [0.09–0.10]** | 4.6 k | checksum=5.227e+09 |
| Grain64 | 27 µs | 1.5% | **0.78× [0.76–0.82]** | 37.1 k | checksum=5.229e+09 |
| Grain1024 | 21.3 µs | 1.8% | 0.99× [0.97–1.04] | 46.9 k | checksum=5.23e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.9 µs | 2.0% | baseline | 47.9 k | checksum=5.23e+09 |
| Each | 28.7 µs | 2.7% | **0.72× [0.64–0.77]** | 34.8 k | checksum=5.229e+09 |
| Grain1 | 225 µs | 3.3% | **0.09× [0.09–0.10]** | 4.44 k | checksum=5.227e+09 |
| Grain64 | 28.9 µs | 7.7% | **0.75× [0.68–0.81]** | 34.6 k | checksum=5.229e+09 |
| Grain1024 | 21.6 µs | 1.7% | 0.98× [0.94–1.03] | 46.3 k | checksum=5.229e+09 |

