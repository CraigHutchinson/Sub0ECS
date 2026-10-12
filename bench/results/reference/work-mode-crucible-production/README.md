# Crucible production headless optimization

This campaign changes Crucible's production coordinator with its Sub0 dependency
pins held fixed. It measures `InspectorSession::TryPump`, including command drain,
simulation, snapshot capture and publication. Admission, rendering, pacing and
input-to-visible latency are outside the timed interval. This is neither a
handwritten-game comparison nor native frame/FPS acceptance.

## Pass A: checked compact-row commit

Pass A replaces per-mobile-entity binary search with a preallocated ID-to-row map.
Gather sorting remains. Sparse mobile subsets, anchored/lost identities and joined
worker publication retain their existing rules. Independent tests compare every
sample's float bits against public all-row steering from an owned pre-tick snapshot,
bypassing the production gather and commit map. No numerical rule changes.

Five alternating independent control/candidate process pairs per target, four
workers, 120 warmup and 300 measured advancing ticks, FLOW/repel/gather mutations
every six ticks. GCC 13.3, Release O3 C++23, portable x86-64, CPU affinity 0–3.
External load, power and thermal state are uncontrolled on this shared virtual host.
All process medians and tails are retained; ranges below are observed process-median
ranges, not confidence intervals.

| Population / geometry | Control median [range], ms | A median [range], ms | Median reduction |
|---|---:|---:|---:|
| 100K / 400×250 | 62.785 [60.403–64.048] | 59.868 [59.175–62.420] | 4.65% |
| 150K / 500×300 | 90.824 [87.891–94.684] | 87.330 [84.736–88.038] | 3.85% |

Both ranges overlap. A alone is encouraging, but does not establish a robust
whole-consumer speedup beyond host variation. See the [100K](pass-a-100000/summary.json)
and [150K](pass-a-150000/summary.json) summaries and sibling raw receipts. Each run
checks initial/final semantic state digests, command traces, completed work and
zero fallback/discarded work; hashes are drift detectors, not collision-free proofs.
Coordinator `new` counts were zero during these measured pumps, not a claim about
all allocations on every worker or platform.

The archived [2048 summary](pass-a-2048/summary.json) is **not qualified**: two raw
process receipts are missing. Its surviving summary cannot substitute for complete
raw evidence or close the small-workload regression gate. Retain it as an incomplete
attempt and rerun the entire paired cohort.

## Refinement and combinations

B guards redundant ID sorts in gather, spatial rebuild and snapshot capture with
`std::ranges::is_sorted`; the original arbitrary-order sort remains the fallback.
Duplicate rejection, finite validation and complete ordered neighbor accumulation
remain intact. The spatial fixture checks sorted duplicate rejection without
changing the previously committed grid. No new public switch or extra storage.

### Received 100K combination cohort

The fresh [100K four-arm cohort](combined-100000-received/summary.json) contains all
20 raw process receipts: five alternating rounds of control, A, B and A+B with
matching semantic state, traces and completed work. Same GCC 13.3 O3 build policy,
four workers, 120 warmup and 300 measured ticks, gather route:

| Arm | Median [process-median range], ms |
|---|---:|
| Control | 63.603 [62.822–65.792] |
| A: checked mapping | 60.787 [60.098–61.579] |
| B: sort guards | 59.041 [57.924–59.788] |
| A+B | 56.493 [55.279–56.874] |

**A+B reduces the 100K production headless pump/capture median by 11.18%.** All five
paired comparisons improve and the observed process-median ranges do not overlap.
This establishes a scoped median improvement on this host; it does not establish
native FPS or universal performance. Tails are mixed: median process p95 improves
70.331→64.478 ms, while median process p99 rises 74.365→75.687 ms. Do not describe
this as an across-the-board tail-latency improvement.

The old combined 2048 cohort also remains incomplete: three raw receipts are empty.
Its summary cannot qualify. Replacement 2048 and complete 150K combination results
are pending, so the small-regression and broader-scale gates remain open.
Dependency pins are unchanged; this gain belongs to production source optimization.

## Source and reproduction

The control is production source `118ce62a75c8e6f1c325cc915cbc539c558a4c11`
plus the identical benchmark receiver. The archived source patches make the four
arms reproducible without depending on a local branch name:

| Arm | Local measured source | Tree / construction |
|---|---|---|
| Control | `b45b145b4db52039c0ae12dce2265fe182ea6cd9` | `48dd04ee95a992b10aef261a7782af1a05640d86`; base + [benchmark.patch](source/benchmark.patch) |
| A | `822652fe0a2d5da441040a3a7c78d63675d9a304` | `f77527fc50f58a0ca15b1c02c48b9495e897478a`; control + [mapping.patch](source/mapping.patch) |
| B | `63c11e1769634164ed1bc0c882c1e17bde564738` | `4b5bfcf156ee32241c892fdfcb7ac69575facdd3`; control + sort guards |
| A+B | `d5c902012ca83b410d76b1426012661fd586e45f` | `1d3e4f3451f7126e8c876e0edc9c7e2d62463541`; A + [sort-guards.patch](source/sort-guards.patch) |

Use [sort-only.patch](source/sort-only.patch) directly on control for B, or
[sort-guards.patch](source/sort-guards.patch) on A for A+B.
Verify each reconstructed tree against the table and commit before capture.
The benchmark SHA256 is
`0f0fd1e6f8f90082cb206bfa93bce5fc345cc2c581ee60d5f41a8964773aed75`.
Published equivalents have identical trees and different commit metadata:
control `035e4fbc246c757f108760d7bfda4d3b758baa54`,
A `7060690de0f9807b0e0c86e9c6b6e56379abba5b`,
B `909c02ec68e70d875a8e954d3b279686e087c496`,
A+B `3754092943e5386fb8774a8f41011c4989de8fcc` in
[Crucible](https://github.com/CraigHutchinson/Crucible).

Build each clean checkout with the same compiler and flags:

```sh
cmake -S /path/to/arm -B /path/to/arm/build -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTING=ON -DCRUCIBLE_BUILD_BENCHMARKS=ON \
  -DCRUCIBLE_BUILD_DESKTOP=OFF -DCRUCIBLE_BUILD_GPU_RECEIVER=OFF
cmake --build /path/to/arm/build --target crucible_runtime_backbone_bench --parallel 2
```

Use the committed dependency pins. This host reused clean CPM source checkouts at
those exact pins. Control was built with BUILD_TESTING=OFF; candidate arms used
ON, enabling extra test targets and Sub0Log test support without changing benchmark
compile/link flags. The losslessly compressed `meta.json.gz` files record overrides, resolved
revisions, CMake caches, compile/link flags and executable hashes. An override is
valid only when its source matches the committed pin; it is not a current-library
comparison. Keep sources and binaries frozen throughout each cohort.

From Sub0ECS, choosing affinity valid for the measurement machine:

```sh
python3 bench/tools/crucible.py --source /path/to/control \
  --build control=/path/to/control/build --build candidate=/path/to/candidate/build \
  --scale 100000 --workers 4 --warmup 120 --ticks 300 --field-route gather \
  --arms integrated --rounds 5 --cooldown 2 --timeout 600 \
  --out bench/results/runs/crucible-production-100000
```

Repeat at 2048 and 150000 and for each candidate; output directories must be new.
Stage-clock probes are separate diagnostics and cannot qualify. Run complete
reference/structural missions separately without `--scale` or `--skip-missions`.
The [full-stack contract](../../../../docs/optimization/full-stack.md) distinguishes
this production-source comparison from the earlier inconclusive library update.
