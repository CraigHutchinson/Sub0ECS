# Benchmark run 20261010-163149-185f5c6-r4-current

- stack: ok in 2 s
- nbody: ok in 2 s
- row-workloads: ok in 0 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.7% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.2% | 1.00× [0.44–1.01] | 64.1 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.5 µs | 0.5% | **0.96× [0.94–0.97]** | 60.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 0.3% | **0.96× [0.95–0.97]** | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.5 µs | 0.4% | **0.95× [0.79–0.98]** | 60.6 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.4 µs | 0.4% | **0.96× [0.94–0.98]** | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.5 µs | 0.2% | **0.96× [0.95–0.97]** | 60.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.4 µs | 0.5% | **0.96× [0.95–0.97]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.4% | **0.96× [0.94–0.98]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.5 µs | 0.6% | **0.96× [0.94–0.98]** | 60.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.5 µs | 0.5% | **0.96× [0.95–0.98]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.4 µs | 0.5% | **0.96× [0.94–0.98]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.4 µs | 0.4% | **0.96× [0.94–0.98]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.4 µs | 0.4% | **0.96× [0.94–0.98]** | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/Compact/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.4% | baseline | 63.6 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| ECS | 15.5 µs | 0.2% | **1.02× [1.01–1.03]** | 64.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactECS | 10.1 µs | 0.7% | **1.57× [1.52–1.60]** | 99.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactHandWritten | 11.4 µs | 0.3% | **1.38× [1.36–1.40]** | 88 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.3 µs | 0.4% | **0.97× [0.96–0.98]** | 61.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x1 | 12.1 µs | 0.3% | **1.30× [1.29–1.31]** | 82.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.3 µs | 0.3% | **0.97× [0.83–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x1 | 12 µs | 0.2% | **1.31× [1.30–1.33]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.3 µs | 0.5% | **0.97× [0.96–0.98]** | 61.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x2 | 12.2 µs | 0.2% | 1.29× [0.62–1.31] | 82.3 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.2 µs | 0.2% | **0.97× [0.96–1.00]** | 61.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x2 | 12 µs | 0.2% | **1.31× [1.29–1.33]** | 83.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.3 µs | 0.3% | **0.97× [0.95–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x4 | 12 µs | 0.2% | **1.30× [1.30–1.33]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.3 µs | 0.2% | **0.97× [0.96–0.98]** | 61.5 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x4 | 12 µs | 0.3% | **1.31× [1.30–1.33]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.8% | baseline | 94.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.7% | **0.88× [0.87–0.89]** | 83.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.6 µs | 0.5% | baseline | 60.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.5% | **1.02× [1.01–3.46]** | 61 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.3% | baseline | 94.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12.1 µs | 0.4% | **0.87× [0.87–0.88]** | 82.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.2% | **0.94× [0.93–0.94]** | 61.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.3% | baseline | 94.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.1% | **0.88× [0.88–0.88]** | 83.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.5 µs | 0.7% | baseline | 60.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.3% | 1.01× [1.00–1.02] | 61.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.4% | baseline | 95.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.1% | **0.89× [0.88–0.89]** | 84.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.3% | **0.94× [0.93–0.95]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.4% | baseline | 95.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.3% | **0.88× [0.88–0.89]** | 83.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.2 µs | 0.4% | baseline | 61.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | **1.00× [0.99–1.00]** | 61.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.6 µs | 0.3% | baseline | 94.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.3% | **0.89× [0.88–0.89]** | 83.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.5% | **0.94× [0.94–0.95]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.37 ms | 4.0% | baseline | 229 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.3 ms | 4.5% | 1.02× [0.95–1.10] | 233 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.21 ms | 2.2% | 1.02× [0.96–1.10] | 238 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.28 ms | 4.0% | 1.02× [0.97–1.10] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.31 ms | 3.9% | 1.02× [0.91–1.12] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.3 ms | 3.9% | 1.02× [0.88–1.12] | 232 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.22 ms | 3.8% | 1.03× [0.94–1.09] | 237 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.39 ms | 4.5% | 1.00× [0.94–1.09] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.19 ms | 3.4% | **1.97× [1.86–2.07]** | 457 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.33 ms | 3.9% | **1.83× [1.71–2.02]** | 428 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.32 ms | 3.3% | 1.00× [0.94–1.12] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.27 ms | 2.9% | 1.02× [0.94–1.08] | 234 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.34 ms | 13.8% | **3.26× [2.63–3.99]** | 748 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.1 ms | 5.4% | **2.07× [1.88–3.79]** | 477 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/Compact/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.45 ms | 3.7% | baseline | 225 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.61 ms | 8.2% | 0.98× [0.79–1.08] | 217 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactECS | 2.79 ms | 6.1% | **1.60× [1.35–1.92]** | 358 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactHandWritten | 2.99 ms | 8.2% | **1.50× [1.17–1.90]** | 334 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.52 ms | 4.7% | 0.99× [0.88–1.14] | 221 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x1 | 2.98 ms | 5.9% | **1.48× [1.30–1.79]** | 336 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.52 ms | 4.3% | 0.95× [0.75–1.10] | 221 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x1 | 2.86 ms | 4.8% | **1.60× [1.37–1.82]** | 349 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 4.07 ms | 2.2% | **1.12× [1.02–2.07]** | 246 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x2 | 1.46 ms | 4.3% | **3.05× [2.71–3.33]** | 686 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.36 ms | 7.8% | **1.84× [1.06–2.17]** | 423 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x2 | 1.45 ms | 7.8% | **3.03× [1.69–3.36]** | 691 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.59 ms | 1.5% | **2.81× [2.16–3.38]** | 631 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x4 | 1.04 ms | 3.2% | **4.28× [3.15–6.08]** | 961 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.1 ms | 2.0% | **2.05× [1.68–3.31]** | 477 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x4 | 1.37 ms | 0.6% | **3.24× [2.86–4.98]** | 731 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 3.01 ms | 4.5% | baseline | 332 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 3.02 ms | 4.2% | 1.02× [0.91–1.07] | 331 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.41 ms | 6.5% | baseline | 227 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.37 ms | 4.6% | 0.97× [0.90–1.04] | 229 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.99 ms | 7.1% | baseline | 334 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 3.02 ms | 6.2% | 0.99× [0.95–1.22] | 332 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.12 ms | 1.2% | baseline | 243 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.24 ms | 3.4% | 1.00× [0.94–1.03] | 236 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.61 ms | 0.7% | baseline | 383 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.36 ms | 4.3% | **1.92× [1.78–2.05]** | 737 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.13 ms | 4.1% | baseline | 471 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.13 ms | 2.1% | 1.00× [0.98–1.12] | 470 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.42 ms | 5.4% | baseline | 704 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.38 ms | 6.1% | 1.07× [0.95–1.18] | 724 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.21 ms | 3.8% | baseline | 453 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.16 ms | 2.7% | 1.00× [0.94–1.06] | 462 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1 ms | 2.5% | baseline | 999 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.03 ms | 4.6% | 0.93× [0.79–1.05] | 967 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.55 ms | 1.0% | baseline | 646 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.57 ms | 2.4% | 0.99× [0.97–1.00] | 636 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.35 ms | 0.6% | baseline | 743 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.35 ms | 1.1% | 0.99× [0.98–1.01] | 740 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.1 ms | 0.9% | baseline | 475 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.07 ms | 0.7% | **1.02× [1.00–1.03]** | 483 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## RowWork/Streaming/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 776 ns | 5.7% | baseline | 1.29 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 782 ns | 1.3% | 0.96× [0.78–1.12] | 1.28 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 2.56 µs | 1.8% | **0.30× [0.27–0.39]** | 391 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 3.16 µs | 4.0% | **0.25× [0.23–0.32]** | 316 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 3.07 µs | 7.5% | **0.28× [0.23–0.39]** | 326 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 846 ns | 3.6% | 0.91× [0.88–1.04] | 1.18 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 2.54 µs | 1.6% | **0.30× [0.28–0.39]** | 393 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 44.3 µs | 137.9% | **0.03× [0.01–0.17]** | 22.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 2.61 µs | 3.4% | **0.30× [0.28–0.39]** | 383 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 12.5 µs | 6.5% | **0.06× [0.02–0.06]** | 79.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 2.58 µs | 2.8% | **0.31× [0.28–0.36]** | 388 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 76.6 µs | 13.0% | **0.01× [0.01–0.05]** | 13.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 2.53 µs | 1.2% | **0.31× [0.28–0.39]** | 395 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 35.1 µs | 57.4% | **0.02× [0.01–0.04]** | 28.5 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 3.14 µs | 5.9% | baseline | 319 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| ECS | 5.72 µs | 16.6% | **0.56× [0.35–0.71]** | 175 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x1 | 7.23 µs | 13.0% | **0.45× [0.31–0.52]** | 138 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x1 | 7.11 µs | 12.3% | **0.43× [0.27–0.52]** | 141 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x1 | 5.79 µs | 19.9% | **0.53× [0.28–0.70]** | 173 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x1 | 5.43 µs | 17.3% | **0.55× [0.39–0.70]** | 184 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x2 | 19.6 µs | 41.5% | **0.16× [0.03–0.28]** | 50.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x2 | 22.3 µs | 48.2% | **0.14× [0.03–0.27]** | 44.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x2 | 18.2 µs | 19.3% | **0.16× [0.08–0.19]** | 55 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x2 | 28.5 µs | 25.2% | **0.11× [0.09–0.18]** | 35.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x4 | 115 µs | 2.5% | **0.03× [0.02–0.03]** | 8.72 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x4 | 120 µs | 5.3% | **0.03× [0.02–0.09]** | 8.32 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x4 | 46 µs | 16.4% | **0.07× [0.03–0.10]** | 21.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x4 | 30.7 µs | 18.5% | **0.10× [0.08–0.17]** | 32.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |

## RowWork/StagedNeighbors/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.6 µs | 0.9% | baseline | 60.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| ECS | 16.4 µs | 0.3% | 1.00× [0.99–1.02] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x1 | 16.4 µs | 0.1% | 1.01× [0.99–1.02] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x1 | 16.6 µs | 0.2% | 0.99× [0.96–1.01] | 60.4 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x1 | 16.5 µs | 0.3% | 1.01× [0.99–1.01] | 60.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x1 | 16.9 µs | 0.3% | **0.98× [0.97–1.00]** | 59.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x2 | 16.4 µs | 0.2% | 1.01× [0.99–1.02] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x2 | 18.2 µs | 2.4% | **0.91× [0.44–0.96]** | 55 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x2 | 16.5 µs | 0.2% | 1.00× [0.99–1.02] | 60.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x2 | 42.4 µs | 8.1% | **0.39× [0.30–0.42]** | 23.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x4 | 16.7 µs | 0.2% | 1.00× [0.98–1.00] | 60 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x4 | 32.4 µs | 4.8% | **0.51× [0.23–0.71]** | 30.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x4 | 16.7 µs | 1.4% | 0.99× [0.53–1.01] | 59.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x4 | 37.6 µs | 9.6% | **0.45× [0.36–0.51]** | 26.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |

## RowWork/Streaming/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.4 µs | 3.5% | baseline | 95.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 10.5 µs | 1.7% | 0.99× [0.51–1.03] | 95.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 42.4 µs | 4.5% | **0.25× [0.16–0.34]** | 23.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 48 µs | 0.3% | **0.21× [0.19–0.31]** | 20.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 11 µs | 3.8% | 0.96× [0.88–1.06] | 90.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 11 µs | 3.6% | **0.95× [0.89–1.00]** | 90.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 15.2 µs | 5.0% | **0.70× [0.62–0.81]** | 66 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 68.3 µs | 18.5% | **0.17× [0.13–0.50]** | 14.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 37.4 µs | 9.5% | **0.28× [0.24–0.33]** | 26.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 33.5 µs | 9.8% | **0.31× [0.25–0.34]** | 29.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 54.1 µs | 15.4% | **0.20× [0.15–0.47]** | 18.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 38.9 µs | 34.8% | **0.32× [0.08–0.40]** | 25.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 39.3 µs | 7.7% | **0.27× [0.22–0.40]** | 25.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 36.4 µs | 3.9% | **0.29× [0.25–0.33]** | 27.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 47.4 µs | 20.5% | baseline | 21.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| ECS | 55.5 µs | 8.8% | 0.73× [0.40–1.58] | 18 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x1 | 73.3 µs | 17.5% | 0.49× [0.37–1.18] | 13.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x1 | 76.1 µs | 17.3% | 0.48× [0.36–1.05] | 13.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x1 | 57.6 µs | 8.9% | 0.77× [0.41–1.45] | 17.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x1 | 62.7 µs | 17.1% | **0.65× [0.45–0.87]** | 16 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x2 | 66.9 µs | 7.6% | 0.69× [0.29–1.29] | 15 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x2 | 82.5 µs | 28.4% | 0.48× [0.39–1.23] | 12.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x2 | 85.9 µs | 16.8% | 0.43× [0.30–1.03] | 11.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x2 | 111 µs | 27.2% | 0.41× [0.20–1.01] | 9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x4 | 92.4 µs | 9.2% | **0.52× [0.30–0.84]** | 10.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x4 | 99.7 µs | 9.5% | **0.42× [0.26–0.54]** | 10 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x4 | 96.6 µs | 24.3% | 0.44× [0.29–1.05] | 10.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x4 | 101 µs | 9.1% | **0.43× [0.28–0.90]** | 9.86 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |

## RowWork/StagedNeighbors/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 276 µs | 4.0% | baseline | 3.63 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| ECS | 280 µs | 4.8% | 1.00× [0.90–1.38] | 3.57 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x1 | 277 µs | 4.1% | 1.01× [0.87–1.33] | 3.61 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x1 | 271 µs | 0.7% | 0.99× [0.85–1.38] | 3.69 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x1 | 274 µs | 2.5% | 1.02× [0.88–1.28] | 3.65 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x1 | 279 µs | 3.9% | 1.00× [0.95–1.37] | 3.59 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x2 | 172 µs | 4.4% | **1.62× [1.38–2.12]** | 5.83 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x2 | 195 µs | 6.3% | **1.49× [1.21–2.19]** | 5.12 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x2 | 207 µs | 8.3% | **1.41× [1.04–1.50]** | 4.84 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x2 | 304 µs | 23.8% | 1.15× [0.78–1.69] | 3.29 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x4 | 147 µs | 10.4% | 1.93× [0.85–2.73] | 6.79 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x4 | 156 µs | 9.7% | **1.75× [1.50–2.08]** | 6.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x4 | 194 µs | 6.2% | 1.44× [0.97–2.37] | 5.16 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x4 | 203 µs | 10.8% | 1.40× [0.91–1.88] | 4.92 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |

## Stack/Admission/1

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.02 ns | 5.8% | baseline | 199 M | commands_consumed=3.67e+05 |
| Domain | 23 ns | 7.3% | **0.24× [0.16–0.27]** | 43.5 M | commands_consumed=3.651e+05 |
| Wiring | 4.78 ns | 1.2% | 1.02× [0.85–1.36] | 209 M | commands_consumed=3.658e+05 |

## Stack/Admission/8

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 9.1 ns | 7.5% | baseline | 110 M | commands_consumed=2.751e+06 |
| Domain | 25.4 ns | 3.7% | **0.36× [0.31–0.44]** | 39.3 M | commands_consumed=2.731e+06 |
| Wiring | 9.41 ns | 12.2% | 0.96× [0.76–1.11] | 106 M | commands_consumed=2.741e+06 |

## Stack/Boundary1/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 121 ns | 0.8% | baseline | 8.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.23 µs | 7.3% | **0.10× [0.09–0.11]** | 812 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.11 µs | 3.6% | **0.11× [0.10–0.12]** | 899 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 841 ns | 3.7% | **0.15× [0.13–0.16]** | 1.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 796 ns | 1.3% | **0.16× [0.15–0.17]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 411 ns | 2.4% | **0.30× [0.29–0.37]** | 2.43 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 400 ns | 2.3% | **0.31× [0.29–0.36]** | 2.5 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary2/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 0.0% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.17 µs | 2.2% | **0.10× [0.08–0.11]** | 857 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.12 µs | 4.4% | **0.11× [0.10–0.12]** | 891 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 846 ns | 3.0% | **0.14× [0.12–0.15]** | 1.18 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 791 ns | 4.7% | **0.15× [0.11–0.16]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 420 ns | 2.4% | **0.29× [0.28–0.32]** | 2.38 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 396 ns | 4.1% | **0.31× [0.28–0.34]** | 2.53 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary4/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 8.6% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.18 µs | 1.3% | **0.10× [0.09–0.10]** | 850 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.09 µs | 1.8% | **0.11× [0.09–0.12]** | 916 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 837 ns | 3.2% | **0.14× [0.12–0.15]** | 1.19 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 796 ns | 1.9% | **0.15× [0.12–0.16]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 420 ns | 4.9% | **0.29× [0.27–0.33]** | 2.38 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 406 ns | 5.2% | **0.29× [0.27–0.34]** | 2.47 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary1/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 5.98 µs | 4.9% | baseline | 167 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 398 µs | 13.0% | **0.02× [0.01–0.02]** | 2.51 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 395 µs | 18.0% | **0.02× [0.01–0.02]** | 2.53 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 207 µs | 4.5% | **0.03× [0.02–0.04]** | 4.83 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 196 µs | 0.8% | **0.03× [0.02–0.04]** | 5.11 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 16.1 µs | 3.1% | **0.38× [0.35–0.45]** | 62.3 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 14.7 µs | 2.2% | **0.40× [0.38–0.53]** | 68 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary2/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.58 µs | 7.8% | baseline | 152 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 409 µs | 6.3% | **0.02× [0.01–0.02]** | 2.44 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 388 µs | 7.3% | **0.02× [0.01–0.02]** | 2.57 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 251 µs | 7.5% | **0.03× [0.02–0.03]** | 3.98 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 248 µs | 10.0% | **0.03× [0.02–0.03]** | 4.04 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 52.1 µs | 11.4% | **0.13× [0.08–0.17]** | 19.2 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 50.7 µs | 5.3% | **0.12× [0.10–0.16]** | 19.7 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary4/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.19 µs | 4.9% | baseline | 162 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 426 µs | 8.6% | **0.01× [0.01–0.02]** | 2.35 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 363 µs | 1.6% | **0.02× [0.02–0.02]** | 2.76 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 236 µs | 2.3% | **0.03× [0.02–0.03]** | 4.24 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 234 µs | 4.1% | **0.03× [0.02–0.03]** | 4.27 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 46.1 µs | 7.3% | **0.13× [0.12–0.16]** | 21.7 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 46.1 µs | 7.5% | **0.13× [0.09–0.17]** | 21.7 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary1/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 143 µs | 1.8% | baseline | 7.01 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.41 ms | 2.8% | **0.02× [0.02–0.03]** | 135 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.24 ms | 1.1% | **0.02× [0.02–0.03]** | 138 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.71 ms | 3.1% | **0.03× [0.03–0.05]** | 212 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.63 ms | 3.0% | **0.03× [0.03–0.05]** | 216 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 424 µs | 7.1% | **0.35× [0.27–0.51]** | 2.36 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 468 µs | 5.3% | **0.32× [0.28–0.55]** | 2.14 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary2/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 142 µs | 1.7% | baseline | 7.04 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.52 ms | 2.0% | **0.02× [0.02–0.02]** | 133 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.4 ms | 1.9% | **0.02× [0.02–0.02]** | 135 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.7 ms | 2.9% | **0.03× [0.03–0.03]** | 213 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.58 ms | 1.6% | **0.03× [0.03–0.03]** | 218 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 460 µs | 6.1% | **0.32× [0.28–0.35]** | 2.17 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 486 µs | 7.4% | **0.29× [0.27–0.35]** | 2.06 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary4/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 152 µs | 8.8% | baseline | 6.57 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.64 ms | 5.6% | **0.02× [0.02–0.03]** | 131 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.48 ms | 5.6% | **0.02× [0.02–0.04]** | 134 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.97 ms | 4.4% | **0.03× [0.03–0.06]** | 201 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.9 ms | 7.3% | **0.03× [0.03–0.06]** | 204 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 456 µs | 10.9% | **0.35× [0.25–0.66]** | 2.19 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 481 µs | 9.3% | **0.36× [0.25–0.63]** | 2.08 k | state_checksum=3.652e+10, completed_ticks=17 |

