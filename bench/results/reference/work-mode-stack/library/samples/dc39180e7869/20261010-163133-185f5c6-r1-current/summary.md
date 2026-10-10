# Benchmark run 20261010-163133-185f5c6-r1-current

- stack: ok in 2 s
- nbody: ok in 3 s
- row-workloads: ok in 0 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.7% | baseline | 63.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.4% | 1.01× [0.44–1.19] | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.4 µs | 1.1% | **0.97× [0.44–0.99]** | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 0.2% | 0.96× [0.89–1.13] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.3 µs | 0.7% | 0.97× [0.83–1.14] | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.4% | 0.97× [0.94–1.14] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.3% | 0.97× [0.95–1.15] | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.2 µs | 0.5% | 0.97× [0.96–1.15] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.4% | **0.97× [0.50–1.00]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.3 µs | 0.2% | 0.97× [0.96–1.14] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.3 µs | 0.6% | 0.97× [0.96–1.14] | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.4% | 0.97× [0.96–1.14] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.2 µs | 0.3% | 0.98× [0.97–1.14] | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.4 µs | 0.3% | 0.97× [0.96–1.13] | 61.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/Compact/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.3% | baseline | 63.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.2% | 1.02× [0.99–1.03] | 64.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactECS | 9.96 µs | 0.5% | **1.59× [1.53–1.60]** | 100 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactHandWritten | 11.4 µs | 0.7% | **1.39× [1.36–1.40]** | 87.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16 µs | 0.8% | 0.99× [0.98–1.02] | 62.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x1 | 12.1 µs | 0.2% | **1.31× [1.31–1.34]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 15.9 µs | 1.2% | 0.99× [0.98–1.02] | 62.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x1 | 12 µs | 0.1% | **1.32× [1.31–1.35]** | 83.3 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16 µs | 0.7% | 0.99× [0.98–1.03] | 62.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x2 | 12.1 µs | 0.3% | **1.31× [1.30–1.33]** | 82.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16 µs | 0.5% | 0.99× [0.98–1.01] | 62.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x2 | 12 µs | 0.4% | **1.32× [1.31–1.34]** | 83.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.1 µs | 0.9% | 0.99× [0.97–1.02] | 62.3 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x4 | 12 µs | 0.2% | **1.31× [1.31–1.34]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 15.9 µs | 0.9% | 0.99× [0.98–1.02] | 62.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x4 | 12 µs | 0.3% | **1.31× [1.30–1.33]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.6% | baseline | 93.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.89–0.90]** | 83.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.8 µs | 0.9% | baseline | 63.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.9 µs | 0.6% | 0.99× [0.98–1.00] | 62.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.3% | baseline | 94.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.1% | **0.88× [0.88–0.89]** | 83.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.1% | baseline | 65 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16 µs | 1.1% | **0.96× [0.96–0.97]** | 62.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.4% | baseline | 93.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.89–0.90]** | 83.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.7% | baseline | 63.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 15.9 µs | 0.9% | 0.99× [0.98–1.02] | 62.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.6% | baseline | 94.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.88–0.89]** | 83.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16 µs | 0.4% | **0.96× [0.96–0.97]** | 62.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.3% | baseline | 93.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.1% | **0.89× [0.88–0.89]** | 83 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 1.1% | baseline | 62.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16 µs | 0.4% | 0.99× [0.97–1.23] | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.4% | baseline | 94.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.1% | **0.89× [0.88–0.89]** | 83.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.95× [0.95–0.95]** | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.15 ms | 1.6% | baseline | 241 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.24 ms | 5.1% | 0.98× [0.90–1.07] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.25 ms | 4.8% | 0.99× [0.88–1.07] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.24 ms | 3.4% | 0.99× [0.74–1.06] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.26 ms | 4.2% | 0.98× [0.87–1.06] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.35 ms | 2.1% | 0.98× [0.93–1.04] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.26 ms | 3.7% | 0.99× [0.94–1.06] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.28 ms | 4.1% | 0.99× [0.94–1.07] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.06 ms | 0.3% | 1.04× [0.99–1.77] | 247 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.63 ms | 12.1% | **1.56× [1.04–1.90]** | 380 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.25 ms | 5.3% | 0.99× [0.75–1.07] | 235 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.11 ms | 1.6% | 1.00× [0.93–1.08] | 243 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.62 ms | 4.1% | **2.59× [1.96–2.71]** | 617 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.15 ms | 1.1% | **1.94× [1.86–2.05]** | 465 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/Compact/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.39 ms | 5.9% | baseline | 228 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.36 ms | 6.3% | 1.00× [0.78–1.11] | 229 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactECS | 2.62 ms | 3.8% | **1.64× [1.43–2.22]** | 382 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactHandWritten | 2.88 ms | 7.1% | **1.59× [1.28–1.78]** | 347 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.49 ms | 8.6% | 1.00× [0.76–1.17] | 223 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x1 | 2.95 ms | 13.2% | **1.61× [1.35–1.71]** | 339 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.35 ms | 5.6% | 1.05× [0.92–1.28] | 230 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x1 | 2.71 ms | 5.9% | **1.67× [1.43–1.95]** | 369 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 3.7 ms | 15.1% | 1.28× [1.00–2.17] | 271 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x2 | 1.4 ms | 4.0% | **3.18× [2.89–3.52]** | 713 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.55 ms | 12.4% | **1.76× [1.02–2.20]** | 391 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x2 | 1.85 ms | 26.7% | **2.43× [1.49–3.89]** | 541 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.66 ms | 14.5% | **2.58× [1.21–4.04]** | 603 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x4 | 1.03 ms | 10.6% | **4.13× [3.35–6.91]** | 975 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.12 ms | 3.8% | **2.08× [1.80–3.32]** | 471 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x4 | 1.38 ms | 3.1% | **3.13× [2.25–3.63]** | 724 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 3.12 ms | 5.7% | baseline | 320 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.93 ms | 11.6% | 1.01× [0.93–1.09] | 341 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.72 ms | 0.3% | baseline | 212 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.7 ms | 0.3% | 1.00× [1.00–1.00] | 213 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.61 ms | 3.8% | baseline | 383 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.49 ms | 1.1% | 1.00× [0.97–1.05] | 401 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.71 ms | 0.4% | baseline | 213 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.69 ms | 0.4% | 1.00× [1.00–1.01] | 213 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.37 ms | 5.9% | baseline | 728 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.41 ms | 1.7% | 1.05× [0.93–1.07] | 710 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.19 ms | 3.4% | baseline | 457 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.3 ms | 3.6% | 0.96× [0.92–1.01] | 435 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.42 ms | 6.5% | baseline | 702 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.58 ms | 5.7% | **0.94× [0.87–1.00]** | 635 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.45 ms | 4.9% | baseline | 408 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.69 ms | 8.1% | 0.94× [0.83–1.10] | 372 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 820 µs | 13.2% | baseline | 1.22 k | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 861 µs | 12.5% | 0.90× [0.76–1.25] | 1.16 k | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.57 ms | 2.9% | baseline | 638 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.55 ms | 0.9% | 0.99× [0.98–1.05] | 646 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 945 µs | 23.7% | baseline | 1.06 k | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.07 ms | 23.9% | 1.00× [0.71–1.26] | 937 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.14 ms | 3.8% | baseline | 468 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.2 ms | 3.9% | 0.99× [0.94–1.02] | 455 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## RowWork/Streaming/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 901 ns | 12.3% | baseline | 1.11 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 972 ns | 15.7% | 0.94× [0.80–1.05] | 1.03 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 2.93 µs | 7.8% | **0.31× [0.27–0.41]** | 341 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 3.54 µs | 4.9% | **0.26× [0.21–0.41]** | 282 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 2.88 µs | 1.7% | **0.32× [0.27–0.50]** | 347 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 906 ns | 8.3% | 0.98× [0.77–1.11] | 1.1 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 2.85 µs | 5.8% | **0.32× [0.26–0.47]** | 350 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 95.2 µs | 8.0% | **0.01× [0.01–0.08]** | 10.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 3.01 µs | 5.7% | **0.30× [0.27–0.52]** | 332 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 33 µs | 62.4% | **0.03× [0.01–0.05]** | 30.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 2.75 µs | 8.0% | **0.31× [0.27–0.66]** | 363 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 37.8 µs | 22.9% | **0.03× [0.02–0.09]** | 26.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 2.67 µs | 4.1% | **0.33× [0.29–0.48]** | 374 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 33.9 µs | 9.3% | **0.03× [0.02–0.06]** | 29.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 3.24 µs | 5.2% | baseline | 309 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| ECS | 5.52 µs | 19.3% | **0.62× [0.29–0.78]** | 181 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x1 | 7.51 µs | 18.9% | **0.43× [0.25–0.53]** | 133 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x1 | 6.8 µs | 12.2% | **0.48× [0.28–0.59]** | 147 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x1 | 5.77 µs | 21.3% | **0.59× [0.27–0.74]** | 173 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x1 | 5 µs | 12.7% | **0.65× [0.35–0.88]** | 200 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x2 | 15.3 µs | 26.2% | **0.23× [0.08–0.27]** | 65.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x2 | 18.4 µs | 36.6% | **0.16× [0.03–0.29]** | 54.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x2 | 30.7 µs | 10.2% | **0.10× [0.07–0.17]** | 32.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x2 | 28.6 µs | 32.7% | **0.14× [0.06–0.20]** | 35 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x4 | 117 µs | 35.6% | **0.03× [0.01–0.06]** | 8.55 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x4 | 46.6 µs | 26.4% | **0.06× [0.05–0.12]** | 21.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x4 | 58 µs | 23.5% | **0.06× [0.01–0.07]** | 17.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x4 | 122 µs | 38.0% | **0.03× [0.01–0.08]** | 8.18 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |

## RowWork/StagedNeighbors/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 µs | 0.3% | baseline | 60.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| ECS | 16.7 µs | 1.4% | 0.99× [0.97–1.02] | 59.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x1 | 16.5 µs | 0.5% | 1.00× [0.97–1.03] | 60.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x1 | 16.6 µs | 0.9% | 0.99× [0.97–1.01] | 60.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x1 | 16.6 µs | 0.5% | 1.00× [0.99–1.02] | 60.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x1 | 16.8 µs | 0.6% | **0.98× [0.97–0.98]** | 59.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x2 | 16.7 µs | 0.6% | **0.99× [0.98–0.99]** | 60 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x2 | 23.4 µs | 2.4% | **0.71× [0.67–0.74]** | 42.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x2 | 16.5 µs | 0.2% | 1.00× [0.99–1.03] | 60.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x2 | 47.7 µs | 8.5% | **0.35× [0.15–0.39]** | 21 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x4 | 16.7 µs | 1.5% | 0.99× [0.95–1.01] | 60 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x4 | 83.4 µs | 36.6% | **0.20× [0.06–0.43]** | 12 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x4 | 16.7 µs | 1.1% | **0.98× [0.97–1.00]** | 59.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x4 | 40.2 µs | 9.2% | **0.42× [0.24–0.50]** | 24.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |

## RowWork/Streaming/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 12.3 µs | 9.1% | baseline | 81.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 11.6 µs | 7.6% | 1.05× [0.96–1.15] | 86.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 44.5 µs | 0.5% | **0.27× [0.19–0.54]** | 22.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 51.5 µs | 4.4% | **0.24× [0.18–0.40]** | 19.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 11.2 µs | 3.7% | 1.06× [0.95–1.18] | 89.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 11.5 µs | 3.3% | 1.04× [0.93–1.15] | 86.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 16.8 µs | 7.1% | **0.71× [0.61–0.88]** | 59.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 105 µs | 23.9% | **0.12× [0.10–0.61]** | 9.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 36.5 µs | 13.1% | **0.35× [0.23–0.43]** | 27.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 30.7 µs | 7.2% | **0.39× [0.32–0.45]** | 32.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 137 µs | 33.5% | **0.09× [0.05–0.17]** | 7.31 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 23.1 µs | 9.9% | **0.56× [0.39–0.74]** | 43.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 74.2 µs | 30.0% | **0.16× [0.08–0.27]** | 13.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 38.1 µs | 16.0% | **0.33× [0.13–0.45]** | 26.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 31.9 µs | 3.5% | baseline | 31.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| ECS | 57.6 µs | 12.6% | 0.59× [0.27–1.07] | 17.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x1 | 71.3 µs | 8.0% | **0.47× [0.41–0.75]** | 14 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x1 | 75.7 µs | 13.7% | **0.43× [0.29–0.94]** | 13.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x1 | 61.6 µs | 25.3% | 0.55× [0.33–1.09] | 16.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x1 | 61.6 µs | 23.6% | 0.55× [0.26–1.10] | 16.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x2 | 67.2 µs | 17.6% | **0.51× [0.26–0.59]** | 14.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x2 | 77.9 µs | 10.6% | **0.43× [0.39–0.97]** | 12.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x2 | 128 µs | 45.8% | **0.31× [0.17–0.47]** | 7.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x2 | 85.5 µs | 7.0% | **0.39× [0.25–0.66]** | 11.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x4 | 80.8 µs | 7.5% | **0.39× [0.33–0.93]** | 12.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x4 | 86.3 µs | 5.2% | **0.38× [0.34–0.86]** | 11.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x4 | 90.9 µs | 17.5% | **0.36× [0.21–0.90]** | 11 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x4 | 88.1 µs | 10.0% | **0.37× [0.25–0.84]** | 11.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |

## RowWork/StagedNeighbors/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 287 µs | 4.3% | baseline | 3.49 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| ECS | 283 µs | 4.4% | 0.99× [0.84–1.20] | 3.53 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x1 | 278 µs | 2.4% | 0.99× [0.78–1.11] | 3.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x1 | 276 µs | 2.8% | 1.01× [0.75–1.11] | 3.62 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x1 | 279 µs | 3.3% | 1.01× [0.83–1.11] | 3.59 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x1 | 272 µs | 1.0% | 1.02× [0.88–1.21] | 3.67 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x2 | 168 µs | 4.2% | **1.71× [1.37–1.98]** | 5.95 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x2 | 175 µs | 4.1% | **1.57× [1.42–1.72]** | 5.73 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x2 | 326 µs | 8.8% | 0.91× [0.75–1.49] | 3.07 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x2 | 207 µs | 5.3% | **1.41× [1.17–1.58]** | 4.82 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x4 | 144 µs | 16.0% | **2.01× [1.52–2.56]** | 6.95 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x4 | 139 µs | 9.8% | **1.99× [1.70–2.61]** | 7.21 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x4 | 322 µs | 3.6% | 0.93× [0.81–1.52] | 3.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x4 | 204 µs | 7.2% | **1.34× [1.22–1.60]** | 4.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |

## Stack/Admission/1

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.78 ns | 0.6% | baseline | 209 M | commands_consumed=4.383e+05 |
| Domain | 21.4 ns | 5.0% | **0.23× [0.21–0.25]** | 46.6 M | commands_consumed=4.36e+05 |
| Wiring | 4.76 ns | 0.5% | 1.00× [0.94–1.08] | 210 M | commands_consumed=4.383e+05 |

## Stack/Admission/8

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 9.02 ns | 5.7% | baseline | 111 M | commands_consumed=3.548e+06 |
| Domain | 24.7 ns | 1.6% | **0.35× [0.32–0.38]** | 40.4 M | commands_consumed=3.519e+06 |
| Wiring | 9.28 ns | 2.9% | 0.99× [0.86–1.07] | 108 M | commands_consumed=3.548e+06 |

## Stack/Boundary1/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 7.7% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.15 µs | 3.6% | **0.10× [0.09–0.12]** | 872 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.17 µs | 4.5% | **0.11× [0.09–0.12]** | 857 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 846 ns | 5.0% | **0.14× [0.13–0.16]** | 1.18 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 806 ns | 3.3% | **0.15× [0.14–0.17]** | 1.24 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 456 ns | 7.1% | **0.27× [0.22–0.30]** | 2.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 391 ns | 0.0% | **0.31× [0.28–0.34]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary2/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 116 ns | 5.0% | baseline | 8.66 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.15 µs | 1.3% | **0.10× [0.10–0.11]** | 868 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.07 µs | 0.9% | **0.11× [0.10–0.12]** | 933 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 821 ns | 1.3% | **0.14× [0.13–0.17]** | 1.22 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 791 ns | 1.2% | **0.15× [0.14–0.17]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 410 ns | 4.8% | **0.28× [0.18–0.34]** | 2.44 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 396 ns | 1.4% | **0.29× [0.27–0.36]** | 2.53 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary4/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 0.4% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.14 µs | 1.8% | **0.11× [0.10–0.12]** | 876 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.08 µs | 0.6% | **0.11× [0.11–0.12]** | 929 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 871 ns | 1.7% | **0.14× [0.13–0.16]** | 1.15 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 796 ns | 1.2% | **0.15× [0.15–0.18]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 405 ns | 3.8% | **0.30× [0.28–0.32]** | 2.47 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 390 ns | 2.5% | **0.31× [0.30–0.39]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary1/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.63 µs | 2.6% | baseline | 177 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 358 µs | 2.6% | **0.02× [0.02–0.02]** | 2.79 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 329 µs | 1.1% | **0.02× [0.02–0.02]** | 3.04 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 198 µs | 1.0% | **0.03× [0.03–0.03]** | 5.06 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 197 µs | 1.2% | **0.03× [0.03–0.03]** | 5.09 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 15.3 µs | 1.7% | **0.37× [0.35–0.41]** | 65.2 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 14.8 µs | 1.8% | **0.38× [0.37–0.43]** | 67.4 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary2/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.62 µs | 2.0% | baseline | 178 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 395 µs | 4.3% | **0.01× [0.01–0.02]** | 2.53 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 371 µs | 4.0% | **0.02× [0.01–0.02]** | 2.69 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 236 µs | 2.0% | **0.02× [0.02–0.03]** | 4.24 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 229 µs | 2.6% | **0.02× [0.02–0.03]** | 4.37 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 49.7 µs | 9.9% | **0.12× [0.07–0.13]** | 20.1 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 46.9 µs | 6.4% | **0.13× [0.11–0.15]** | 21.3 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary4/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.86 µs | 4.7% | baseline | 171 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 398 µs | 3.4% | **0.01× [0.01–0.02]** | 2.51 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 362 µs | 1.3% | **0.02× [0.01–0.02]** | 2.76 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 234 µs | 2.8% | **0.02× [0.02–0.03]** | 4.28 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 231 µs | 2.8% | **0.02× [0.02–0.03]** | 4.33 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 46.4 µs | 3.4% | **0.13× [0.11–0.14]** | 21.5 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 45.6 µs | 10.2% | **0.13× [0.09–0.16]** | 21.9 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary1/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 145 µs | 3.3% | baseline | 6.92 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.65 ms | 3.9% | **0.02× [0.02–0.02]** | 131 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.62 ms | 5.7% | **0.02× [0.02–0.02]** | 131 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.85 ms | 4.6% | **0.03× [0.03–0.04]** | 206 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.85 ms | 3.1% | **0.03× [0.03–0.04]** | 206 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 447 µs | 6.7% | **0.34× [0.29–0.41]** | 2.24 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 459 µs | 9.8% | **0.33× [0.20–0.49]** | 2.18 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary2/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 141 µs | 2.4% | baseline | 7.1 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.68 ms | 3.8% | **0.02× [0.02–0.02]** | 130 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.76 ms | 4.5% | **0.02× [0.02–0.03]** | 129 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.9 ms | 3.6% | **0.03× [0.03–0.04]** | 204 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.89 ms | 3.6% | **0.03× [0.02–0.04]** | 204 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 463 µs | 7.5% | **0.32× [0.23–0.41]** | 2.16 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 451 µs | 6.5% | **0.32× [0.28–0.48]** | 2.22 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary4/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 146 µs | 3.1% | baseline | 6.86 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.7 ms | 3.9% | **0.02× [0.02–0.02]** | 130 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.54 ms | 4.1% | **0.02× [0.02–0.03]** | 133 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.8 ms | 2.6% | **0.03× [0.02–0.05]** | 208 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 5.01 ms | 7.3% | **0.03× [0.02–0.04]** | 200 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 463 µs | 4.9% | **0.33× [0.28–0.43]** | 2.16 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 488 µs | 4.2% | **0.31× [0.28–0.44]** | 2.05 k | state_checksum=3.652e+10, completed_ticks=17 |

