# SubzeroECS Project Rules

## Build & Test

```bash
cmake --preset default          # Release: tests + benchmarks (Ninja; Windows: VS developer prompt)
cmake --build --preset default
ctest --preset default
```

`ci-msvc` uses the Visual Studio generator and needs no developer prompt. Benchmarks
are built by `default` but not run by ctest; use the harness:
`python3 bench/tools/run.py --profile quick` (see `bench/BENCHMARKING.md`).

## Test and CI tiers

Keep the everyday loop fast; put breadth on demand.

| Tier | What | Budget | Where |
|---|---|---|---|
| Gate | `ctest --preset default` (label `gate`; includes the examples, label `example`) | ~5 s Release, ~60 s sanitizers (now 1.2 s / 35 s) | every push, any branch (`ci.yml`) |
| Exhaustive | doctest `TEST_SUITE("exhaustive")`, label `exhaustive`: `ctest --preset exhaustive` | under a minute (now 24 s) | merges: PRs into and pushes to `master`/`v2` (`ci.yml`, Clang job); nightly, also under sanitizers |
| Benchmark smoke | `bench/tools/run.py --profile quick` | ~10 s | `ci.yml` (Linux GCC) |
| Benchmarks | `standard` / `reference` profiles; `rotate.py` (`compare` profile) for anything between builds | minutes / hours; ~1 min per sample | by hand |

When a test outgrows the gate budget, move the heavy variant into the exhaustive
suite and keep a small representative sample gated (see the churn test in
`tests/test_store.cpp`). CI time is dominated by compiling, not testing: benchmarks
are compiled on GCC, MSVC and macOS only. `nightly.yml` runs from the default branch
only (a GitHub rule) and tests that branch.

## Layout

| Path | What |
|---|---|
| `include/sub0ecs/` | The library (header-only, `Sub0ECS::Sub0ECS`) |
| `tests/` | doctest suites, one `sub0ecs_tests` binary |
| `bench/` | Comparison benchmarks: reference designs, Skirmish testbed, harness, results |
| `docs/` | FINDINGS (design and evidence), EXAMPLES, BACKLOG (open work), research notes (exploration-phase design notes) |

## Rules

- **Single-responsibility headers.** One type, concept or alternative per header,
  grouped by directory, with an umbrella header per group. See `STYLE_GUIDE.md`.
- **The references stay honest.** The designs in `bench/designs/` are what the
  library is measured against, not library code: the hand-written bars
  (`handwritten.hpp`) are the target, and a headline figure is always quoted
  relative to them, on a named machine and compiler. Never weaken a reference to
  improve a ratio.
- **Conformance before numbers.** Every design must pass the conformance tests
  (bit-identical state) before its benchmark numbers mean anything.
- **Record decisions with evidence.** A design choice backed by a measurement gets a
  FINDINGS section; ad-hoc benchmark runs stay in `bench/results/runs/`
  (ignored). Curate runs worth keeping into `bench/results/reference/<host>/`.
- **No stale numbers.** A figure stays in the tree only while the current code
  produces it. When a measurement is superseded, replace it everywhere (docs,
  comments, curated captures); the history is in git.
- **Profile before optimising.** A speed change starts from evidence, not from a
  guess: the paired ratio to the hand-written floor, then `bench/tools/profile.py`
  (VTune hot functions and processor metrics), then the compiler's vectoriser
  report. The order and commands are in `bench/BENCHMARKING.md`, "Finding out why".
  Compare compilers on one machine, never across machines.
- **Consumer integration work.** Follow `docs/optimization/crucible.md` for actual
  caller attribution, adapter ownership, refinement passes and the stronger
  five-pair qualification gate. Microbenchmarks alone do not receive a consumer.
- **Several samples, interleaved.** Other work runs on the benchmark machine and it
  heats up. Never judge a between-run difference from one run of each: use
  `bench/tools/rotate.py` (at least three rounds, builds rotated, load checked before
  each sample) and report the spread with the figure.
- **API changes** to anything under `include/sub0ecs/` are described in the commit
  message.
- Follow `STYLE_GUIDE.md`. Tests must pass before committing.
