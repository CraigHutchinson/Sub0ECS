# Runtime-query timeline

1M entities, a query added at frame 0 that needs a component side-stored on 500K of them; 60 frames.
Median over the clean samples of the `exec` rotation. Times in microseconds unless marked.

| Build | Budget (entities/frame) | Degraded pass | Full pass | Worst frame | Frames to finish | Total migration (ms) |
|---|---|---:|---:|---:|---:|---:|
| msvc | stall (all at once) | 2444 | 318 | 32261 | 0 | 29.7 |
| msvc | 65536 | 1769 | 324 | 8924 | 7 | 31.0 |
| msvc | 16384 | 1396 | 324 | 6314 | 30 | 31.0 |
| msvc | 4096 | 1851 |  | 4332 | never | 15.1 |
| msvc | never (degraded only) | 2406 |  | 3073 | never | 0.0 |
| clangcl | stall (all at once) | 2587 | 250 | 34634 | 0 | 32.1 |
| clangcl | 65536 | 1720 | 250 | 9114 | 7 | 31.8 |
| clangcl | 16384 | 1310 | 220 | 5590 | 30 | 29.5 |
| clangcl | 4096 | 1866 |  | 3867 | never | 14.7 |
| clangcl | never (degraded only) | 2319 |  | 3216 | never | 0.0 |
| gcc | stall (all at once) | 2215 | 218 | 26920 | 0 | 24.5 |
| gcc | 65536 | 1533 | 217 | 7970 | 7 | 25.3 |
| gcc | 16384 | 1137 | 221 | 5989 | 30 | 25.5 |
| gcc | 4096 | 1769 |  | 4330 | never | 13.5 |
| gcc | never (degraded only) | 2234 |  | 2670 | never | 0.0 |
