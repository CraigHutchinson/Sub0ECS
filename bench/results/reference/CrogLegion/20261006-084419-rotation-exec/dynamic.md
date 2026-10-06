# Runtime-query timeline

1M entities, a query added at frame 0 that needs a component side-stored on 500K of them; 60 frames.
Median over the clean samples of the `exec` rotation. Times in microseconds unless marked.

| Build | Budget (entities/frame) | Degraded pass | Full pass | Worst frame | Frames to finish | Total migration (ms) |
|---|---|---:|---:|---:|---:|---:|
| msvc | stall (all at once) | 2464 | 277 | 29240 | 0 | 26.8 |
| msvc | 65536 | 1757 | 277 | 8376 | 7 | 27.9 |
| msvc | 16384 | 1351 | 275 | 5678 | 30 | 26.9 |
| msvc | 4096 | 1849 |  | 4873 | never | 13.6 |
| msvc | never (degraded only) | 2318 |  | 2980 | never | 0.0 |
| clangcl | stall (all at once) | 2458 | 220 | 30405 | 0 | 28.1 |
| clangcl | 65536 | 1702 | 250 | 8818 | 7 | 32.2 |
| clangcl | 16384 | 1440 | 236 | 6359 | 30 | 31.3 |
| clangcl | 4096 | 2076 |  | 4312 | never | 16.5 |
| clangcl | never (degraded only) | 2423 |  | 3282 | never | 0.0 |
| gcc | stall (all at once) | 2421 | 229 | 29093 | 0 | 26.8 |
| gcc | 65536 | 1678 | 241 | 8714 | 7 | 28.0 |
| gcc | 16384 | 1406 | 247 | 6941 | 30 | 28.0 |
| gcc | 4096 | 1814 |  | 4178 | never | 13.1 |
| gcc | never (degraded only) | 2299 |  | 2587 | never | 0.0 |
