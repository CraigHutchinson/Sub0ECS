# Rotation 20261010-090901-459a0d0-rotation-offset-columns

3 rounds, builds rotated, pin `0-3`, profile `compare`.

| Build | Samples used | Left out (busy machine) |
|---|---:|---:|
| control | 3 | 0 |
| candidate | 3 | 0 |

| Case | control time (spread) | candidate time (spread) | control vs baseline (range) | candidate vs baseline (range) |
|---|---:|---:|---:|---:|
| RowDispatch/Coherent/Each/100000 | 33.6 µs (±9%) | 22.5 µs (±5%) | 0.68× (0.66–0.81) | 0.98× (0.98–1.01) |
| RowDispatch/Coherent/Each/4096 | 1.26 µs (±2%) | 696 ns (±5%) | 0.60× (0.59–0.62) | 1.06× (1.02–1.07) |
| RowDispatch/Coherent/Each/64 | 28.6 ns (±25%) | 16.7 ns (±9%) | 0.63× (0.57–0.69) | 0.85× (0.83–0.88) |
| RowDispatch/Coherent/Grain1/100000 | 249 µs (±19%) | 22.5 µs (±9%) | 0.09× (0.08–0.11) | 1.03× (0.98–1.04) |
| RowDispatch/Coherent/Grain1/4096 | 10 µs (±1%) | 734 ns (±5%) | 0.08× (0.08–0.08) | 1.10× (1.03–1.14) |
| RowDispatch/Coherent/Grain1/64 | 160 ns (±22%) | 18.6 ns (±6%) | 0.10× (0.09–0.10) | 0.81× (0.78–0.81) |
| RowDispatch/Coherent/Grain1024/100000 | 24.1 µs (±6%) | 23.3 µs (±5%) | 0.99× (0.99–1.04) | 1.01× (0.92–1.03) |
| RowDispatch/Coherent/Grain1024/4096 | 738 ns (±3%) | 729 ns (±2%) | 0.99× (0.96–1.09) | 1.06× (1.04–1.11) |
| RowDispatch/Coherent/Grain1024/64 | 20.2 ns (±6%) | 23.5 ns (±4%) | 0.80× (0.78–0.85) | 0.64× (0.61–0.67) |
| RowDispatch/Coherent/Grain64/100000 | 28.2 µs (±12%) | 22.4 µs (±5%) | 0.81× (0.77–0.83) | 1.03× (1.01–1.03) |
| RowDispatch/Coherent/Grain64/4096 | 1.18 µs (±3%) | 754 ns (±5%) | 0.68× (0.63–0.69) | 1.01× (1.00–1.06) |
| RowDispatch/Coherent/Grain64/64 | 25.5 ns (±5%) | 23.7 ns (±1%) | 0.61× (0.56–0.69) | 0.63× (0.60–0.63) |
| RowDispatch/Coherent/HandWritten/100000 | 25 µs (±6%) | 22.7 µs (±6%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Coherent/HandWritten/4096 | 783 ns (±2%) | 741 ns (±6%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Coherent/HandWritten/64 | 15.9 ns (±12%) | 14.6 ns (±8%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/Each/100000 | 32.3 µs (±7%) | 23.5 µs (±3%) | 0.75× (0.75–0.76) | 1.04× (0.98–1.12) |
| RowDispatch/Fragmented/Each/4096 | 1.22 µs (±8%) | 700 ns (±8%) | 0.58× (0.56–0.62) | 1.04× (0.98–1.19) |
| RowDispatch/Fragmented/Each/64 | 22.4 ns (±6%) | 19.8 ns (±6%) | 0.77× (0.68–0.77) | 0.86× (0.82–0.87) |
| RowDispatch/Fragmented/Grain1/100000 | 255 µs (±8%) | 254 µs (±6%) | 0.10× (0.09–0.10) | 0.09× (0.09–0.09) |
| RowDispatch/Fragmented/Grain1/4096 | 9.07 µs (±7%) | 10 µs (±7%) | 0.08× (0.07–0.08) | 0.08× (0.08–0.08) |
| RowDispatch/Fragmented/Grain1/64 | 154 ns (±2%) | 153 ns (±3%) | 0.11× (0.11–0.11) | 0.11× (0.11–0.12) |
| RowDispatch/Fragmented/Grain1024/100000 | 25.2 µs (±4%) | 24.3 µs (±3%) | 0.99× (0.94–0.99) | 1.01× (0.93–1.03) |
| RowDispatch/Fragmented/Grain1024/4096 | 711 ns (±11%) | 751 ns (±2%) | 0.98× (0.84–1.04) | 1.04× (0.96–1.05) |
| RowDispatch/Fragmented/Grain1024/64 | 21.5 ns (±3%) | 27.2 ns (±2%) | 0.76× (0.71–0.78) | 0.65× (0.62–0.66) |
| RowDispatch/Fragmented/Grain64/100000 | 31.9 µs (±9%) | 31.5 µs (±5%) | 0.76× (0.76–0.79) | 0.75× (0.74–0.83) |
| RowDispatch/Fragmented/Grain64/4096 | 1.05 µs (±9%) | 1.24 µs (±4%) | 0.64× (0.62–0.72) | 0.67× (0.66–0.75) |
| RowDispatch/Fragmented/Grain64/64 | 26.2 ns (±3%) | 28.1 ns (±4%) | 0.62× (0.62–0.65) | 0.61× (0.61–0.61) |
| RowDispatch/Fragmented/HandWritten/100000 | 22.8 µs (±7%) | 23 µs (±10%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/HandWritten/4096 | 748 ns (±6%) | 785 ns (±7%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/HandWritten/64 | 16.3 ns (±3%) | 17.3 ns (±9%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
