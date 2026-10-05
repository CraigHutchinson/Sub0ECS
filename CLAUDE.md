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
| Benchmarks | `standard` / `reference` profiles | minutes / hours | by hand, dedicated hardware |

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
| `bench/` | Comparison benchmarks: comparator designs, frozen v1 baseline, Skirmish testbed, harness, results |
| `docs/` | FINDINGS (decision record), research notes, BACKLOG |

## Rules

- **Single-responsibility headers.** One type, concept or alternative per header,
  grouped by directory, with an umbrella header per group. See `STYLE_GUIDE.md`.
- **The comparators stay honest.** `bench/baselines/v1/` is v1 frozen
  byte-identical; never edit it. The designs in `bench/designs/` are reference
  implementations for comparison, not library code.
- **Conformance before numbers.** Every design must pass the conformance tests
  (bit-identical state) before its benchmark numbers mean anything.
- **Record decisions with evidence.** A design choice backed by a measurement gets a
  FINDINGS section; ad-hoc benchmark runs stay in `bench/results/runs/`
  (ignored). Curate runs worth keeping into `bench/results/reference/<host>/`.
- **Profile before optimising.** A speed change starts from evidence, not from a
  guess: the paired ratio to the hand-written floor, then `bench/tools/profile.py`
  (VTune hot functions and processor metrics), then the compiler's vectoriser
  report. The order and commands are in `bench/BENCHMARKING.md`, "Finding out why".
  Compare compilers on one machine, in rotated runs; never across machines.
- **API changes** to anything under `include/sub0ecs/` are described in the commit
  message.
- Follow `STYLE_GUIDE.md`. Tests must pass before committing.
