# Benchmark run 20261010-163154-185f5c6-r5-current

- stack: ok in 2 s
- nbody: ok in 3 s
- row-workloads: ok in 0 s

## NBody/Ordered/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.6 µs | 0.3% | baseline | 64 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.1% | **1.00× [1.00–1.01]** | 64.2 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native1 | 16.4 µs | 0.5% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline1 | 16.4 µs | 0.6% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.4 µs | 0.2% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.4 µs | 0.4% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native2 | 16.4 µs | 0.3% | **0.95× [0.94–0.97]** | 60.9 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline2 | 16.4 µs | 0.3% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.4% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.4 µs | 0.2% | **0.95× [0.94–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Native4 | 16.5 µs | 0.4% | **0.95× [0.94–0.96]** | 60.5 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| Pipeline4 | 16.4 µs | 0.5% | **0.95× [0.94–0.96]** | 61 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.4 µs | 0.2% | **0.95× [0.95–0.96]** | 60.8 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.5 µs | 0.4% | **0.95× [0.94–0.95]** | 60.7 k | state_checksum=475.4, completed_ticks=16, directed_interactions_per_tick=4032 |

## NBody/Compact/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.7 µs | 0.4% | baseline | 63.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| ECS | 15.6 µs | 0.5% | 1.00× [1.00–1.21] | 63.9 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactECS | 9.95 µs | 0.6% | **1.58× [1.52–1.86]** | 101 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactHandWritten | 11.2 µs | 0.7% | 1.40× [0.48–1.74] | 89.2 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x1 | 16.4 µs | 0.5% | 0.96× [0.95–1.16] | 60.9 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x1 | 12 µs | 0.7% | **1.31× [1.27–1.59]** | 83.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x1 | 16.4 µs | 0.6% | 0.96× [0.94–1.16] | 60.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x1 | 11.9 µs | 0.3% | **1.31× [1.30–1.61]** | 83.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x2 | 16.4 µs | 0.5% | 0.96× [0.94–1.17] | 60.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x2 | 11.9 µs | 0.2% | **1.31× [1.30–1.61]** | 83.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x2 | 16.4 µs | 0.3% | 0.96× [0.95–1.17] | 61 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x2 | 11.9 µs | 0.5% | **1.32× [1.30–1.60]** | 83.8 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| NativeG64x4 | 16.5 µs | 0.5% | 0.95× [0.95–1.17] | 60.7 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactNativeG64x4 | 12 µs | 0.5% | **1.31× [1.30–1.59]** | 83.4 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| PipelineG64x4 | 16.4 µs | 0.5% | 0.96× [0.93–1.17] | 61.1 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |
| CompactPipelineG64x4 | 11.9 µs | 0.4% | **1.32× [1.31–1.53]** | 83.9 k | state_checksum=475.4, completed_ticks=18, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.7 µs | 0.3% | baseline | 93.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 12 µs | 0.2% | **0.89× [0.89–0.90]** | 83.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.3 µs | 0.9% | baseline | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.3% | 1.00× [0.99–1.02] | 61.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.2% | baseline | 95.6 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.2% | **0.88× [0.88–0.89]** | 84.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline1/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.1% | **0.94× [0.94–0.94]** | 61.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.8% | baseline | 94.8 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.5% | **0.88× [0.88–0.90]** | 83.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.1 µs | 0.3% | baseline | 61.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.2 µs | 0.4% | 1.00× [0.99–1.00] | 61.7 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.4% | baseline | 95.1 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.3% | 0.89× [0.88–1.30] | 84.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline2/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.2% | baseline | 65.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.4 µs | 0.7% | **0.93× [0.92–0.94]** | 60.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.2% | baseline | 94.9 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.2% | **0.88× [0.88–0.89]** | 84 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedNative4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.6 µs | 0.3% | baseline | 60.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.6% | **1.02× [1.00–1.03]** | 61.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/CompactMatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 10.5 µs | 0.4% | baseline | 95.4 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 11.9 µs | 0.1% | **0.88× [0.88–0.89]** | 84.3 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/MatchedPipeline4/64

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 15.3 µs | 0.1% | baseline | 65.5 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |
| ECS | 16.3 µs | 0.2% | **0.93× [0.93–0.94]** | 61.2 k | state_checksum=475.4, completed_ticks=14, directed_interactions_per_tick=4032 |

## NBody/Ordered/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.48 ms | 3.9% | baseline | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.48 ms | 5.6% | 0.99× [0.89–1.09] | 223 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native1 | 4.39 ms | 4.0% | 1.00× [0.89–1.10] | 228 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline1 | 4.56 ms | 5.8% | 0.98× [0.94–1.08] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.49 ms | 3.1% | 1.01× [0.93–1.04] | 222 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.46 ms | 2.9% | 1.00× [0.95–1.08] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native2 | 4.46 ms | 4.1% | 0.99× [0.89–1.10] | 224 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline2 | 4.44 ms | 3.4% | 0.97× [0.90–1.09] | 225 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.44 ms | 6.9% | **1.80× [1.14–2.00]** | 410 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.66 ms | 19.4% | **1.77× [1.12–2.03]** | 376 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Native4 | 4.56 ms | 4.5% | 1.01× [0.89–1.04] | 219 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| Pipeline4 | 4.33 ms | 4.2% | 1.03× [0.92–1.13] | 231 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 1.61 ms | 1.9% | **2.77× [2.55–3.32]** | 622 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.09 ms | 6.4% | **2.17× [1.55–3.10]** | 477 | state_checksum=7971, completed_ticks=16, directed_interactions_per_tick=1.048e+06 |

## NBody/Compact/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.66 ms | 6.0% | baseline | 215 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.53 ms | 6.8% | 1.02× [0.87–1.28] | 221 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactECS | 2.7 ms | 4.7% | **1.69× [1.58–1.87]** | 370 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactHandWritten | 2.81 ms | 6.7% | **1.65× [1.47–2.01]** | 356 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x1 | 4.49 ms | 3.8% | 1.04× [0.91–1.21] | 223 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x1 | 2.85 ms | 10.1% | **1.63× [1.42–1.82]** | 351 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x1 | 4.53 ms | 4.8% | 0.99× [0.86–1.20] | 221 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x1 | 2.76 ms | 6.9% | **1.68× [1.34–1.81]** | 362 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x2 | 2.25 ms | 3.6% | **2.00× [1.66–2.36]** | 444 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x2 | 1.45 ms | 7.8% | **3.05× [1.81–3.55]** | 691 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x2 | 2.35 ms | 9.7% | **1.94× [1.09–2.32]** | 425 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x2 | 1.45 ms | 5.4% | **3.22× [1.78–3.77]** | 692 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| NativeG64x4 | 2.21 ms | 18.1% | **2.17× [1.68–3.38]** | 452 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactNativeG64x4 | 1.02 ms | 14.8% | **4.59× [2.21–7.11]** | 983 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| PipelineG64x4 | 2.17 ms | 3.7% | **2.09× [1.75–2.50]** | 461 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |
| CompactPipelineG64x4 | 1.42 ms | 3.5% | **3.34× [2.05–4.03]** | 705 | state_checksum=7971, completed_ticks=18, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.77 ms | 8.0% | baseline | 361 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 3.1 ms | 12.2% | **0.97× [0.95–1.00]** | 322 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.23 ms | 4.9% | baseline | 237 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.55 ms | 4.7% | 0.97× [0.89–1.04] | 220 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.77 ms | 5.6% | baseline | 362 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.89 ms | 4.7% | 0.92× [0.86–1.14] | 346 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline1/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.39 ms | 3.9% | baseline | 228 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.44 ms | 2.7% | 0.96× [0.86–1.02] | 225 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.46 ms | 4.4% | baseline | 687 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.44 ms | 2.1% | 1.04× [0.96–1.10] | 692 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.25 ms | 3.2% | baseline | 445 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.26 ms | 3.2% | 0.98× [0.92–1.08] | 442 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.45 ms | 5.5% | baseline | 691 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.57 ms | 10.3% | 0.95× [0.76–1.07] | 635 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline2/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.15 ms | 0.2% | baseline | 241 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 4.09 ms | 0.3% | **1.02× [1.01–1.02]** | 244 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1 ms | 4.7% | baseline | 996 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.01 ms | 7.2% | 1.04× [0.95–1.26] | 995 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedNative4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.53 ms | 0.7% | baseline | 654 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.58 ms | 2.2% | 0.98× [0.91–1.00] | 634 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/CompactMatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.3 ms | 1.4% | baseline | 770 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 1.33 ms | 2.9% | 0.98× [0.96–1.01] | 750 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## NBody/MatchedPipeline4/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 1.89 ms | 13.1% | baseline | 528 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |
| ECS | 2.15 ms | 3.8% | **0.81× [0.63–0.98]** | 466 | state_checksum=7971, completed_ticks=14, directed_interactions_per_tick=1.048e+06 |

## RowWork/Streaming/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 756 ns | 3.4% | baseline | 1.32 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 801 ns | 4.6% | 0.96× [0.87–1.17] | 1.25 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 2.6 µs | 3.1% | **0.29× [0.28–0.33]** | 385 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 3.1 µs | 3.2% | **0.24× [0.24–0.28]** | 322 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 2.58 µs | 2.9% | **0.29× [0.28–0.34]** | 387 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 836 ns | 3.7% | 0.90× [0.85–1.12] | 1.2 M | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 2.64 µs | 4.5% | **0.29× [0.23–0.35]** | 379 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 28 µs | 78.7% | **0.03× [0.01–0.26]** | 35.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 2.58 µs | 3.2% | **0.29× [0.27–0.34]** | 387 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 25.1 µs | 1.7% | **0.03× [0.03–0.04]** | 39.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 2.58 µs | 2.9% | **0.29× [0.28–0.34]** | 388 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 99 µs | 12.2% | **0.01× [0.01–0.01]** | 10.1 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 2.59 µs | 2.1% | **0.29× [0.29–0.35]** | 386 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 35.5 µs | 30.3% | **0.02× [0.01–0.04]** | 28.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 2.92 µs | 3.2% | baseline | 342 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| ECS | 4.81 µs | 7.3% | **0.62× [0.38–0.72]** | 208 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x1 | 7.04 µs | 6.5% | **0.42× [0.28–0.48]** | 142 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x1 | 6.66 µs | 7.3% | **0.44× [0.30–0.51]** | 150 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x1 | 5.28 µs | 6.3% | **0.55× [0.37–0.68]** | 189 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x1 | 5.09 µs | 13.4% | **0.57× [0.33–0.72]** | 196 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x2 | 15.2 µs | 28.3% | **0.19× [0.03–0.25]** | 65.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x2 | 97.9 µs | 8.2% | **0.03× [0.03–0.17]** | 10.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x2 | 30.6 µs | 22.7% | **0.10× [0.03–0.12]** | 32.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x2 | 19.9 µs | 32.3% | **0.15× [0.08–0.22]** | 50.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG1024x4 | 56.7 µs | 16.8% | **0.05× [0.03–0.13]** | 17.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| NativeG64x4 | 28.4 µs | 20.3% | **0.11× [0.06–0.13]** | 35.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG1024x4 | 41.9 µs | 13.2% | **0.07× [0.02–0.08]** | 23.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |
| PipelineG64x4 | 54.5 µs | 8.4% | **0.05× [0.03–0.06]** | 18.3 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=0, structural_toggles_completed=1024 |

## RowWork/StagedNeighbors/1024

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 16.4 µs | 0.3% | baseline | 61 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| ECS | 16.4 µs | 0.3% | 1.00× [0.97–1.02] | 60.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x1 | 16.4 µs | 0.3% | 1.00× [0.99–1.02] | 60.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x1 | 16.6 µs | 1.2% | 0.99× [0.97–1.01] | 60.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x1 | 16.5 µs | 0.4% | 1.00× [0.97–1.01] | 60.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x1 | 16.7 µs | 0.2% | **0.98× [0.96–0.98]** | 59.9 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x2 | 16.4 µs | 0.1% | 1.00× [0.98–1.01] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x2 | 17.1 µs | 4.6% | 0.96× [0.67–1.10] | 58.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x2 | 16.4 µs | 0.2% | 1.00× [0.99–1.02] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x2 | 42.1 µs | 11.7% | **0.40× [0.15–0.45]** | 23.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG1024x4 | 16.5 µs | 0.4% | 0.99× [0.43–1.02] | 60.8 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| NativeG64x4 | 58.1 µs | 14.3% | **0.29× [0.19–0.39]** | 17.2 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG1024x4 | 16.5 µs | 0.4% | 0.99× [0.95–1.02] | 60.7 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |
| PipelineG64x4 | 36.2 µs | 11.6% | **0.45× [0.20–0.53]** | 27.6 k | state_checksum=7851, completed_ticks=16, entities_per_tick=1024, neighbor_reads_per_tick=8192, structural_toggles_completed=0 |

## RowWork/Streaming/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 8.38 µs | 9.0% | baseline | 119 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| ECS | 8.36 µs | 5.1% | 0.98× [0.47–1.11] | 120 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x1 | 27.1 µs | 0.5% | **0.31× [0.28–0.67]** | 37 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x1 | 31.7 µs | 0.3% | **0.26× [0.24–0.57]** | 31.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x1 | 8.27 µs | 4.3% | 0.97× [0.89–1.23] | 121 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x1 | 8.34 µs | 7.0% | 0.95× [0.87–1.37] | 120 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x2 | 17.2 µs | 33.4% | **0.61× [0.21–0.90]** | 58 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x2 | 28.1 µs | 48.0% | **0.37× [0.13–0.47]** | 35.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x2 | 38.3 µs | 21.5% | **0.25× [0.13–0.31]** | 26.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x2 | 38.3 µs | 12.8% | **0.23× [0.10–0.27]** | 26.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG1024x4 | 35.6 µs | 49.3% | **0.35× [0.11–0.51]** | 28.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| NativeG64x4 | 86 µs | 20.7% | **0.10× [0.07–0.41]** | 11.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG1024x4 | 31.3 µs | 7.9% | **0.28× [0.22–0.58]** | 31.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |
| PipelineG64x4 | 33.9 µs | 9.9% | **0.23× [0.20–0.62]** | 29.5 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=0 |

## RowWork/FragmentedChurn/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 30.4 µs | 6.8% | baseline | 32.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| ECS | 53.7 µs | 13.9% | **0.61× [0.31–0.71]** | 18.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x1 | 99.9 µs | 4.2% | **0.30× [0.25–0.39]** | 10 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x1 | 72.5 µs | 11.7% | **0.46× [0.29–0.55]** | 13.8 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x1 | 70.9 µs | 19.0% | **0.45× [0.27–0.66]** | 14.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x1 | 77 µs | 13.4% | **0.41× [0.31–0.54]** | 13 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x2 | 85.6 µs | 9.2% | **0.36× [0.24–0.43]** | 11.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x2 | 74.8 µs | 9.5% | **0.41× [0.25–0.53]** | 13.4 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x2 | 96.9 µs | 11.0% | **0.34× [0.22–0.39]** | 10.3 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x2 | 99.2 µs | 14.0% | **0.32× [0.22–0.39]** | 10.1 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG1024x4 | 101 µs | 14.6% | **0.33× [0.22–0.39]** | 9.92 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| NativeG64x4 | 128 µs | 13.8% | **0.24× [0.16–0.31]** | 7.79 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG1024x4 | 94.2 µs | 9.5% | **0.31× [0.25–0.44]** | 10.6 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |
| PipelineG64x4 | 75.7 µs | 13.7% | **0.41× [0.28–0.49]** | 13.2 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=0, structural_toggles_completed=1.638e+04 |

## RowWork/StagedNeighbors/16384

| Design | time/tick | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 293 µs | 8.8% | baseline | 3.42 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| ECS | 295 µs | 9.1% | 0.99× [0.71–1.31] | 3.38 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x1 | 270 µs | 2.1% | 1.03× [0.97–1.19] | 3.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x1 | 274 µs | 3.1% | 1.09× [0.87–1.30] | 3.65 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x1 | 272 µs | 1.5% | 1.08× [0.99–1.31] | 3.67 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x1 | 270 µs | 1.2% | 1.09× [0.94–1.32] | 3.7 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x2 | 197 µs | 16.4% | **1.49× [1.34–2.14]** | 5.09 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x2 | 173 µs | 2.7% | **1.60× [1.34–2.12]** | 5.76 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x2 | 315 µs | 7.0% | 1.03× [0.78–1.54] | 3.18 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x2 | 210 µs | 10.8% | 1.41× [0.76–1.77] | 4.75 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG1024x4 | 175 µs | 15.9% | **1.73× [1.27–2.38]** | 5.71 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| NativeG64x4 | 156 µs | 16.6% | 1.81× [0.15–2.29] | 6.39 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG1024x4 | 214 µs | 7.7% | 1.31× [0.52–1.87] | 4.68 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |
| PipelineG64x4 | 204 µs | 12.3% | 1.46× [0.20–1.79] | 4.9 k | state_checksum=1.275e+05, completed_ticks=16, entities_per_tick=1.638e+04, neighbor_reads_per_tick=1.311e+05, structural_toggles_completed=0 |

## Stack/Admission/1

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 4.84 ns | 2.3% | baseline | 206 M | commands_consumed=4.616e+05 |
| Domain | 25.2 ns | 10.2% | **0.22× [0.18–0.50]** | 39.6 M | commands_consumed=4.586e+05 |
| Wiring | 5.69 ns | 18.5% | 0.91× [0.73–2.23] | 176 M | commands_consumed=4.608e+05 |

## Stack/Admission/8

| Design | time/batch | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 9.43 ns | 4.7% | baseline | 106 M | commands_consumed=3.14e+06 |
| Domain | 25.3 ns | 4.0% | **0.37× [0.33–0.41]** | 39.5 M | commands_consumed=3.099e+06 |
| Wiring | 9.17 ns | 8.2% | 0.96× [0.79–1.18] | 109 M | commands_consumed=3.135e+06 |

## Stack/Boundary1/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 110 ns | 0.5% | baseline | 9.05 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.21 µs | 5.3% | **0.09× [0.08–0.10]** | 829 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.15 µs | 1.7% | **0.10× [0.09–0.11]** | 872 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 836 ns | 1.8% | **0.13× [0.10–0.16]** | 1.2 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 832 ns | 3.6% | **0.14× [0.13–0.16]** | 1.2 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 401 ns | 2.2% | **0.28× [0.26–0.32]** | 2.49 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 390 ns | 2.6% | **0.29× [0.28–0.34]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary2/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 8.4% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.15 µs | 4.1% | **0.10× [0.06–0.11]** | 868 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.07 µs | 1.4% | **0.11× [0.10–0.13]** | 937 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 816 ns | 2.6% | **0.15× [0.12–0.16]** | 1.22 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 791 ns | 1.3% | **0.15× [0.14–0.17]** | 1.26 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 420 ns | 3.6% | **0.29× [0.21–0.35]** | 2.38 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 401 ns | 2.5% | **0.30× [0.27–0.33]** | 2.49 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary4/64

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 120 ns | 0.4% | baseline | 8.33 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainSearch | 1.14 µs | 1.3% | **0.11× [0.10–0.11]** | 880 k | state_checksum=4.855e+04, completed_ticks=17 |
| WiringSearch | 1.09 µs | 1.4% | **0.11× [0.10–0.12]** | 920 k | state_checksum=4.855e+04, completed_ticks=17 |
| DomainDense | 816 ns | 1.9% | **0.15× [0.13–0.15]** | 1.22 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringDense | 786 ns | 1.9% | **0.15× [0.14–0.16]** | 1.27 M | state_checksum=4.855e+04, completed_ticks=17 |
| DomainIndexed | 406 ns | 3.7% | **0.29× [0.28–0.31]** | 2.47 M | state_checksum=4.855e+04, completed_ticks=17 |
| WiringIndexed | 390 ns | 2.4% | **0.30× [0.28–0.33]** | 2.56 M | state_checksum=4.855e+04, completed_ticks=17 |

## Stack/Boundary1/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 7.52 µs | 13.7% | baseline | 133 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 359 µs | 0.8% | **0.02× [0.02–0.04]** | 2.79 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 361 µs | 9.3% | **0.02× [0.02–0.04]** | 2.77 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 215 µs | 7.9% | **0.03× [0.03–0.06]** | 4.65 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 202 µs | 3.7% | **0.03× [0.02–0.07]** | 4.94 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 19.8 µs | 7.4% | **0.40× [0.33–0.62]** | 50.5 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 19.3 µs | 5.7% | **0.37× [0.30–0.48]** | 51.7 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary2/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 6.86 µs | 18.4% | baseline | 146 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 436 µs | 11.8% | **0.01× [0.00–0.02]** | 2.3 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 410 µs | 7.5% | **0.02× [0.00–0.02]** | 2.44 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 239 µs | 4.4% | **0.03× [0.02–0.04]** | 4.18 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 233 µs | 3.7% | **0.03× [0.00–0.04]** | 4.3 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 43.3 µs | 7.3% | **0.16× [0.13–0.28]** | 23.1 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 47.7 µs | 8.4% | **0.14× [0.10–0.21]** | 21 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary4/4096

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 7.35 µs | 20.0% | baseline | 136 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainSearch | 395 µs | 1.9% | **0.02× [0.01–0.03]** | 2.53 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringSearch | 388 µs | 2.0% | **0.02× [0.01–0.03]** | 2.58 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainDense | 250 µs | 6.8% | **0.03× [0.02–0.05]** | 3.99 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringDense | 250 µs | 3.1% | **0.03× [0.02–0.05]** | 3.99 k | state_checksum=1.435e+08, completed_ticks=17 |
| DomainIndexed | 52.9 µs | 4.1% | **0.14× [0.11–0.25]** | 18.9 k | state_checksum=1.435e+08, completed_ticks=17 |
| WiringIndexed | 55 µs | 7.3% | **0.13× [0.10–0.21]** | 18.2 k | state_checksum=1.435e+08, completed_ticks=17 |

## Stack/Boundary1/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 144 µs | 1.5% | baseline | 6.94 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.32 ms | 2.7% | **0.02× [0.02–0.02]** | 137 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.12 ms | 2.1% | **0.02× [0.02–0.03]** | 141 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.56 ms | 1.4% | **0.03× [0.03–0.04]** | 219 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.58 ms | 2.1% | **0.03× [0.03–0.04]** | 218 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 426 µs | 1.7% | **0.34× [0.33–0.42]** | 2.35 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 449 µs | 4.7% | **0.32× [0.28–0.35]** | 2.23 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary2/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 143 µs | 1.9% | baseline | 6.98 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.93 ms | 3.2% | **0.02× [0.02–0.03]** | 126 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.8 ms | 3.0% | **0.02× [0.02–0.03]** | 128 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.99 ms | 3.4% | **0.03× [0.03–0.05]** | 200 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.78 ms | 4.9% | **0.03× [0.03–0.05]** | 209 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 522 µs | 16.0% | **0.32× [0.23–0.40]** | 1.92 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 449 µs | 8.1% | **0.33× [0.22–0.51]** | 2.23 k | state_checksum=3.652e+10, completed_ticks=17 |

## Stack/Boundary4/65536

| Design | time/boundary | err% | vs baseline | items/s | notes |
|---|---:|---:|---:|---:|---|
| HandWritten | 142 µs | 0.9% | baseline | 7.04 k | state_checksum=3.652e+10, completed_ticks=17 |
| DomainSearch | 7.47 ms | 1.5% | **0.02× [0.02–0.02]** | 134 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringSearch | 7.48 ms | 4.4% | **0.02× [0.02–0.02]** | 134 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainDense | 4.97 ms | 3.2% | **0.03× [0.03–0.03]** | 201 | state_checksum=3.652e+10, completed_ticks=17 |
| WiringDense | 4.68 ms | 2.8% | **0.03× [0.03–0.03]** | 214 | state_checksum=3.652e+10, completed_ticks=17 |
| DomainIndexed | 447 µs | 4.4% | **0.32× [0.27–0.34]** | 2.23 k | state_checksum=3.652e+10, completed_ticks=17 |
| WiringIndexed | 481 µs | 5.3% | **0.29× [0.27–0.34]** | 2.08 k | state_checksum=3.652e+10, completed_ticks=17 |

