# Benchmark run 20261010-011302-459a0d0-r1-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.2% | baseline | 63.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | 1.01× [0.99–1.02] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.1 µs | 0.6% | **0.97× [0.58–0.98]** | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.1 µs | 0.5% | **0.97× [0.97–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.5% | **0.97× [0.96–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.6% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.1 µs | 0.6% | **0.97× [0.97–0.99]** | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.2 µs | 0.5% | **0.97× [0.96–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.2 µs | 0.2% | **0.97× [0.48–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.3% | **0.97× [0.84–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.5% | **0.97× [0.96–0.98]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.2% | **0.97× [0.97–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.6% | **0.97× [0.41–0.98]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.8% | **0.97× [0.96–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.5% | baseline | 62.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | 0.99× [0.98–1.00] | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.6% | **0.95× [0.94–0.95]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.7% | 1.00× [0.99–1.00] | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.4% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.96]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.2% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | 0.99× [0.99–1.00] | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.1% | baseline | 64.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.2% | **0.95× [0.94–0.95]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.15 ms | 1.1% | baseline | 241 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.39 ms | 4.9% | 0.95× [0.90–1.03] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.32 ms | 6.9% | 1.01× [0.94–1.04] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.24 ms | 3.2% | 0.99× [0.93–1.06] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.19 ms | 3.9% | 1.00× [0.93–1.09] | 239 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.05 ms | 0.6% | 1.02× [0.93–1.10] | 247 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.13 ms | 2.5% | 1.02× [0.91–1.09] | 242 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.08 ms | 1.1% | 1.02× [0.96–1.11] | 245 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.18 ms | 5.3% | **1.91× [1.79–2.09]** | 459 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.2 ms | 2.3% | **1.92× [1.73–2.02]** | 455 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.25 ms | 3.3% | 1.00× [0.93–1.08] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.18 ms | 3.2% | 1.03× [0.93–1.05] | 239 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.56 ms | 1.7% | **2.68× [2.16–3.38]** | 639 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.13 ms | 2.6% | **1.95× [1.44–2.13]** | 470 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 0.2% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.05 ms | 0.5% | **1.00× [0.96–1.00]** | 247 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 0.4% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.11 ms | 1.9% | 0.98× [0.94–1.00] | 243 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.08 ms | 2.2% | baseline | 480 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.18 ms | 3.8% | **0.94× [0.92–0.99]** | 458 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.33 ms | 5.4% | baseline | 428 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.34 ms | 10.2% | 0.98× [0.62–1.06] | 427 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.22 ms | 4.4% | baseline | 821 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.2 ms | 8.4% | 1.01× [0.84–1.17] | 834 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.14 ms | 2.3% | baseline | 468 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.08 ms | 0.5% | 1.04× [1.00–1.06] | 481 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.4 ms | 2.3% | baseline | 14.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.5 ms | 2.5% | 0.99× [0.97–1.01] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 70.3 ms | 1.1% | 0.98× [0.95–1.02] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 70.6 ms | 2.4% | 0.98× [0.92–1.03] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 69.1 ms | 2.1% | 0.99× [0.94–1.04] | 14.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 68.7 ms | 1.1% | 1.01× [0.96–1.04] | 14.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 35.6 ms | 2.9% | **1.93× [1.76–2.09]** | 28.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 38.1 ms | 8.5% | **1.86× [1.24–2.05]** | 26.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 35.3 ms | 1.0% | **1.97× [1.81–2.05]** | 28.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 37.5 ms | 9.1% | **1.89× [1.08–2.07]** | 26.7 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 18.5 ms | 7.4% | **3.72× [2.17–4.00]** | 53.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 18.5 ms | 5.6% | **3.78× [3.37–4.03]** | 54 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 18.3 ms | 5.0% | **3.72× [2.96–4.09]** | 54.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 19.1 ms | 8.9% | **3.65× [2.19–4.03]** | 52.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 80 ms | 6.0% | baseline | 12.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 83.9 ms | 15.4% | 0.97× [0.76–1.05] | 11.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.1 ms | 1.5% | baseline | 14.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 70.6 ms | 1.1% | 0.99× [0.98–1.00] | 14.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 36.1 ms | 4.2% | baseline | 27.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 44.1 ms | 22.4% | 0.91× [0.67–1.02] | 22.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 36.5 ms | 4.5% | baseline | 27.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.7 ms | 1.3% | 1.01× [0.96–1.04] | 28 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 18.5 ms | 6.5% | baseline | 54 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 22.8 ms | 6.7% | **0.91× [0.82–0.99]** | 43.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 29.8 ms | 11.0% | baseline | 33.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 24.8 ms | 29.1% | **1.02× [1.00–1.44]** | 40.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.2 ns | 2.5% | baseline | 70.2 M | checksum=9.402e+07 |
| Each | 16 ns | 4.4% | **0.90× [0.84–0.93]** | 62.6 M | checksum=9.398e+07 |
| Grain1 | 20.5 ns | 1.6% | **0.69× [0.66–0.73]** | 48.9 M | checksum=9.384e+07 |
| Grain64 | 17.7 ns | 2.4% | **0.80× [0.78–0.84]** | 56.4 M | checksum=9.392e+07 |
| Grain1024 | 21.5 ns | 2.9% | **0.66× [0.63–0.70]** | 46.5 M | checksum=9.398e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.2 ns | 2.6% | baseline | 65.8 M | checksum=8.787e+07 |
| Each | 18.7 ns | 1.2% | **0.81× [0.76–0.95]** | 53.5 M | checksum=8.793e+07 |
| Grain1 | 146 ns | 3.1% | **0.10× [0.10–0.12]** | 6.85 M | checksum=8.681e+07 |
| Grain64 | 21.4 ns | 2.6% | **0.71× [0.65–0.76]** | 46.7 M | checksum=8.784e+07 |
| Grain1024 | 20.8 ns | 2.1% | **0.72× [0.69–0.82]** | 48 M | checksum=8.778e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 723 ns | 5.4% | baseline | 1.38 M | checksum=2.941e+08 |
| Each | 663 ns | 2.0% | 1.06× [0.95–1.21] | 1.51 M | checksum=2.944e+08 |
| Grain1 | 1.15 µs | 0.7% | **0.63× [0.56–0.71]** | 869 k | checksum=2.922e+08 |
| Grain64 | 775 ns | 2.8% | 0.91× [0.84–1.06] | 1.29 M | checksum=2.939e+08 |
| Grain1024 | 1.22 µs | 3.0% | **0.59× [0.54–0.67]** | 817 k | checksum=2.923e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 682 ns | 1.8% | baseline | 1.47 M | checksum=2.833e+08 |
| Each | 674 ns | 0.7% | 1.00× [0.99–1.06] | 1.48 M | checksum=2.826e+08 |
| Grain1 | 8.98 µs | 2.2% | **0.08× [0.07–0.08]** | 111 k | checksum=2.792e+08 |
| Grain64 | 1.03 µs | 1.3% | **0.66× [0.63–0.71]** | 968 k | checksum=2.818e+08 |
| Grain1024 | 1.2 µs | 2.0% | **0.57× [0.54–0.60]** | 834 k | checksum=2.815e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.6 µs | 4.6% | baseline | 46.4 k | checksum=5.235e+09 |
| Each | 22.1 µs | 7.5% | 1.04× [0.89–1.15] | 45.2 k | checksum=5.235e+09 |
| Grain1 | 29.1 µs | 2.9% | **0.76× [0.71–0.86]** | 34.4 k | checksum=5.233e+09 |
| Grain64 | 22 µs | 3.0% | 1.03× [0.92–1.19] | 45.5 k | checksum=5.235e+09 |
| Grain1024 | 29.9 µs | 4.0% | **0.74× [0.70–0.89]** | 33.5 k | checksum=5.233e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 20.5 µs | 1.2% | baseline | 48.8 k | checksum=5.235e+09 |
| Each | 20.7 µs | 1.7% | 1.00× [0.96–1.11] | 48.3 k | checksum=5.233e+09 |
| Grain1 | 219 µs | 1.1% | **0.09× [0.09–0.11]** | 4.57 k | checksum=5.231e+09 |
| Grain64 | 27 µs | 1.1% | **0.76× [0.73–0.90]** | 37.1 k | checksum=5.233e+09 |
| Grain1024 | 29.2 µs | 1.6% | **0.70× [0.69–0.76]** | 34.2 k | checksum=5.233e+09 |

