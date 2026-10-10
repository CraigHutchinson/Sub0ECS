# Benchmark run 20261010-011323-459a0d0-r2-candidate

- rows: ok in 1 s
- nbody: ok in 23 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.3% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.01× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.7% | **0.97× [0.95–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.1 µs | 0.5% | **0.97× [0.97–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.5% | **0.97× [0.97–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.2 µs | 0.5% | **0.97× [0.97–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.2 µs | 0.3% | **0.97× [0.94–0.99]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.2 µs | 0.4% | **0.97× [0.96–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.2 µs | 0.6% | **0.97× [0.96–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.3% | **0.97× [0.96–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.1 µs | 0.7% | **0.98× [0.96–0.98]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 0.7% | **0.97× [0.96–0.99]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.5% | **0.97× [0.96–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.5% | **0.97× [0.96–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.5% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.7% | 1.00× [0.99–1.01] | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.2% | **0.94× [0.94–0.95]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.6% | **0.99× [0.98–1.00]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.3% | **0.95× [0.95–0.95]** | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.4% | baseline | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.5% | 1.00× [1.00–1.01] | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.38 ms | 3.9% | baseline | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.34 ms | 5.0% | 1.02× [0.67–1.15] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.16 ms | 2.2% | 1.04× [0.98–1.11] | 241 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.34 ms | 4.3% | 1.03× [0.93–1.16] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.4 ms | 3.1% | 1.01× [0.93–1.06] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.27 ms | 4.2% | 1.01× [0.62–1.12] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.4 ms | 7.6% | 1.00× [0.47–1.11] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.26 ms | 5.0% | 1.06× [0.80–1.11] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.35 ms | 12.3% | 1.88× [0.76–2.16] | 426 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.38 ms | 9.4% | **1.90× [1.20–2.16]** | 420 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.27 ms | 2.6% | 1.01× [0.95–1.07] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.34 ms | 4.5% | 1.01× [0.75–1.06] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.58 ms | 4.1% | **2.74× [1.04–3.59]** | 633 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.34 ms | 12.6% | **1.93× [1.25–2.20]** | 428 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.1 ms | 1.8% | baseline | 244 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.21 ms | 3.7% | 0.99× [0.93–1.03] | 237 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 0.5% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.05 ms | 0.8% | **1.00× [0.97–1.00]** | 247 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.05 ms | 0.3% | baseline | 247 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.27 ms | 6.2% | **1.75× [1.57–1.90]** | 440 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.35 ms | 5.1% | baseline | 425 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.27 ms | 4.1% | 1.03× [0.99–1.07] | 441 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.59 ms | 4.3% | baseline | 630 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.57 ms | 2.2% | 0.98× [0.83–1.06] | 637 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.11 ms | 0.7% | baseline | 474 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.09 ms | 1.0% | 1.00× [0.94–1.03] | 478 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 75.3 ms | 4.6% | baseline | 13.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 73.1 ms | 4.1% | 1.04× [0.88–1.14] | 13.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 76.7 ms | 5.9% | 1.00× [0.90–1.06] | 13 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 79.9 ms | 12.7% | 0.98× [0.79–1.07] | 12.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 74 ms | 4.9% | 1.02× [0.75–1.08] | 13.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 75.1 ms | 5.4% | 1.03× [0.84–1.15] | 13.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 44.3 ms | 15.6% | **1.62× [1.11–2.04]** | 22.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 50.3 ms | 23.0% | **1.45× [1.12–2.06]** | 19.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 39.7 ms | 10.3% | **1.90× [1.34–2.14]** | 25.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 42.3 ms | 15.0% | **1.84× [1.31–2.17]** | 23.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 31.2 ms | 15.6% | **2.73× [1.98–3.96]** | 32 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.5 ms | 10.7% | **2.35× [1.54–3.48]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 28.6 ms | 14.8% | **2.71× [2.18–4.09]** | 34.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 25.9 ms | 23.5% | **3.04× [1.75–3.85]** | 38.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 86.9 ms | 7.0% | baseline | 11.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 85 ms | 11.0% | 1.00× [0.91–1.07] | 11.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 73.2 ms | 9.1% | baseline | 13.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 76.6 ms | 4.7% | 0.95× [0.89–1.39] | 13.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 64.3 ms | 3.4% | baseline | 15.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 53.2 ms | 18.1% | 1.03× [0.93–1.31] | 18.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 41.8 ms | 11.5% | baseline | 23.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 40.8 ms | 14.9% | 0.98× [0.73–1.06] | 24.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 25.1 ms | 4.6% | baseline | 39.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 24.2 ms | 6.2% | 1.03× [0.93–1.17] | 41.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32.8 ms | 5.1% | baseline | 30.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.4 ms | 12.5% | 0.99× [0.83–1.29] | 30.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.3 ns | 5.0% | baseline | 69.9 M | checksum=9.312e+07 |
| Each | 16.8 ns | 5.0% | **0.88× [0.75–0.98]** | 59.7 M | checksum=9.32e+07 |
| Grain1 | 23.4 ns | 10.2% | **0.65× [0.51–0.67]** | 42.8 M | checksum=9.315e+07 |
| Grain64 | 18.4 ns | 2.7% | **0.76× [0.71–0.85]** | 54.4 M | checksum=9.322e+07 |
| Grain1024 | 21.7 ns | 7.5% | **0.66× [0.60–0.72]** | 46.1 M | checksum=9.311e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.5 ns | 5.2% | baseline | 64.3 M | checksum=9.242e+07 |
| Each | 19.7 ns | 4.8% | **0.80× [0.68–0.97]** | 50.7 M | checksum=9.225e+07 |
| Grain1 | 150 ns | 7.1% | **0.10× [0.10–0.12]** | 6.69 M | checksum=9.122e+07 |
| Grain64 | 22.9 ns | 4.2% | **0.71× [0.59–0.82]** | 43.8 M | checksum=9.136e+07 |
| Grain1024 | 22.5 ns | 3.6% | **0.71× [0.64–0.82]** | 44.4 M | checksum=9.22e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 757 ns | 8.3% | baseline | 1.32 M | checksum=2.747e+08 |
| Each | 684 ns | 4.9% | 1.08× [0.96–1.24] | 1.46 M | checksum=2.715e+08 |
| Grain1 | 1.23 µs | 6.6% | **0.64× [0.42–0.70]** | 816 k | checksum=2.73e+08 |
| Grain64 | 766 ns | 2.0% | 0.95× [0.89–1.21] | 1.31 M | checksum=2.715e+08 |
| Grain1024 | 1.22 µs | 4.3% | **0.59× [0.53–0.67]** | 822 k | checksum=2.73e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 691 ns | 3.4% | baseline | 1.45 M | checksum=2.496e+08 |
| Each | 706 ns | 4.7% | 1.00× [0.97–1.14] | 1.42 M | checksum=2.488e+08 |
| Grain1 | 9.09 µs | 2.5% | **0.08× [0.07–0.08]** | 110 k | checksum=2.46e+08 |
| Grain64 | 1.08 µs | 1.8% | **0.64× [0.62–0.81]** | 929 k | checksum=2.486e+08 |
| Grain1024 | 1.24 µs | 3.0% | **0.56× [0.53–0.67]** | 808 k | checksum=2.48e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.2 µs | 3.3% | baseline | 47.2 k | checksum=5.182e+09 |
| Each | 20.8 µs | 1.7% | 1.03× [0.99–1.14] | 48.2 k | checksum=5.184e+09 |
| Grain1 | 29.4 µs | 4.3% | **0.72× [0.66–0.76]** | 34 k | checksum=5.184e+09 |
| Grain64 | 21.3 µs | 2.4% | 1.02× [0.95–1.08] | 47 k | checksum=5.185e+09 |
| Grain1024 | 30.1 µs | 2.8% | **0.72× [0.66–0.79]** | 33.2 k | checksum=5.184e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.4 µs | 5.5% | baseline | 46.7 k | checksum=5.233e+09 |
| Each | 22.1 µs | 5.9% | 0.98× [0.87–1.15] | 45.2 k | checksum=5.235e+09 |
| Grain1 | 225 µs | 3.1% | **0.10× [0.07–0.12]** | 4.45 k | checksum=5.231e+09 |
| Grain64 | 27.9 µs | 1.4% | **0.77× [0.76–0.91]** | 35.9 k | checksum=5.234e+09 |
| Grain1024 | 30 µs | 4.3% | **0.71× [0.64–0.79]** | 33.4 k | checksum=5.233e+09 |

