# Full-stack evidence — 10 October 2026

Source/ownership and reproduction: [full-stack contract](../../../../docs/optimization/full-stack.md).
Library base is merged PR20 (`185f5c6`); exact benchmark source/binary SHA256s are in `manifest.json`.
GCC 13.3, Release O3, portable x86-64, strict ECS FP contraction policy, no LTO/native tuning.
Five independent process captures, internally interleaved alternatives. CPU affinity 0–3.
All five samples are included, including four library samples flagged BUSY; no outlier removal.
This virtual shared host has uncontrolled external load, power and thermal state. Ranges are
full process-median ranges, not confidence intervals or production performance qualification.

## Synthetic complete boundary

65,536 fixed dense identities, one worker; microseconds per complete boundary.

| Arm | Median [range], µs | Throughput relative to handwritten |
|---|---:|---:|
| HandWritten | 144.1 [137.6–146.1] | 1.000 |
| DomainSearch | 7414.1 [7292.2–7977.4] | 0.019 |
| WiringSearch | 7238.1 [7117.1–7624.2] | 0.020 |
| DomainDense | 4763.2 [4562.0–4848.8] | 0.031 |
| WiringDense | 4634.5 [4550.5–4884.7] | 0.030 |
| DomainIndexed | 431.2 [424.4–447.4] | 0.342 |
| WiringIndexed | 449.0 [432.2–467.5] | 0.323 |

Dense commit removes the per-row binary search. Indexed staging additionally removes the sort
under the stronger complete fixed-ID premise: DomainSearch → DomainIndexed is about 17.2× in
 this synthetic workload, yet Indexed still costs about 3× the handwritten floor. This is not
a Crucible speedup. Do not apply indexed staging to a mobile subset without a checked mapping.
Wiring does not establish a faster large boundary than DomainIndexed here. All widths/sizes
and the existing n-body/row-workload results are preserved in `library/`.

For a one-command admission/drain batch, median nanoseconds [range]:

- HandWritten: 4.80 [4.78–5.02] ns.
- Domain: 21.44 [20.95–25.23] ns.
- Wiring: 4.78 [4.76–5.69] ns.

This scopes a fixed synchronous endpoint. Domain retains dynamic registration, locking and
quiescence semantics; Wiring is not a drop-in replacement for every broker. The borrowed command
payload is copied into bounded owned storage in all arms. Timing includes drain and a receipt check.

## Actual Crucible consumer

Five alternating rounds × two library builds × direct/integrated routes = 20 processes.
All completed with matching emitted state/checksums, command traces and mission outcomes.
Full reference and structural missions are included. No native renderer was run.

| Integrated consumer workload | Pinned libraries median [range], ms | Current libraries median [range], ms |
|---|---:|---:|
| tick_capture_steady/2048 | 2.841 [2.778–2.988] | 2.750 [2.732–3.002] |
| reference_complete_mission/2048 | 897.359 [862.382–967.502] | 858.865 [849.097–926.267] |
| structural_complete_mission/2048 | 1715.043 [1652.732–1755.696] | 1648.912 [1626.875–1826.686] |

The ranges overlap; no qualified production speedup is established. Both integrated builds
report zero coordinator ordinary-new calls for steady ticks and admission, and five calls over
each complete mission. These counters do not intercept all worker/C-runtime/OS allocations.
Admission medians quantize to 0.05 µs in both actual-consumer builds; do not infer finer
routing attribution from that clock. Current ECS/Pub pins build and preserve emitted results,
but production pin promotion still needs Crucible’s full per-field replay/FP/lifecycle gates.
Raw JSONL, stderr, invocations and provenance are in `crucible/`.

## Profiling and validation

The pre-indexed control gprof capture has 227 samples: about 55% in sort and 41% in the
commit graph callback. Inlined attribution and instrumented code limit precision; no PMU/cache
or per-line claim is made. It motivates eliminating redundant lookup/sort work, not tuning Pub
first. `stack-vectorization.txt.gz` belongs to that pre-indexed profiling build;
`stack-linked-disassembly.txt.gz` is the final uninstrumented benchmark. Instrumented timings
are excluded from the performance tables. The profiling executable hash is retained.

- 14/14 CTest entries: 12 examples and gate/exhaustive; 86 doctest cases, 377,689,789 assertions.
- Focused fully instrumented ASan/UBSan, including Pipeline sources and Pub headers: 2 cases,
  5,407 assertions. Leak detection disabled; no leak or TSan claim.
- 11 Python tests, including receipt rejection on changed traces, missing/duplicate workloads
  and nonfinite timing. Five library captures and 20 actual-consumer captures succeeded.
- Local full tests use assertions-enabled O1 to bound compiler memory; benchmarks use O3.
- Platform CI is recorded on the PR, independently of these local receipts.
