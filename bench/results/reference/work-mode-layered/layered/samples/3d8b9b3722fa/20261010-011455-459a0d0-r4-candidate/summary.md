# Benchmark run 20261010-011455-459a0d0-r4-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.2% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.4% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.2 µs | 0.2% | **0.97× [0.96–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.3 µs | 0.3% | **0.97× [0.95–0.98]** | 61.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.2 µs | 0.9% | **0.97× [0.96–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.1 µs | 0.5% | **0.97× [0.94–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.2 µs | 0.3% | **0.97× [0.96–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 1.1% | **0.97× [0.95–0.98]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.5% | **0.97× [0.96–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.3 µs | 0.5% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.3 µs | 0.3% | **0.97× [0.96–0.99]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.4% | **0.97× [0.96–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.5% | **0.97× [0.81–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.6% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.7% | **0.99× [0.55–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.6% | **0.95× [0.94–0.95]** | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.5% | baseline | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | 1.01× [1.00–1.03] | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.94× [0.94–0.95]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.6% | 1.00× [0.99–1.00] | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.4% | **0.94× [0.94–0.95]** | 61.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.44 ms | 3.5% | baseline | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.53 ms | 4.7% | 1.00× [0.93–1.25] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.22 ms | 3.9% | 1.07× [0.97–1.40] | 237 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.33 ms | 6.7% | 1.05× [0.97–1.45] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.31 ms | 6.1% | 1.07× [1.00–1.39] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.49 ms | 4.0% | 0.99× [0.89–1.36] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.57 ms | 5.0% | 1.04× [0.92–1.19] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.43 ms | 5.8% | 1.02× [0.93–1.41] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.24 ms | 5.1% | **2.08× [1.82–2.36]** | 446 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.45 ms | 12.7% | **1.77× [1.09–2.24]** | 408 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.38 ms | 4.6% | 1.07× [0.93–1.37] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.41 ms | 4.6% | 1.02× [0.95–1.24] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.59 ms | 3.1% | **2.78× [2.38–3.26]** | 628 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.97 ms | 16.3% | **1.73× [1.37–2.09]** | 337 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.37 ms | 7.6% | baseline | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.38 ms | 2.5% | 1.05× [0.98–1.08] | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.37 ms | 6.5% | baseline | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.32 ms | 5.2% | 1.02× [0.97–1.12] | 231 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.21 ms | 3.6% | baseline | 452 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.21 ms | 5.5% | 0.98× [0.95–1.08] | 453 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.43 ms | 7.5% | baseline | 412 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.4 ms | 4.7% | 0.98× [0.94–1.08] | 417 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.05 ms | 1.2% | baseline | 487 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.45 ms | 17.1% | **0.75× [0.52–0.90]** | 408 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.09 ms | 1.9% | baseline | 479 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.11 ms | 2.0% | 0.98× [0.95–1.07] | 473 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.6 ms | 2.0% | baseline | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 69 ms | 2.2% | 1.00× [0.94–1.08] | 14.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 69.1 ms | 3.1% | 1.01× [0.98–1.07] | 14.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 70.1 ms | 2.4% | 1.00× [0.92–1.06] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.4 ms | 2.1% | 0.99× [0.96–1.06] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 69.6 ms | 2.0% | 1.01× [0.96–1.06] | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 39.2 ms | 10.6% | **1.79× [1.05–2.02]** | 25.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 35.5 ms | 2.8% | **1.99× [1.69–2.08]** | 28.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 36.2 ms | 4.2% | **1.94× [1.69–2.06]** | 27.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 34.9 ms | 3.5% | **1.99× [1.87–2.13]** | 28.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 32.3 ms | 0.9% | **2.37× [2.11–3.73]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 32.4 ms | 1.2% | **2.19× [2.09–3.30]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 24.3 ms | 25.6% | **2.85× [2.06–3.91]** | 41.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 0.9% | **2.44× [2.10–3.74]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.4 ms | 2.2% | baseline | 14.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.1 ms | 1.5% | 1.01× [0.99–1.05] | 14.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 67.3 ms | 1.5% | baseline | 14.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 68.5 ms | 0.8% | **0.98× [0.96–1.00]** | 14.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 37.4 ms | 7.0% | baseline | 26.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.4 ms | 4.2% | 1.02× [0.97–1.35] | 27.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 39.6 ms | 12.0% | baseline | 25.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.8 ms | 3.7% | 1.11× [0.95–1.21] | 27.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 26.8 ms | 16.2% | baseline | 37.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 23.9 ms | 16.8% | 1.03× [0.99–1.23] | 41.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 32.8 ms | 0.7% | baseline | 30.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 32.4 ms | 1.4% | 1.01× [0.99–1.15] | 30.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.1 ns | 2.6% | baseline | 70.9 M | checksum=9.402e+07 |
| Each | 16 ns | 3.6% | **0.90× [0.72–0.95]** | 62.5 M | checksum=9.398e+07 |
| Grain1 | 22 ns | 5.7% | **0.64× [0.57–0.69]** | 45.4 M | checksum=9.386e+07 |
| Grain64 | 18.1 ns | 2.7% | **0.78× [0.75–0.88]** | 55.4 M | checksum=9.398e+07 |
| Grain1024 | 20.7 ns | 3.8% | **0.68× [0.63–0.72]** | 48.3 M | checksum=9.4e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 ns | 3.5% | baseline | 60.9 M | checksum=9.243e+07 |
| Each | 19.2 ns | 4.0% | **0.84× [0.77–0.95]** | 52.1 M | checksum=9.221e+07 |
| Grain1 | 148 ns | 4.3% | **0.12× [0.10–0.13]** | 6.78 M | checksum=9.123e+07 |
| Grain64 | 21.7 ns | 2.4% | **0.76× [0.70–0.83]** | 46.1 M | checksum=9.219e+07 |
| Grain1024 | 21.2 ns | 1.3% | **0.78× [0.71–0.87]** | 47.2 M | checksum=9.228e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 702 ns | 2.8% | baseline | 1.42 M | checksum=2.903e+08 |
| Each | 664 ns | 2.3% | 1.05× [0.98–1.12] | 1.51 M | checksum=2.942e+08 |
| Grain1 | 1.21 µs | 2.3% | **0.59× [0.55–0.63]** | 825 k | checksum=2.922e+08 |
| Grain64 | 785 ns | 3.7% | **0.89× [0.80–0.97]** | 1.27 M | checksum=2.929e+08 |
| Grain1024 | 1.22 µs | 3.4% | **0.58× [0.57–0.60]** | 820 k | checksum=2.919e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 750 ns | 4.3% | baseline | 1.33 M | checksum=2.742e+08 |
| Each | 699 ns | 2.3% | 1.04× [0.96–1.22] | 1.43 M | checksum=2.74e+08 |
| Grain1 | 9.39 µs | 3.6% | **0.08× [0.07–0.09]** | 106 k | checksum=2.702e+08 |
| Grain64 | 1.07 µs | 3.3% | **0.70× [0.59–0.75]** | 939 k | checksum=2.715e+08 |
| Grain1024 | 1.27 µs | 4.9% | **0.58× [0.56–0.62]** | 788 k | checksum=2.724e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.9 µs | 4.0% | baseline | 45.6 k | checksum=5.235e+09 |
| Each | 21.9 µs | 4.6% | 0.99× [0.83–1.11] | 45.6 k | checksum=5.235e+09 |
| Grain1 | 29.2 µs | 4.3% | **0.75× [0.66–0.81]** | 34.3 k | checksum=5.233e+09 |
| Grain64 | 22.5 µs | 3.8% | 0.99× [0.95–1.04] | 44.4 k | checksum=5.234e+09 |
| Grain1024 | 33.1 µs | 12.6% | **0.69× [0.58–0.74]** | 30.2 k | checksum=5.233e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 22.5 µs | 4.6% | baseline | 44.4 k | checksum=5.229e+09 |
| Each | 22.5 µs | 7.3% | 1.01× [0.89–1.10] | 44.5 k | checksum=5.23e+09 |
| Grain1 | 229 µs | 2.0% | **0.10× [0.09–0.11]** | 4.36 k | checksum=5.226e+09 |
| Grain64 | 29.9 µs | 5.1% | **0.75× [0.71–0.83]** | 33.5 k | checksum=5.228e+09 |
| Grain1024 | 33 µs | 4.4% | **0.67× [0.64–0.79]** | 30.3 k | checksum=5.229e+09 |

