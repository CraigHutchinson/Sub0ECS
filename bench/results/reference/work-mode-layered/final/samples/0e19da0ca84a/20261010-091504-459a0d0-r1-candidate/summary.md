# Benchmark run 20261010-091504-459a0d0-r1-candidate

- rows: ok in 1 s
- nbody: ok in 22 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.3% | baseline | 63.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | **1.02× [1.01–1.02]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.2 µs | 0.6% | 0.98× [0.94–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.2 µs | 0.6% | 0.98× [0.97–1.01] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.9% | 0.98× [0.82–1.00] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.2 µs | 0.8% | 0.98× [0.58–1.01] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.1 µs | 0.6% | 0.98× [0.97–1.01] | 62.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.1 µs | 1.0% | 0.98× [0.96–1.01] | 62.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.1 µs | 0.9% | 0.98× [0.97–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.6% | 0.98× [0.97–1.00] | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.9% | **0.98× [0.96–0.99]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.7% | 0.98× [0.96–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.9% | 0.98× [0.96–1.02] | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.9% | 0.98× [0.96–1.02] | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.8% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.7% | 1.00× [0.98–1.00] | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.4% | **0.95× [0.95–0.96]** | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.4% | baseline | 62.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16 µs | 1.0% | 0.99× [0.98–1.00] | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16 µs | 1.0% | **0.96× [0.94–0.97]** | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.5% | baseline | 62.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.9% | 1.00× [0.98–1.01] | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.3% | baseline | 65 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.6% | **0.95× [0.94–0.96]** | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.67 ms | 8.1% | baseline | 214 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.72 ms | 6.7% | 0.98× [0.86–1.24] | 212 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.74 ms | 12.1% | 1.04× [0.83–1.24] | 211 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.69 ms | 4.4% | 0.99× [0.82–1.21] | 213 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.55 ms | 7.1% | 1.02× [0.89–1.18] | 220 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.76 ms | 7.3% | 1.00× [0.70–1.16] | 210 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.71 ms | 3.2% | 1.00× [0.82–1.19] | 213 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.75 ms | 9.8% | 0.98× [0.79–1.20] | 210 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.43 ms | 8.7% | **1.99× [1.18–2.37]** | 411 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 3.51 ms | 20.2% | **1.72× [1.05–2.28]** | 285 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.43 ms | 3.5% | 1.05× [0.94–1.65] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.4 ms | 2.7% | 1.01× [0.93–1.16] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.81 ms | 12.6% | **2.62× [1.96–4.99]** | 554 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.2 ms | 4.3% | **2.07× [1.86–3.68]** | 455 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.39 ms | 3.7% | baseline | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.63 ms | 4.9% | 0.95× [0.87–1.07] | 216 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.42 ms | 4.7% | baseline | 226 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.74 ms | 5.0% | **0.91× [0.85–0.99]** | 211 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.37 ms | 7.2% | baseline | 421 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.36 ms | 4.7% | 1.04× [0.98–1.07] | 423 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.36 ms | 9.6% | baseline | 423 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.71 ms | 13.1% | 0.92× [0.72–1.34] | 369 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.96 ms | 7.8% | baseline | 511 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.6 ms | 3.3% | 1.01× [0.99–1.16] | 625 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.11 ms | 2.0% | baseline | 473 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.07 ms | 1.4% | **1.01× [1.00–1.22]** | 484 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.8 ms | 3.0% | baseline | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.6 ms | 2.8% | 1.00× [0.92–1.03] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 73.4 ms | 4.5% | 0.98× [0.89–1.05] | 13.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 75.1 ms | 4.0% | 0.95× [0.87–1.05] | 13.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 75.1 ms | 4.2% | 0.98× [0.90–1.06] | 13.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 74 ms | 6.1% | 0.97× [0.73–1.06] | 13.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 49.7 ms | 24.1% | **1.47× [1.08–1.99]** | 20.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 38.3 ms | 10.4% | **1.90× [1.20–2.06]** | 26.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 53.4 ms | 18.2% | **1.34× [1.11–1.99]** | 18.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 38.3 ms | 5.5% | **1.84× [1.32–2.03]** | 26.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.6 ms | 0.8% | **2.20× [1.92–3.69]** | 30.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 0.9% | **2.29× [1.99–3.75]** | 30.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 23.8 ms | 7.4% | **2.98× [2.02–3.32]** | 42 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.5 ms | 1.7% | **2.16× [1.96–2.50]** | 30.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 77.2 ms | 3.0% | baseline | 12.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 76.5 ms | 4.6% | 0.99× [0.86–1.02] | 13.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.9 ms | 1.9% | baseline | 14.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.8 ms | 1.3% | 0.99× [0.97–1.02] | 14.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.2 ms | 1.3% | baseline | 29.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.5 ms | 4.8% | **0.94× [0.62–0.98]** | 27.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 38.9 ms | 9.6% | baseline | 25.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 37.7 ms | 6.8% | 0.99× [0.85–1.35] | 26.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.8 ms | 12.0% | baseline | 50.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 23.2 ms | 9.1% | 0.98× [0.85–1.06] | 43.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.7 ms | 9.9% | baseline | 48.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 19.7 ms | 7.7% | 1.03× [1.00–1.51] | 50.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.4 ns | 6.6% | baseline | 69.2 M | checksum=9.182e+07 |
| Each | 17.6 ns | 5.4% | 0.82× [0.76–1.20] | 56.7 M | checksum=9.242e+07 |
| Grain1 | 18.7 ns | 7.4% | 0.78× [0.71–1.08] | 53.4 M | checksum=9.243e+07 |
| Grain64 | 24 ns | 4.7% | **0.62× [0.54–0.82]** | 41.6 M | checksum=9.239e+07 |
| Grain1024 | 26.6 ns | 21.0% | **0.60× [0.41–0.67]** | 37.6 M | checksum=9.224e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 19.7 ns | 5.0% | baseline | 50.8 M | checksum=8.94e+07 |
| Each | 19.3 ns | 4.8% | 1.00× [0.96–1.17] | 51.7 M | checksum=8.938e+07 |
| Grain1 | 154 ns | 4.2% | **0.13× [0.12–0.16]** | 6.48 M | checksum=8.823e+07 |
| Grain64 | 28.6 ns | 9.9% | **0.73× [0.61–0.83]** | 35 M | checksum=8.927e+07 |
| Grain1024 | 26.3 ns | 5.0% | **0.74× [0.59–0.83]** | 38.1 M | checksum=8.929e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 747 ns | 8.6% | baseline | 1.34 M | checksum=2.919e+08 |
| Each | 683 ns | 4.0% | 1.04× [0.93–1.30] | 1.46 M | checksum=2.919e+08 |
| Grain1 | 663 ns | 1.5% | 1.06× [0.99–1.18] | 1.51 M | checksum=2.922e+08 |
| Grain64 | 792 ns | 7.0% | 0.98× [0.85–1.05] | 1.26 M | checksum=2.907e+08 |
| Grain1024 | 696 ns | 5.0% | 1.02× [0.87–1.23] | 1.44 M | checksum=2.921e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 723 ns | 7.3% | baseline | 1.38 M | checksum=2.822e+08 |
| Each | 738 ns | 7.1% | 1.03× [0.88–1.21] | 1.36 M | checksum=2.826e+08 |
| Grain1 | 10.2 µs | 3.5% | **0.07× [0.07–0.08]** | 98 k | checksum=2.785e+08 |
| Grain64 | 1.08 µs | 4.2% | **0.68× [0.60–0.74]** | 923 k | checksum=2.811e+08 |
| Grain1024 | 753 ns | 4.3% | 0.93× [0.83–1.17] | 1.33 M | checksum=2.823e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.9 µs | 8.7% | baseline | 43.7 k | checksum=5.235e+09 |
| Each | 23.4 µs | 9.2% | 1.02× [0.77–1.19] | 42.7 k | checksum=5.234e+09 |
| Grain1 | 22.1 µs | 6.2% | 1.03× [0.95–1.10] | 45.3 k | checksum=5.234e+09 |
| Grain64 | 21.8 µs | 5.2% | 1.02× [0.89–1.18] | 45.8 k | checksum=5.234e+09 |
| Grain1024 | 22.7 µs | 8.6% | 1.02× [0.72–1.17] | 44 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.8 µs | 5.8% | baseline | 43.9 k | checksum=5.234e+09 |
| Each | 22.7 µs | 9.5% | 1.06× [0.87–1.11] | 44 k | checksum=5.235e+09 |
| Grain1 | 243 µs | 7.1% | **0.10× [0.08–0.10]** | 4.12 k | checksum=5.231e+09 |
| Grain64 | 30.5 µs | 7.3% | **0.76× [0.67–0.82]** | 32.8 k | checksum=5.234e+09 |
| Grain1024 | 22.3 µs | 3.6% | 1.01× [0.95–1.12] | 44.8 k | checksum=5.234e+09 |

