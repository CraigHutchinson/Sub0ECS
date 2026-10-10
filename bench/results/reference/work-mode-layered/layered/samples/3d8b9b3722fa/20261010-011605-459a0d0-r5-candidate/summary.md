# Benchmark run 20261010-011605-459a0d0-r5-candidate

- rows: ok in 1 s
- nbody: ok in 20 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.3% | baseline | 63.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.1% | **1.02× [1.01–1.03]** | 64.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.8% | **0.98× [0.96–1.00]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.1 µs | 0.7% | **0.99× [0.97–1.00]** | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 1.0% | **0.98× [0.96–1.00]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.1 µs | 0.6% | 0.98× [0.97–1.02] | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.1 µs | 0.4% | 0.98× [0.97–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.2 µs | 0.5% | 0.98× [0.97–1.00] | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.2 µs | 0.6% | 0.98× [0.97–1.01] | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.1 µs | 0.4% | 0.98× [0.97–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 1.0% | 0.98× [0.82–1.02] | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.7% | 0.99× [0.97–1.03] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.6% | 0.98× [0.97–1.01] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.6% | 0.98× [0.97–1.02] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.7% | baseline | 62.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.7% | 0.99× [0.99–1.83] | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.1% | baseline | 65.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 1.0% | **0.95× [0.94–0.96]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.4% | baseline | 63 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.6% | **0.99× [0.98–0.99]** | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 1.1% | **0.94× [0.93–0.96]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 1.4% | baseline | 62.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.5% | 0.99× [0.97–1.01] | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.1 µs | 0.2% | **0.95× [0.95–0.95]** | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.33 ms | 4.5% | baseline | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.6 ms | 6.5% | 0.95× [0.68–1.10] | 217 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.39 ms | 2.7% | 0.98× [0.91–1.10] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.42 ms | 4.5% | 1.01× [0.93–1.09] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.49 ms | 4.4% | 0.97× [0.88–1.09] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.52 ms | 5.3% | 0.96× [0.60–1.15] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.32 ms | 4.7% | 1.00× [0.85–1.22] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.52 ms | 4.8% | 0.95× [0.60–1.17] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 3.54 ms | 17.6% | 1.28× [0.99–2.32] | 282 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.34 ms | 5.5% | **1.94× [1.61–2.11]** | 427 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.32 ms | 4.7% | 0.99× [0.91–1.14] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.47 ms | 5.2% | 1.01× [0.57–1.22] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 2.34 ms | 25.6% | **1.89× [1.20–2.75]** | 428 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.23 ms | 6.5% | **1.96× [1.04–2.22]** | 449 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.6 ms | 4.1% | baseline | 218 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.3 ms | 4.3% | 1.05× [0.99–1.12] | 233 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.25 ms | 3.0% | baseline | 235 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.33 ms | 7.0% | 0.99× [0.93–1.07] | 231 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.02 ms | 0.7% | baseline | 249 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.23 ms | 2.6% | **1.76× [1.01–1.82]** | 448 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.45 ms | 7.0% | baseline | 408 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.37 ms | 5.4% | 1.01× [0.95–1.10] | 421 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.57 ms | 2.3% | baseline | 637 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.55 ms | 1.8% | 1.00× [0.96–1.04] | 646 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.09 ms | 0.8% | baseline | 478 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.06 ms | 0.8% | **1.02× [1.01–1.03]** | 485 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/Ordered/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 71.3 ms | 2.5% | baseline | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.7 ms | 2.8% | 1.02× [0.98–1.06] | 14.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native1 | 71.3 ms | 3.3% | 0.99× [0.88–1.05] | 14 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline1 | 70.7 ms | 3.1% | 1.00× [0.90–1.06] | 14.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x1 | 70.7 ms | 2.3% | 0.99× [0.88–1.06] | 14.2 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x1 | 71.8 ms | 3.1% | 0.99× [0.95–1.04] | 13.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native2 | 36.4 ms | 4.3% | **1.95× [1.18–2.10]** | 27.5 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline2 | 35.9 ms | 4.9% | **1.99× [1.42–2.10]** | 27.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x2 | 36.2 ms | 3.7% | **1.94× [1.33–2.11]** | 27.6 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x2 | 35.9 ms | 5.7% | **1.94× [1.30–2.06]** | 27.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Native4 | 30.9 ms | 17.8% | **2.27× [2.07–3.78]** | 32.4 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| Pipeline4 | 28.3 ms | 12.5% | **2.59× [2.09–3.67]** | 35.3 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| NativeG64x4 | 22.7 ms | 2.6% | **3.09× [2.83–4.03]** | 44.1 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |
| PipelineG64x4 | 32.4 ms | 1.6% | **2.28× [2.05–3.20]** | 30.9 | state_checksum=3.197e+04, completed_ticks=16, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 69.1 ms | 2.7% | baseline | 14.5 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 68.4 ms | 2.8% | 1.00× [0.98–1.03] | 14.6 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline1/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 67.9 ms | 1.3% | baseline | 14.7 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 69.6 ms | 0.9% | **0.97× [0.94–0.99]** | 14.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 34.3 ms | 2.0% | baseline | 29.1 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.4 ms | 1.8% | 0.98× [0.94–1.00] | 28.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline2/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 37.1 ms | 5.8% | baseline | 26.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 35.9 ms | 4.0% | 0.99× [0.95–1.04] | 27.8 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedNative4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 25.7 ms | 4.9% | baseline | 38.9 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 25.4 ms | 5.5% | 1.00× [0.93–1.06] | 39.4 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## NBody/MatchedPipeline4/4096

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.1 ms | 10.2% | baseline | 47.3 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |
| ECS | 24.3 ms | 25.4% | 1.04× [0.95–1.12] | 41.2 | state_checksum=3.197e+04, completed_ticks=14, directed_interactions_per_tick=1.677e+07 |

## RowDispatch/Coherent/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.1 ns | 2.7% | baseline | 70.9 M | checksum=9.4e+07 |
| Each | 15.5 ns | 2.1% | **0.89× [0.86–0.98]** | 64.4 M | checksum=9.387e+07 |
| Grain1 | 21.3 ns | 0.8% | **0.65× [0.61–0.70]** | 47 M | checksum=9.354e+07 |
| Grain64 | 17.7 ns | 2.1% | **0.80× [0.76–0.85]** | 56.6 M | checksum=9.399e+07 |
| Grain1024 | 20.7 ns | 1.9% | **0.68× [0.64–0.74]** | 48.3 M | checksum=9.399e+07 |

## RowDispatch/Fragmented/64

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 14.9 ns | 2.3% | baseline | 67 M | checksum=9.243e+07 |
| Each | 18.5 ns | 2.0% | **0.80× [0.75–0.85]** | 54.1 M | checksum=9.226e+07 |
| Grain1 | 138 ns | 0.4% | **0.11× [0.10–0.11]** | 7.26 M | checksum=9.124e+07 |
| Grain64 | 21.3 ns | 1.1% | **0.70× [0.67–0.72]** | 47 M | checksum=9.229e+07 |
| Grain1024 | 20.7 ns | 1.1% | **0.73× [0.68–0.74]** | 48.4 M | checksum=9.227e+07 |

## RowDispatch/Coherent/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 701 ns | 2.3% | baseline | 1.43 M | checksum=2.948e+08 |
| Each | 663 ns | 1.7% | **1.06× [1.03–1.13]** | 1.51 M | checksum=2.952e+08 |
| Grain1 | 1.19 µs | 2.5% | **0.59× [0.58–0.64]** | 842 k | checksum=2.927e+08 |
| Grain64 | 763 ns | 2.8% | 0.93× [0.83–1.01] | 1.31 M | checksum=2.936e+08 |
| Grain1024 | 1.2 µs | 2.1% | **0.59× [0.57–0.65]** | 836 k | checksum=2.931e+08 |

## RowDispatch/Fragmented/4096

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 707 ns | 1.9% | baseline | 1.41 M | checksum=2.745e+08 |
| Each | 737 ns | 7.4% | 1.00× [0.93–1.12] | 1.36 M | checksum=2.743e+08 |
| Grain1 | 9.01 µs | 3.1% | **0.08× [0.07–0.09]** | 111 k | checksum=2.705e+08 |
| Grain64 | 1.07 µs | 3.5% | **0.67× [0.63–0.79]** | 934 k | checksum=2.731e+08 |
| Grain1024 | 1.21 µs | 2.1% | **0.60× [0.56–0.74]** | 828 k | checksum=2.728e+08 |

## RowDispatch/Coherent/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.2 µs | 2.4% | baseline | 47.2 k | checksum=5.23e+09 |
| Each | 20.6 µs | 3.2% | 1.02× [0.84–1.04] | 48.6 k | checksum=5.23e+09 |
| Grain1 | 29.4 µs | 4.9% | **0.73× [0.64–0.76]** | 34 k | checksum=5.229e+09 |
| Grain64 | 21.2 µs | 3.6% | 0.98× [0.89–1.06] | 47.1 k | checksum=5.23e+09 |
| Grain1024 | 30 µs | 3.0% | **0.71× [0.66–0.76]** | 33.3 k | checksum=5.229e+09 |

## RowDispatch/Fragmented/100000

| Design | time/pass | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 21.7 µs | 4.1% | baseline | 46.2 k | checksum=5.235e+09 |
| Each | 21.4 µs | 3.3% | 1.00× [0.94–1.03] | 46.7 k | checksum=5.235e+09 |
| Grain1 | 224 µs | 3.2% | **0.10× [0.09–0.10]** | 4.46 k | checksum=5.231e+09 |
| Grain64 | 27.7 µs | 3.6% | **0.77× [0.70–0.80]** | 36.2 k | checksum=5.234e+09 |
| Grain1024 | 30.6 µs | 5.3% | **0.71× [0.62–0.79]** | 32.7 k | checksum=5.233e+09 |

