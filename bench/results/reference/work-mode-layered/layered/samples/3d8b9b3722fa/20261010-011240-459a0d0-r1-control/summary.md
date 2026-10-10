# Benchmark run 20261010-011240-459a0d0-r1-control

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.2% | baseline | 63.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.4% | 1.01× [0.50–1.02] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 15.5 µs | 0.3% | 1.01× [0.88–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 15.5 µs | 0.3% | 1.01× [0.19–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.5 µs | 0.2% | **1.01× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 15.5 µs | 0.2% | **1.02× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 15.5 µs | 0.2% | **1.02× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 15.5 µs | 0.2% | 1.01× [0.94–1.02] | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 15.5 µs | 0.2% | **1.02× [1.00–1.02]** | 64.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 15.5 µs | 0.2% | **1.02× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 15.5 µs | 0.4% | **1.01× [1.01–1.03]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.5 µs | 0.1% | **1.02× [1.01–1.02]** | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.3% | baseline | 62.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **1.03× [1.02–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **0.99× [0.99–0.99]** | 64.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.3% | baseline | 62.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.03× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.1% | **0.99× [0.99–0.99]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.3% | baseline | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.3% | **1.03× [1.03–1.04]** | 64.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.4% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.5% | 0.99× [0.98–1.18] | 64.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.42 ms | 5.2% | baseline | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.63 ms | 4.2% | 0.97× [0.85–1.10] | 216 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.61 ms | 5.1% | 0.99× [0.92–1.06] | 217 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.41 ms | 3.9% | 0.99× [0.89–1.12] | 227 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.57 ms | 4.6% | 0.99× [0.88–1.15] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.59 ms | 4.4% | 0.95× [0.90–1.19] | 218 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.37 ms | 3.5% | 1.03× [0.86–1.13] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.49 ms | 6.0% | 0.96× [0.87–1.12] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.13 ms | 1.4% | **1.08× [1.01–2.04]** | 242 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.26 ms | 3.9% | **1.96× [1.79–2.31]** | 443 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.39 ms | 4.9% | 0.99× [0.91–1.20] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.47 ms | 3.1% | 1.04× [0.86–1.12] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.59 ms | 1.0% | **2.78× [2.61–4.28]** | 629 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.14 ms | 2.2% | **2.02× [1.79–2.34]** | 468 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.35 ms | 4.8% | baseline | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.46 ms | 1.6% | 0.98× [0.94–1.02] | 224 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.55 ms | 5.4% | baseline | 220 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.53 ms | 4.5% | 0.99× [0.91–1.02] | 221 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 0.8% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.3 ms | 5.7% | 1.51× [0.89–1.89] | 435 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.19 ms | 4.4% | baseline | 457 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.14 ms | 3.0% | 1.01× [0.97–1.14] | 467 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.54 ms | 4.6% | baseline | 648 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.53 ms | 2.8% | 0.99× [0.93–1.09] | 654 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.12 ms | 1.1% | baseline | 473 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.11 ms | 1.6% | 0.99× [0.91–1.02] | 473 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.1 ms | 2.0% | baseline | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 72.4 ms | 2.3% | 0.97× [0.89–1.02] | 13.8 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 71.1 ms | 2.1% | 0.99× [0.93–1.02] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 70.5 ms | 2.4% | 1.00× [0.87–1.04] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.8 ms | 2.9% | 0.99× [0.90–1.05] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 70.8 ms | 1.5% | 1.00× [0.93–1.05] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36 ms | 1.2% | **1.97× [1.22–2.04]** | 27.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 34.9 ms | 2.5% | **2.06× [1.91–2.22]** | 28.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.4 ms | 2.8% | **2.00× [1.23–2.07]** | 28.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 35.3 ms | 1.5% | **2.01× [1.91–2.09]** | 28.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 18.4 ms | 3.1% | **3.78× [2.14–4.14]** | 54.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 19.3 ms | 8.9% | **3.62× [2.12–4.00]** | 51.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 18.2 ms | 4.5% | **3.82× [3.07–4.18]** | 54.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 18.6 ms | 5.8% | **3.73× [2.21–4.12]** | 53.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70 ms | 1.0% | baseline | 14.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.6 ms | 1.2% | 0.99× [0.99–1.01] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 70.3 ms | 0.9% | baseline | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 71.3 ms | 1.3% | 0.98× [0.97–1.03] | 14 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 35.7 ms | 4.4% | baseline | 28 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.4 ms | 1.8% | 1.01× [0.96–1.77] | 28.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 36.3 ms | 1.7% | baseline | 27.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 36.2 ms | 2.9% | 1.00× [0.97–1.04] | 27.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 17.7 ms | 2.3% | baseline | 56.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 18.3 ms | 4.9% | **0.97× [0.92–0.99]** | 54.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.8 ms | 5.0% | baseline | 53.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 19.3 ms | 5.6% | 0.96× [0.81–1.01] | 51.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.1 ns | 2.5% | baseline | 70.8 M | checksum=9.403e+07 |
| Each | 21 ns | 1.6% | **0.67× [0.65–0.74]** | 47.6 M | checksum=9.399e+07 |
| Grain1 | 138 ns | 1.3% | **0.10× [0.10–0.11]** | 7.22 M | checksum=9.281e+07 |
| Grain64 | 22.3 ns | 1.8% | **0.63× [0.60–0.67]** | 44.8 M | checksum=9.375e+07 |
| Grain1024 | 17.5 ns | 3.8% | **0.84× [0.75–0.86]** | 57.3 M | checksum=9.39e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.2 ns | 1.3% | baseline | 65.7 M | checksum=9.197e+07 |
| Each | 20.5 ns | 2.4% | **0.74× [0.72–0.77]** | 48.7 M | checksum=9.194e+07 |
| Grain1 | 141 ns | 1.7% | **0.11× [0.10–0.12]** | 7.12 M | checksum=9.076e+07 |
| Grain64 | 24 ns | 1.8% | **0.63× [0.61–0.66]** | 41.6 M | checksum=9.184e+07 |
| Grain1024 | 20.4 ns | 2.6% | **0.75× [0.65–0.78]** | 49 M | checksum=9.182e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 686 ns | 1.3% | baseline | 1.46 M | checksum=2.775e+08 |
| Each | 1.15 µs | 0.6% | **0.59× [0.57–0.62]** | 867 k | checksum=2.758e+08 |
| Grain1 | 8.81 µs | 1.3% | **0.08× [0.08–0.08]** | 113 k | checksum=2.735e+08 |
| Grain64 | 1.05 µs | 2.3% | **0.65× [0.56–0.67]** | 952 k | checksum=2.761e+08 |
| Grain1024 | 691 ns | 1.3% | 0.99× [0.95–1.01] | 1.45 M | checksum=2.775e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 724 ns | 0.6% | baseline | 1.38 M | checksum=2.648e+08 |
| Each | 1.17 µs | 0.7% | **0.62× [0.61–0.63]** | 857 k | checksum=2.636e+08 |
| Grain1 | 8.66 µs | 0.6% | **0.08× [0.08–0.08]** | 115 k | checksum=2.613e+08 |
| Grain64 | 1.04 µs | 0.2% | **0.70× [0.68–0.70]** | 965 k | checksum=2.639e+08 |
| Grain1024 | 686 ns | 0.8% | **1.05× [1.02–1.07]** | 1.46 M | checksum=2.652e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.9 µs | 1.1% | baseline | 47.9 k | checksum=5.234e+09 |
| Each | 28.9 µs | 2.1% | **0.73× [0.71–0.75]** | 34.6 k | checksum=5.234e+09 |
| Grain1 | 216 µs | 1.7% | **0.10× [0.09–0.10]** | 4.62 k | checksum=5.231e+09 |
| Grain64 | 26.6 µs | 0.7% | **0.79× [0.78–0.80]** | 37.6 k | checksum=5.233e+09 |
| Grain1024 | 21.3 µs | 2.1% | 0.99× [0.92–1.02] | 47 k | checksum=5.235e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.8 µs | 1.6% | baseline | 48.1 k | checksum=5.235e+09 |
| Each | 28.5 µs | 1.4% | **0.73× [0.70–0.74]** | 35.1 k | checksum=5.234e+09 |
| Grain1 | 216 µs | 1.1% | **0.10× [0.09–0.10]** | 4.63 k | checksum=5.231e+09 |
| Grain64 | 27 µs | 1.0% | **0.77× [0.75–0.79]** | 37 k | checksum=5.234e+09 |
| Grain1024 | 21.5 µs | 1.8% | **0.97× [0.90–1.00]** | 46.5 k | checksum=5.234e+09 |

