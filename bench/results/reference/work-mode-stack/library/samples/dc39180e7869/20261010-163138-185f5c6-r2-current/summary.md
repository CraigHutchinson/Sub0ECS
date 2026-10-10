# Benchmark run 20261010-163138-185f5c6-r2-current

- stack: ok in 2 s
- nbody: ok in 2 s
- row-workloads: ok in 0 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.3% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | 1.01× [0.88–1.01] | 64.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.3 µs | 0.7% | **0.97× [0.91–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.3 µs | 0.6% | **0.97× [0.95–0.97]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.2 µs | 0.4% | **0.97× [0.96–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.6% | **0.97× [0.49–0.97]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.3 µs | 0.4% | **0.97× [0.96–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 0.8% | **0.97× [0.95–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.4% | **0.96× [0.96–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.3 µs | 0.5% | **0.97× [0.94–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.6% | **0.97× [0.96–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.2% | **0.97× [0.97–0.98]** | 61.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.3 µs | 0.4% | **0.97× [0.95–0.97]** | 61.3 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.4% | **0.97× [0.96–0.98]** | 61.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/Compact/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.5% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.4% | **1.01× [1.00–3.15]** | 64.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactECS | 10.1 µs | 1.2% | **1.56× [1.45–1.58]** | 98.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactHandWritten | 11.4 µs | 0.4% | **1.38× [1.38–3.97]** | 87.9 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.4 µs | 0.6% | 0.96× [0.94–2.98] | 61 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x1 | 12 µs | 0.2% | **1.31× [1.28–3.88]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.7% | **0.97× [0.65–0.99]** | 61.3 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x1 | 12 µs | 0.3% | **1.31× [1.27–3.83]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.5% | 0.97× [0.96–2.99] | 61.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x2 | 12.1 µs | 0.5% | **1.30× [1.29–3.59]** | 82.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.3 µs | 0.3% | 0.97× [0.91–2.99] | 61.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x2 | 12 µs | 0.3% | **1.31× [1.15–3.88]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.3 µs | 0.3% | 0.96× [0.95–2.98] | 61.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x4 | 12.1 µs | 0.6% | **1.30× [1.23–3.87]** | 82.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.2 µs | 0.3% | 0.97× [0.96–3.01] | 61.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x4 | 12 µs | 0.3% | **1.31× [1.20–3.86]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.5% | baseline | 93.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.88–0.89]** | 83.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.2% | baseline | 62.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | 1.00× [0.99–1.00] | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.4% | baseline | 94.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.88× [0.88–0.90]** | 83.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.95]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.7% | baseline | 93.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.4% | **0.89× [0.89–0.89]** | 83.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16 µs | 0.4% | baseline | 62.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.99× [0.98–0.99]** | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.1% | baseline | 94.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.88× [0.88–0.89]** | 83.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.4% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.6% | **0.95× [0.94–0.97]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.2% | baseline | 93.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.89–0.89]** | 83.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.4% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.99× [0.99–1.00]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.4% | baseline | 94.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.88× [0.88–0.89]** | 83.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.7% | **0.94× [0.88–0.95]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.69 ms | 4.4% | baseline | 213 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.45 ms | 3.8% | 1.03× [0.92–1.16] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.71 ms | 7.2% | 1.01× [0.82–1.15] | 213 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.46 ms | 5.0% | 1.06× [0.89–1.13] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.53 ms | 5.3% | 1.03× [0.80–1.12] | 221 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.43 ms | 3.7% | 1.04× [0.87–1.23] | 226 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.62 ms | 8.6% | 1.03× [0.93–1.10] | 216 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.57 ms | 5.7% | 0.99× [0.82–1.13] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.45 ms | 4.7% | **1.86× [1.19–2.10]** | 408 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.29 ms | 4.5% | **2.03× [1.79–2.19]** | 436 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.56 ms | 4.4% | 1.01× [0.86–1.14] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.5 ms | 3.8% | 1.00× [0.82–1.18] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.6 ms | 2.3% | **2.84× [1.05–3.42]** | 624 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.1 ms | 1.1% | **2.20× [2.00–3.26]** | 477 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/Compact/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.17 ms | 2.1% | baseline | 240 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.29 ms | 3.4% | 0.99× [0.95–1.04] | 233 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactECS | 2.69 ms | 4.3% | **1.60× [1.45–1.78]** | 372 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactHandWritten | 2.62 ms | 5.7% | **1.61× [1.38–1.66]** | 381 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.17 ms | 2.9% | 0.99× [0.85–1.10] | 240 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x1 | 2.99 ms | 7.2% | **1.50× [1.30–1.77]** | 335 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.21 ms | 2.2% | 0.99× [0.93–1.10] | 237 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x1 | 2.61 ms | 4.5% | **1.60× [1.45–1.80]** | 383 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.31 ms | 6.1% | **1.81× [1.18–2.10]** | 433 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x2 | 1.43 ms | 4.9% | **2.98× [1.58–3.16]** | 701 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.42 ms | 5.1% | **1.70× [1.45–1.97]** | 413 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x2 | 1.51 ms | 7.0% | **2.87× [2.06–3.08]** | 660 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.61 ms | 4.2% | 2.65× [0.96–4.13] | 623 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x4 | 1.02 ms | 20.2% | **4.08× [2.63–5.98]** | 980 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.11 ms | 1.2% | **1.99× [1.88–2.28]** | 475 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x4 | 1.4 ms | 4.3% | **2.99× [2.02–3.40]** | 713 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.92 ms | 5.5% | baseline | 343 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.6 ms | 2.3% | **1.12× [1.02–1.21]** | 385 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.67 ms | 1.4% | baseline | 214 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.68 ms | 5.7% | 1.00× [0.94–1.02] | 213 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.65 ms | 2.8% | baseline | 377 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.85 ms | 11.2% | 1.01× [0.91–1.03] | 351 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.36 ms | 4.3% | baseline | 230 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.44 ms | 4.2% | 0.95× [0.91–1.12] | 225 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.39 ms | 8.5% | baseline | 718 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.41 ms | 4.6% | 1.00× [0.92–1.14] | 711 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.03 ms | 1.2% | baseline | 248 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.3 ms | 4.0% | 1.68× [0.99–1.85] | 435 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.66 ms | 1.7% | baseline | 376 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.46 ms | 8.4% | 1.49× [0.97–1.88] | 685 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.52 ms | 7.3% | baseline | 397 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.28 ms | 4.9% | **1.10× [1.01–1.27]** | 440 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.1 ms | 3.4% | baseline | 907 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 940 µs | 8.5% | **1.16× [1.07–1.58]** | 1.06 k | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.28 ms | 3.8% | baseline | 781 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.17 ms | 4.0% | 1.06× [1.00–1.16] | 855 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.35 ms | 0.7% | baseline | 739 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.34 ms | 0.7% | 1.00× [0.97–1.01] | 746 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.1 ms | 0.7% | baseline | 475 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.07 ms | 0.7% | **1.02× [1.00–1.03]** | 482 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## RowWork/Streaming/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 791 ns | 2.5% | baseline | 1.26 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 802 ns | 3.2% | 0.99× [0.74–1.01] | 1.25 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 2.6 µs | 0.8% | **0.30× [0.26–0.32]** | 384 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 3.45 µs | 0.6% | **0.23× [0.22–0.31]** | 290 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 2.87 µs | 0.8% | **0.28× [0.27–0.38]** | 349 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 851 ns | 3.5% | 0.92× [0.64–1.01] | 1.18 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 2.86 µs | 0.5% | **0.28× [0.25–0.38]** | 349 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 19.8 µs | 65.1% | **0.04× [0.01–0.09]** | 50.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 2.84 µs | 0.5% | **0.28× [0.26–0.38]** | 352 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 12.4 µs | 7.9% | **0.07× [0.03–0.07]** | 81 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 2.85 µs | 1.7% | **0.28× [0.26–0.45]** | 351 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 94.8 µs | 7.2% | **0.01× [0.01–0.04]** | 10.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 2.86 µs | 0.9% | **0.28× [0.25–0.45]** | 350 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 43 µs | 10.6% | **0.02× [0.01–0.02]** | 23.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.93 µs | 1.2% | baseline | 341 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| ECS | 4.88 µs | 7.4% | **0.61× [0.32–0.77]** | 205 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x1 | 6.53 µs | 5.5% | **0.46× [0.26–0.53]** | 153 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x1 | 6.69 µs | 7.0% | **0.45× [0.28–0.52]** | 149 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x1 | 5.03 µs | 9.8% | **0.61× [0.29–0.73]** | 199 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x1 | 5.24 µs | 10.3% | **0.58× [0.36–0.69]** | 191 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x2 | 16.3 µs | 33.0% | **0.18× [0.04–0.24]** | 61.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x2 | 13.5 µs | 11.6% | **0.22× [0.13–0.24]** | 73.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x2 | 30 µs | 26.8% | **0.10× [0.05–0.18]** | 33.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x2 | 28 µs | 7.8% | **0.10× [0.06–0.15]** | 35.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x4 | 109 µs | 1.7% | **0.03× [0.02–0.03]** | 9.21 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x4 | 26.6 µs | 11.5% | **0.11× [0.03–0.12]** | 37.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x4 | 31 µs | 8.1% | **0.10× [0.08–0.11]** | 32.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x4 | 69.3 µs | 39.9% | **0.04× [0.03–0.07]** | 14.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |

## RowWork/StagedNeighbors/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.3 µs | 0.2% | baseline | 61.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| ECS | 16.4 µs | 0.1% | **1.00× [0.44–1.00]** | 61 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x1 | 16.4 µs | 0.3% | **0.99× [0.98–1.00]** | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x1 | 16.6 µs | 1.2% | 0.99× [0.45–1.00] | 60.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x1 | 16.6 µs | 1.4% | **0.98× [0.96–0.99]** | 60.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x1 | 16.5 µs | 0.2% | **0.99× [0.98–0.99]** | 60.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x2 | 16.5 µs | 0.1% | **0.99× [0.98–0.99]** | 60.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x2 | 16.9 µs | 2.9% | 0.96× [0.70–1.01] | 59.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x2 | 16.6 µs | 1.4% | **0.99× [0.94–1.00]** | 60.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x2 | 36.2 µs | 1.4% | **0.45× [0.42–0.46]** | 27.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x4 | 16.4 µs | 0.3% | **0.99× [0.97–0.99]** | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x4 | 96.6 µs | 4.7% | **0.17× [0.15–0.44]** | 10.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x4 | 16.9 µs | 0.7% | **0.96× [0.96–0.99]** | 59.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x4 | 36.7 µs | 10.1% | **0.44× [0.36–0.50]** | 27.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |

## RowWork/Streaming/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.4 µs | 2.0% | baseline | 96.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 10.4 µs | 1.4% | 1.01× [0.98–1.03] | 96.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 40 µs | 2.7% | **0.26× [0.23–0.35]** | 25 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 53 µs | 0.5% | **0.20× [0.19–0.44]** | 18.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 10.4 µs | 1.4% | 0.99× [0.96–1.08] | 95.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 10.6 µs | 1.7% | 0.98× [0.93–1.03] | 94.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 22.6 µs | 26.3% | **0.52× [0.09–0.74]** | 44.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 26.9 µs | 23.4% | **0.43× [0.26–0.52]** | 37.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 43.4 µs | 16.0% | **0.26× [0.10–0.30]** | 23 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 30 µs | 2.6% | **0.35× [0.12–0.37]** | 33.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 116 µs | 6.9% | **0.09× [0.08–0.41]** | 8.62 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 25 µs | 2.5% | **0.42× [0.35–0.43]** | 40 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 38.5 µs | 17.1% | **0.34× [0.23–0.42]** | 26 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 33.7 µs | 3.4% | **0.31× [0.28–0.40]** | 29.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 43.8 µs | 14.6% | baseline | 22.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| ECS | 66.9 µs | 14.5% | **0.71× [0.33–0.89]** | 15 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x1 | 102 µs | 12.7% | **0.39× [0.30–0.60]** | 9.81 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x1 | 81.2 µs | 14.7% | **0.48× [0.31–0.73]** | 12.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x1 | 62.2 µs | 22.2% | 0.58× [0.31–1.05] | 16.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x1 | 58.7 µs | 15.7% | 0.61× [0.31–1.01] | 17 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x2 | 86.9 µs | 16.2% | **0.52× [0.25–0.74]** | 11.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x2 | 113 µs | 7.5% | **0.33× [0.22–0.45]** | 8.84 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x2 | 104 µs | 12.6% | **0.37× [0.18–0.52]** | 9.59 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x2 | 111 µs | 8.9% | **0.37× [0.23–0.57]** | 8.99 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x4 | 92.8 µs | 14.1% | **0.41× [0.26–0.70]** | 10.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x4 | 105 µs | 16.1% | **0.40× [0.21–0.61]** | 9.55 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x4 | 108 µs | 10.4% | **0.38× [0.23–0.56]** | 9.29 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x4 | 105 µs | 9.6% | **0.38× [0.20–0.55]** | 9.52 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |

## RowWork/StagedNeighbors/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 275 µs | 3.0% | baseline | 3.63 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| ECS | 279 µs | 4.0% | 0.96× [0.73–1.24] | 3.58 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x1 | 279 µs | 3.6% | 0.98× [0.77–1.27] | 3.58 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x1 | 276 µs | 2.3% | 1.01× [0.73–1.21] | 3.63 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x1 | 273 µs | 1.7% | 1.00× [0.83–1.27] | 3.66 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x1 | 288 µs | 7.8% | 0.98× [0.76–1.26] | 3.48 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x2 | 169 µs | 3.6% | 1.59× [0.97–2.06] | 5.93 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x2 | 174 µs | 5.5% | **1.63× [1.02–1.82]** | 5.74 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x2 | 231 µs | 15.8% | 1.30× [0.91–1.65] | 4.32 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x2 | 203 µs | 11.2% | 1.38× [0.84–1.88] | 4.93 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x4 | 167 µs | 6.8% | **1.72× [1.50–2.22]** | 5.98 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x4 | 131 µs | 8.3% | **2.16× [1.68–2.43]** | 7.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x4 | 194 µs | 8.3% | **1.40× [1.14–1.55]** | 5.16 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x4 | 200 µs | 12.5% | 1.41× [0.94–1.59] | 5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |

## Stack/Admission/1

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.8 ns | 0.8% | baseline | 208 M | commands_consumed=4.617e+05 |
| Domain | 20.9 ns | 2.0% | **0.23× [0.20–0.24]** | 47.7 M | commands_consumed=4.538e+05 |
| Wiring | 4.79 ns | 1.1% | 1.01× [0.74–1.04] | 209 M | commands_consumed=4.617e+05 |

## Stack/Admission/8

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 9.51 ns | 9.2% | baseline | 105 M | commands_consumed=3.354e+06 |
| Domain | 25.1 ns | 1.2% | **0.36× [0.33–0.52]** | 39.8 M | commands_consumed=3.346e+06 |
| Wiring | 9.25 ns | 3.7% | 1.05× [0.80–1.50] | 108 M | commands_consumed=3.367e+06 |

## Stack/Boundary1/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 4.3% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.19 µs | 5.7% | **0.10× [0.08–0.11]** | 839 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.1 µs | 2.2% | **0.11× [0.10–0.11]** | 907 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 811 ns | 1.2% | **0.15× [0.14–0.16]** | 1.23 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 781 ns | 1.3% | **0.15× [0.14–0.16]** | 1.28 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 410 ns | 2.5% | **0.30× [0.28–0.33]** | 2.44 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 396 ns | 1.4% | **0.30× [0.28–0.33]** | 2.53 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary2/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 0.8% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.17 µs | 1.3% | **0.10× [0.09–0.10]** | 857 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.1 µs | 2.3% | **0.10× [0.10–0.11]** | 912 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 842 ns | 2.5% | **0.13× [0.13–0.15]** | 1.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 776 ns | 0.6% | **0.15× [0.14–0.16]** | 1.29 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 436 ns | 6.7% | **0.27× [0.21–0.30]** | 2.3 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 391 ns | 2.5% | **0.30× [0.28–0.32]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary4/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 0.4% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.2 µs | 3.9% | **0.10× [0.09–0.11]** | 832 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.07 µs | 1.4% | **0.11× [0.10–0.12]** | 933 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 816 ns | 1.9% | **0.15× [0.13–0.15]** | 1.22 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 841 ns | 6.5% | **0.14× [0.12–0.16]** | 1.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 406 ns | 3.4% | **0.29× [0.27–0.31]** | 2.46 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 401 ns | 2.4% | **0.30× [0.27–0.32]** | 2.49 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary1/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.09 µs | 10.1% | baseline | 164 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 399 µs | 10.6% | **0.02× [0.01–0.02]** | 2.51 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 350 µs | 6.2% | **0.02× [0.01–0.02]** | 2.85 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 200 µs | 1.5% | **0.03× [0.02–0.04]** | 5 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 196 µs | 0.9% | **0.03× [0.03–0.04]** | 5.11 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 15.8 µs | 3.4% | **0.38× [0.34–0.55]** | 63.3 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 15.9 µs | 4.4% | **0.35× [0.15–0.48]** | 62.9 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary2/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.76 µs | 12.3% | baseline | 148 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 395 µs | 3.9% | **0.02× [0.01–0.03]** | 2.53 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 395 µs | 3.9% | **0.02× [0.01–0.03]** | 2.53 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 253 µs | 4.1% | **0.03× [0.02–0.05]** | 3.95 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 278 µs | 14.1% | **0.03× [0.01–0.03]** | 3.6 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 55.5 µs | 10.1% | **0.12× [0.11–0.20]** | 18 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 57.3 µs | 9.2% | **0.13× [0.08–0.20]** | 17.5 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary4/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 7.44 µs | 11.9% | baseline | 134 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 426 µs | 7.9% | **0.02× [0.01–0.03]** | 2.35 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 423 µs | 13.1% | **0.02× [0.01–0.02]** | 2.36 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 265 µs | 6.9% | **0.03× [0.02–0.04]** | 3.77 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 246 µs | 2.8% | **0.03× [0.02–0.04]** | 4.07 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 55.5 µs | 18.6% | **0.13× [0.08–0.18]** | 18 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 56.4 µs | 10.8% | **0.13× [0.06–0.16]** | 17.7 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary1/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 146 µs | 2.4% | baseline | 6.85 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.98 ms | 5.4% | **0.02× [0.02–0.02]** | 125 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.51 ms | 1.3% | **0.02× [0.02–0.03]** | 133 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.84 ms | 4.0% | **0.03× [0.03–0.04]** | 207 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.88 ms | 4.0% | **0.03× [0.03–0.04]** | 205 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 435 µs | 6.2% | **0.34× [0.27–0.49]** | 2.3 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 432 µs | 4.8% | **0.35× [0.31–0.45]** | 2.31 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary2/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 147 µs | 5.5% | baseline | 6.8 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 8.35 ms | 6.6% | **0.02× [0.02–0.03]** | 120 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.66 ms | 4.2% | **0.02× [0.02–0.03]** | 131 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 5.35 ms | 9.9% | **0.03× [0.03–0.03]** | 187 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 5.13 ms | 4.4% | **0.03× [0.03–0.04]** | 195 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 579 µs | 20.7% | **0.29× [0.23–0.37]** | 1.73 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 509 µs | 13.7% | **0.32× [0.19–0.45]** | 1.97 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary4/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 188 µs | 30.5% | baseline | 5.31 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 8 ms | 3.9% | **0.02× [0.02–0.04]** | 125 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 8.13 ms | 4.9% | **0.02× [0.02–0.04]** | 123 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 5.3 ms | 8.7% | **0.04× [0.03–0.05]** | 189 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 5.31 ms | 8.4% | **0.04× [0.03–0.05]** | 188 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 470 µs | 12.6% | **0.35× [0.30–0.62]** | 2.13 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 471 µs | 11.6% | **0.37× [0.25–0.68]** | 2.12 k | state_checksum=3.652e+10, completed_ticks=17 |

