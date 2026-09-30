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
- **API changes** to anything under `include/sub0ecs/` are described in the commit
  message.
- Follow `STYLE_GUIDE.md`. Tests must pass before committing.
