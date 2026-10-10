# Benchmark run 20261010-163144-185f5c6-r3-current

- stack: ok in 2 s
- nbody: ok in 2 s
- row-workloads: ok in 0 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.9 µs | 0.1% | baseline | 63.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.3% | 1.02× [0.53–1.03] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.1 µs | 0.5% | 0.99× [0.97–1.00] | 62.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.1 µs | 0.3% | 0.98× [0.97–1.02] | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.1 µs | 0.5% | 0.98× [0.97–1.00] | 62.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.2 µs | 0.5% | 0.98× [0.97–1.00] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.2 µs | 0.5% | 0.98× [0.97–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.3 µs | 0.7% | **0.98× [0.97–1.00]** | 61.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.2 µs | 0.3% | **0.98× [0.98–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.8% | **0.98× [0.97–0.99]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.2 µs | 0.6% | **0.98× [0.97–0.99]** | 61.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.2 µs | 0.9% | 0.98× [0.97–1.01] | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.1 µs | 0.5% | **0.98× [0.97–1.00]** | 61.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.1 µs | 0.3% | 0.98× [0.97–1.01] | 62 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/Compact/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.3% | baseline | 63.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.01× [1.01–1.25]** | 64.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactECS | 10.1 µs | 1.0% | **1.55× [1.51–1.81]** | 98.9 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactHandWritten | 11.4 µs | 0.5% | **1.38× [1.37–1.69]** | 88 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.3 µs | 0.4% | **0.96× [0.95–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x1 | 12.1 µs | 0.3% | **1.30× [1.29–1.58]** | 82.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.3% | 0.97× [0.96–1.21] | 61.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x1 | 12 µs | 0.3% | **1.31× [1.30–1.63]** | 83 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.2 µs | 0.4% | 0.97× [0.96–1.06] | 61.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x2 | 12.1 µs | 0.5% | 1.30× [0.49–1.61] | 82.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.2% | 0.97× [0.96–1.05] | 61.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x2 | 12 µs | 0.4% | **1.31× [1.03–1.57]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.4 µs | 0.6% | 0.97× [0.85–1.11] | 61.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x4 | 12.1 µs | 0.4% | **1.30× [1.30–1.61]** | 82.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.6% | 0.97× [0.96–1.10] | 61.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x4 | 12 µs | 0.3% | **1.31× [1.30–1.64]** | 83.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.9 µs | 2.8% | baseline | 91.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12.1 µs | 0.7% | **0.90× [0.88–0.93]** | 82.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.2% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.2% | **0.99× [0.99–1.00]** | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.5% | baseline | 94.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.88× [0.87–0.89]** | 83.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.4 µs | 0.2% | baseline | 65.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **0.95× [0.94–0.95]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.5% | baseline | 93.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.1% | **0.89× [0.88–0.89]** | 83.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.4% | baseline | 62.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.5% | **0.99× [0.98–1.00]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.4% | baseline | 94.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.88–0.89]** | 83.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.5% | **0.95× [0.94–0.95]** | 61.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.4% | baseline | 93.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.89× [0.89–0.91]** | 83.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 62 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | 1.00× [0.98–1.00] | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.3% | baseline | 94.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.88× [0.88–0.89]** | 83.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.3% | **0.95× [0.94–0.95]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.32 ms | 3.2% | baseline | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.37 ms | 5.2% | 0.99× [0.90–1.07] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.37 ms | 5.0% | 0.99× [0.90–1.09] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.21 ms | 3.3% | 1.02× [0.92–1.10] | 238 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.37 ms | 3.5% | 1.00× [0.92–1.09] | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.31 ms | 2.2% | 1.00× [0.95–1.08] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.45 ms | 2.9% | 0.97× [0.90–1.06] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.22 ms | 3.0% | 1.01× [0.97–1.06] | 237 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.07 ms | 0.5% | **1.04× [1.00–1.89]** | 246 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.35 ms | 5.8% | **1.82× [1.67–2.08]** | 426 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.24 ms | 5.2% | 1.02× [0.93–1.09] | 236 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.35 ms | 2.1% | 0.98× [0.95–1.09] | 230 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.58 ms | 2.2% | **2.72× [2.42–3.19]** | 632 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.1 ms | 1.5% | **1.98× [1.89–2.85]** | 475 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/Compact/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.24 ms | 2.8% | baseline | 236 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.25 ms | 3.1% | 1.00× [0.93–1.12] | 235 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactECS | 2.63 ms | 5.5% | **1.65× [1.25–1.74]** | 381 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactHandWritten | 2.72 ms | 7.9% | **1.56× [1.39–1.69]** | 368 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.3 ms | 3.2% | 1.00× [0.95–1.08] | 233 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x1 | 2.6 ms | 3.7% | **1.63× [1.36–1.69]** | 384 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.33 ms | 3.9% | 1.00× [0.93–1.10] | 231 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x1 | 2.69 ms | 6.1% | **1.63× [1.47–1.79]** | 372 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.28 ms | 2.5% | **1.92× [1.56–2.07]** | 439 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x2 | 1.35 ms | 4.1% | **3.20× [2.94–3.41]** | 742 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.31 ms | 4.8% | **1.83× [1.10–2.01]** | 433 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x2 | 1.52 ms | 7.7% | **2.77× [2.27–3.17]** | 660 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.56 ms | 3.2% | **2.64× [2.11–2.89]** | 639 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x4 | 1.03 ms | 12.5% | **4.08× [2.97–5.21]** | 969 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.18 ms | 2.8% | **1.95× [1.31–2.11]** | 458 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x4 | 1.39 ms | 3.8% | **3.06× [2.29–3.36]** | 718 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.54 ms | 2.2% | baseline | 393 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.63 ms | 4.5% | 0.98× [0.94–1.02] | 380 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.29 ms | 3.8% | baseline | 233 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.36 ms | 3.0% | 0.99× [0.96–1.04] | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.54 ms | 2.9% | baseline | 394 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.48 ms | 1.2% | 1.01× [0.99–1.02] | 403 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.72 ms | 3.8% | baseline | 212 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.47 ms | 3.2% | 1.07× [0.95–1.11] | 224 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.59 ms | 4.2% | baseline | 630 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.38 ms | 4.1% | **1.14× [1.04–1.23]** | 723 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.31 ms | 1.1% | baseline | 432 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.36 ms | 0.5% | 0.98× [0.96–1.01] | 423 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.36 ms | 3.5% | baseline | 737 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.36 ms | 2.7% | 0.99× [0.92–1.13] | 737 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.32 ms | 5.1% | baseline | 431 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.21 ms | 3.1% | 1.05× [0.94–1.09] | 452 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.06 ms | 7.0% | baseline | 942 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.04 ms | 10.2% | 1.05× [0.90–1.15] | 959 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.62 ms | 7.1% | baseline | 616 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.57 ms | 2.1% | 1.06× [0.98–1.61] | 637 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.37 ms | 4.4% | baseline | 730 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.36 ms | 3.7% | 1.00× [0.93–1.06] | 734 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.11 ms | 1.8% | baseline | 473 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.11 ms | 1.8% | 1.00× [0.88–1.02] | 474 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## RowWork/Streaming/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 580 ns | 3.5% | baseline | 1.72 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 621 ns | 4.0% | 0.97× [0.90–1.02] | 1.61 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 1.75 µs | 0.6% | **0.33× [0.32–0.43]** | 570 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 2.06 µs | 0.5% | **0.28× [0.27–0.36]** | 485 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 1.76 µs | 0.9% | **0.33× [0.32–0.42]** | 567 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 651 ns | 8.4% | 0.92× [0.80–1.02] | 1.54 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 1.76 µs | 0.6% | **0.33× [0.32–0.43]** | 569 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 39.8 µs | 106.1% | **0.02× [0.01–0.41]** | 25.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 1.77 µs | 0.3% | **0.33× [0.32–0.42]** | 566 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 14.9 µs | 47.8% | **0.05× [0.02–0.06]** | 66.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 1.75 µs | 0.6% | **0.33× [0.32–0.42]** | 572 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 88.1 µs | 17.1% | **0.01× [0.01–0.05]** | 11.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 1.75 µs | 0.6% | **0.33× [0.32–0.43]** | 570 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 16.4 µs | 12.9% | **0.04× [0.01–0.04]** | 61.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.05 µs | 5.9% | baseline | 487 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| ECS | 3.31 µs | 7.5% | **0.61× [0.27–0.73]** | 302 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x1 | 4.26 µs | 3.4% | **0.47× [0.24–0.51]** | 235 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x1 | 4.57 µs | 7.1% | **0.45× [0.26–0.52]** | 219 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x1 | 3.31 µs | 5.1% | **0.62× [0.33–0.72]** | 302 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x1 | 3.43 µs | 6.4% | **0.60× [0.29–0.71]** | 292 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x2 | 11.8 µs | 10.9% | **0.16× [0.13–0.19]** | 84.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x2 | 14.9 µs | 37.2% | **0.14× [0.03–0.22]** | 67.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x2 | 29.7 µs | 11.1% | **0.07× [0.03–0.09]** | 33.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x2 | 22.4 µs | 11.1% | **0.09× [0.07–0.14]** | 44.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x4 | 51.4 µs | 43.4% | **0.04× [0.02–0.11]** | 19.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x4 | 122 µs | 19.4% | **0.02× [0.01–0.03]** | 8.18 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x4 | 35.8 µs | 18.8% | **0.06× [0.02–0.07]** | 27.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x4 | 41.8 µs | 15.6% | **0.05× [0.02–0.06]** | 23.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |

## RowWork/StagedNeighbors/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.6 µs | 0.5% | baseline | 60.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| ECS | 16.6 µs | 0.4% | 1.00× [0.99–1.01] | 60.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x1 | 16.6 µs | 0.2% | 1.00× [0.99–1.01] | 60.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x1 | 16.5 µs | 0.4% | 1.00× [0.99–1.02] | 60.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x1 | 16.6 µs | 0.2% | 1.00× [0.46–1.00] | 60.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x1 | 16.8 µs | 0.4% | 0.99× [0.98–1.00] | 59.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x2 | 16.7 µs | 0.5% | 1.00× [0.43–1.01] | 60 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x2 | 19.2 µs | 6.5% | **0.86× [0.71–0.94]** | 52.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x2 | 16.6 µs | 0.3% | 1.00× [0.96–1.00] | 60.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x2 | 35.7 µs | 6.2% | **0.47× [0.31–0.55]** | 28 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x4 | 16.7 µs | 0.2% | 0.99× [0.99–1.00] | 60 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x4 | 22.4 µs | 2.8% | **0.75× [0.51–0.87]** | 44.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x4 | 16.6 µs | 0.5% | 1.00× [0.99–1.00] | 60.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x4 | 41.1 µs | 6.5% | **0.41× [0.36–0.60]** | 24.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |

## RowWork/Streaming/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.9 µs | 1.8% | baseline | 91.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 11 µs | 2.7% | 0.98× [0.95–1.04] | 90.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 41.1 µs | 1.5% | **0.27× [0.25–0.37]** | 24.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 49.2 µs | 1.6% | **0.22× [0.15–0.32]** | 20.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 11.2 µs | 1.7% | 0.96× [0.87–1.01] | 89.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 11.4 µs | 2.0% | 0.96× [0.95–1.11] | 88 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 58.5 µs | 25.7% | **0.19× [0.14–0.95]** | 17.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 28.3 µs | 4.1% | **0.39× [0.25–0.55]** | 35.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 39.9 µs | 6.7% | **0.28× [0.21–0.33]** | 25.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 32.2 µs | 9.2% | **0.34× [0.16–0.40]** | 31 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 94 µs | 28.7% | **0.13× [0.10–0.49]** | 10.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 115 µs | 4.6% | **0.10× [0.07–0.36]** | 8.71 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 38.5 µs | 7.4% | **0.30× [0.26–0.38]** | 26 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 42.4 µs | 14.8% | **0.26× [0.21–0.43]** | 23.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 33.2 µs | 16.7% | baseline | 30.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| ECS | 52.4 µs | 9.1% | **0.57× [0.34–0.70]** | 19.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x1 | 72.2 µs | 14.2% | **0.41× [0.29–0.52]** | 13.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x1 | 70 µs | 9.4% | **0.42× [0.31–0.53]** | 14.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x1 | 52.3 µs | 9.9% | **0.57× [0.33–0.73]** | 19.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x1 | 54.5 µs | 11.9% | **0.56× [0.35–0.71]** | 18.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x2 | 65.6 µs | 10.8% | **0.47× [0.24–0.62]** | 15.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x2 | 109 µs | 10.4% | **0.28× [0.25–0.46]** | 9.15 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x2 | 82.8 µs | 11.2% | **0.37× [0.26–0.44]** | 12.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x2 | 82.8 µs | 10.0% | **0.37× [0.29–0.45]** | 12.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x4 | 85.1 µs | 30.9% | **0.42× [0.22–0.56]** | 11.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x4 | 96.7 µs | 30.2% | **0.32× [0.16–0.44]** | 10.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x4 | 111 µs | 11.4% | **0.29× [0.23–0.36]** | 8.99 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x4 | 89.8 µs | 19.7% | **0.36× [0.22–0.48]** | 11.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |

## RowWork/StagedNeighbors/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 269 µs | 1.4% | baseline | 3.72 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| ECS | 268 µs | 0.6% | 1.00× [0.99–1.15] | 3.73 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x1 | 283 µs | 6.2% | 0.96× [0.80–1.09] | 3.53 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x1 | 268 µs | 0.6% | 1.01× [0.94–1.15] | 3.74 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x1 | 270 µs | 2.3% | 0.99× [0.79–1.15] | 3.71 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x1 | 269 µs | 1.7% | 1.00× [0.75–1.08] | 3.72 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x2 | 164 µs | 3.6% | **1.68× [1.53–1.80]** | 6.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x2 | 197 µs | 5.4% | **1.41× [1.06–1.74]** | 5.07 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x2 | 206 µs | 13.5% | 1.34× [0.75–1.65] | 4.86 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x2 | 205 µs | 6.1% | **1.34× [1.03–1.55]** | 4.87 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x4 | 114 µs | 5.5% | **2.35× [1.38–2.78]** | 8.75 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x4 | 128 µs | 4.0% | **2.16× [1.76–2.30]** | 7.81 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x4 | 176 µs | 3.1% | **1.56× [1.30–1.78]** | 5.69 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x4 | 183 µs | 3.7% | **1.49× [1.11–1.72]** | 5.48 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |

## Stack/Admission/1

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.78 ns | 1.2% | baseline | 209 M | commands_consumed=4.616e+05 |
| Domain | 21.3 ns | 1.9% | **0.23× [0.20–0.24]** | 47 M | commands_consumed=4.588e+05 |
| Wiring | 4.78 ns | 1.0% | 1.00× [0.76–1.11] | 209 M | commands_consumed=4.616e+05 |

## Stack/Admission/8

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 8.78 ns | 3.9% | baseline | 114 M | commands_consumed=3.548e+06 |
| Domain | 24.8 ns | 0.5% | **0.34× [0.33–0.37]** | 40.3 M | commands_consumed=3.513e+06 |
| Wiring | 8.95 ns | 4.0% | 0.96× [0.83–1.11] | 112 M | commands_consumed=3.533e+06 |

## Stack/Boundary1/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 110 ns | 0.0% | baseline | 9.09 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.2 µs | 6.2% | **0.10× [0.08–0.11]** | 835 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.15 µs | 5.1% | **0.10× [0.08–0.12]** | 868 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 846 ns | 3.5% | **0.14× [0.13–0.23]** | 1.18 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 802 ns | 2.0% | **0.14× [0.13–0.20]** | 1.25 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 455 ns | 4.5% | **0.26× [0.24–0.44]** | 2.2 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 415 ns | 7.5% | **0.29× [0.26–0.73]** | 2.41 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary2/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 130 ns | 10.8% | baseline | 7.69 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.22 µs | 7.5% | **0.10× [0.09–0.12]** | 818 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.1 µs | 2.4% | **0.11× [0.08–0.12]** | 907 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 841 ns | 3.7% | **0.15× [0.12–0.16]** | 1.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 821 ns | 3.1% | **0.15× [0.14–0.16]** | 1.22 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 411 ns | 2.4% | **0.32× [0.27–0.37]** | 2.43 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 405 ns | 3.6% | **0.32× [0.26–0.33]** | 2.47 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary4/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 9.1% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.15 µs | 0.9% | **0.10× [0.09–0.11]** | 868 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.13 µs | 0.9% | **0.10× [0.10–0.11]** | 884 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 871 ns | 2.2% | **0.13× [0.13–0.16]** | 1.15 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 791 ns | 2.5% | **0.15× [0.14–0.16]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 396 ns | 1.2% | **0.30× [0.28–0.33]** | 2.53 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 391 ns | 0.3% | **0.31× [0.28–0.38]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary1/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.72 µs | 3.7% | baseline | 175 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 355 µs | 1.7% | **0.02× [0.01–0.02]** | 2.82 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 363 µs | 3.3% | **0.02× [0.01–0.02]** | 2.76 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 198 µs | 1.1% | **0.03× [0.02–0.03]** | 5.04 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 203 µs | 6.0% | **0.03× [0.02–0.03]** | 4.93 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 15.9 µs | 2.8% | **0.37× [0.35–0.39]** | 62.9 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 15.1 µs | 2.7% | **0.38× [0.36–0.40]** | 66.1 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary2/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.75 µs | 2.9% | baseline | 174 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 415 µs | 5.0% | **0.01× [0.01–0.02]** | 2.41 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 384 µs | 3.4% | **0.02× [0.01–0.02]** | 2.6 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 239 µs | 4.3% | **0.02× [0.02–0.03]** | 4.19 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 231 µs | 1.9% | **0.02× [0.02–0.04]** | 4.32 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 42.4 µs | 4.9% | **0.14× [0.12–0.19]** | 23.6 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 44.4 µs | 5.7% | **0.13× [0.11–0.20]** | 22.5 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary4/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.1 µs | 5.7% | baseline | 164 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 402 µs | 3.4% | **0.02× [0.01–0.02]** | 2.49 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 383 µs | 3.7% | **0.02× [0.01–0.02]** | 2.61 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 239 µs | 1.1% | **0.03× [0.02–0.03]** | 4.19 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 235 µs | 3.0% | **0.03× [0.02–0.03]** | 4.25 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 50.9 µs | 12.9% | **0.11× [0.08–0.18]** | 19.7 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 53.5 µs | 5.2% | **0.12× [0.10–0.15]** | 18.7 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary1/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 138 µs | 1.1% | baseline | 7.27 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.29 ms | 1.3% | **0.02× [0.02–0.02]** | 137 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.24 ms | 1.4% | **0.02× [0.02–0.02]** | 138 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.76 ms | 2.0% | **0.03× [0.03–0.03]** | 210 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.55 ms | 1.8% | **0.03× [0.03–0.03]** | 220 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 431 µs | 3.2% | **0.32× [0.30–0.42]** | 2.32 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 446 µs | 4.8% | **0.31× [0.28–0.36]** | 2.24 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary2/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 142 µs | 1.2% | baseline | 7.04 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 8.03 ms | 5.8% | **0.02× [0.02–0.02]** | 124 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 8.24 ms | 6.3% | **0.02× [0.01–0.02]** | 121 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 5.18 ms | 7.2% | **0.03× [0.02–0.03]** | 193 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 5.03 ms | 5.3% | **0.03× [0.02–0.03]** | 199 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 511 µs | 8.5% | **0.28× [0.19–0.31]** | 1.96 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 492 µs | 6.9% | **0.29× [0.25–0.31]** | 2.03 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary4/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 167 µs | 18.2% | baseline | 5.99 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.99 ms | 4.3% | **0.02× [0.02–0.03]** | 125 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.77 ms | 7.8% | **0.02× [0.02–0.03]** | 129 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.78 ms | 5.3% | **0.03× [0.03–0.05]** | 209 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 5.03 ms | 10.9% | **0.03× [0.03–0.05]** | 199 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 479 µs | 16.4% | **0.33× [0.28–0.48]** | 2.09 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 519 µs | 22.2% | **0.33× [0.24–0.46]** | 1.93 k | state_checksum=3.652e+10, completed_ticks=17 |

