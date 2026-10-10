# Rotation 20261010-011641-459a0d0-rotation-row-index

3 rounds, builds rotated, pin `0-3`, profile `compare`.

| Build | Samples used | Left out (busy machine) |
|---|---:|---:|
| control | 3 | 0 |
| candidate | 3 | 0 |

| Case | control time (spread) | candidate time (spread) | control vs baseline (range) | candidate vs baseline (range) |
|---|---:|---:|---:|---:|
| RowDispatch/Coherent/Each/100000 | 29.1 µs (±3%) | 20.5 µs (±6%) | 0.74× (0.71–0.74) | 1.01× (0.99–1.03) |
| RowDispatch/Coherent/Each/4096 | 1.16 µs (±0%) | 657 ns (±1%) | 0.59× (0.59–0.59) | 1.04× (1.03–1.05) |
| RowDispatch/Coherent/Each/64 | 21.4 ns (±1%) | 16 ns (±1%) | 0.68× (0.66–0.72) | 0.87× (0.87–0.92) |
| RowDispatch/Coherent/Grain1/100000 | 221 µs (±2%) | 62.8 µs (±2%) | 0.10× (0.10–0.10) | 0.34× (0.33–0.34) |
| RowDispatch/Coherent/Grain1/4096 | 8.97 µs (±0%) | 2.53 µs (±2%) | 0.08× (0.08–0.08) | 0.27× (0.27–0.28) |
| RowDispatch/Coherent/Grain1/64 | 146 ns (±4%) | 49.4 ns (±4%) | 0.10× (0.10–0.11) | 0.29× (0.28–0.31) |
| RowDispatch/Coherent/Grain1024/100000 | 21.5 µs (±1%) | 20.9 µs (±2%) | 0.99× (0.98–1.00) | 0.99× (0.99–1.01) |
| RowDispatch/Coherent/Grain1024/4096 | 719 ns (±2%) | 730 ns (±1%) | 0.99× (0.98–0.99) | 0.95× (0.94–0.96) |
| RowDispatch/Coherent/Grain1024/64 | 18.1 ns (±1%) | 17.9 ns (±2%) | 0.78× (0.77–0.83) | 0.80× (0.78–0.82) |
| RowDispatch/Coherent/Grain64/100000 | 27.3 µs (±1%) | 30.2 µs (±2%) | 0.78× (0.78–0.79) | 0.69× (0.68–0.70) |
| RowDispatch/Coherent/Grain64/4096 | 1.05 µs (±1%) | 1.25 µs (±2%) | 0.66× (0.65–0.66) | 0.55× (0.55–0.57) |
| RowDispatch/Coherent/Grain64/64 | 22.8 ns (±0%) | 18.1 ns (±2%) | 0.63× (0.60–0.67) | 0.79× (0.77–0.81) |
| RowDispatch/Coherent/HandWritten/100000 | 21.1 µs (±1%) | 20.9 µs (±3%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Coherent/HandWritten/4096 | 683 ns (±1%) | 683 ns (±1%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Coherent/HandWritten/64 | 14.5 ns (±5%) | 14.1 ns (±1%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/Each/100000 | 29.8 µs (±7%) | 20.8 µs (±4%) | 0.73× (0.71–0.73) | 1.02× (0.98–1.03) |
| RowDispatch/Fragmented/Each/4096 | 1.19 µs (±1%) | 684 ns (±1%) | 0.57× (0.56–0.62) | 1.04× (0.98–1.08) |
| RowDispatch/Fragmented/Each/64 | 20.5 ns (±1%) | 19 ns (±2%) | 0.74× (0.73–0.75) | 0.81× (0.79–0.81) |
| RowDispatch/Fragmented/Grain1/100000 | 217 µs (±5%) | 219 µs (±1%) | 0.09× (0.09–0.10) | 0.10× (0.09–0.10) |
| RowDispatch/Fragmented/Grain1/4096 | 8.8 µs (±2%) | 9.08 µs (±1%) | 0.08× (0.07–0.08) | 0.08× (0.08–0.08) |
| RowDispatch/Fragmented/Grain1/64 | 143 ns (±2%) | 145 ns (±2%) | 0.11× (0.11–0.11) | 0.11× (0.10–0.11) |
| RowDispatch/Fragmented/Grain1024/100000 | 21.7 µs (±6%) | 22.4 µs (±5%) | 0.97× (0.95–0.97) | 0.97× (0.94–1.01) |
| RowDispatch/Fragmented/Grain1024/4096 | 694 ns (±1%) | 719 ns (±2%) | 0.97× (0.94–1.04) | 1.00× (0.98–1.03) |
| RowDispatch/Fragmented/Grain1024/64 | 20.8 ns (±1%) | 25.1 ns (±2%) | 0.74× (0.71–0.75) | 0.60× (0.60–0.61) |
| RowDispatch/Fragmented/Grain64/100000 | 26.9 µs (±7%) | 28.4 µs (±4%) | 0.77× (0.76–0.77) | 0.78× (0.76–0.78) |
| RowDispatch/Fragmented/Grain64/4096 | 1.06 µs (±5%) | 1.05 µs (±4%) | 0.63× (0.61–0.68) | 0.67× (0.65–0.67) |
| RowDispatch/Fragmented/Grain64/64 | 24.6 ns (±1%) | 25.1 ns (±1%) | 0.62× (0.61–0.63) | 0.61× (0.60–0.63) |
| RowDispatch/Fragmented/HandWritten/100000 | 20.6 µs (±3%) | 21.5 µs (±4%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/HandWritten/4096 | 670 ns (±5%) | 715 ns (±4%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
| RowDispatch/Fragmented/HandWritten/64 | 15.5 ns (±3%) | 15.2 ns (±1%) | 1.00× (1.00–1.00) | 1.00× (1.00–1.00) |
