# Reference runs

Curated runs of `bench/tools/run.py`, kept because a claim in
[docs/FINDINGS.md](../../../docs/FINDINGS.md) rests on them. Each directory is a
complete run: `meta.json` (machine, compiler, flags, git, warnings), one results
file per suite, and `summary.md`.

| Run | What |
|---|---|
| `CrogLegion/20261005-163357-a35bf97-ref-msvc-pcores` | MSVC 19.51, Core Ultra 9 275HX, `reference` profile, pinned to the 8 P-cores: storage scenarios up to 10M entities, fusion, planners x executors, Skirmish, H9 timeline, spans |
| `CrogLegion/20261005-165440-a35bf97-ref-msvc-threads` | Same build, unpinned: thread scaling 1 to 24 threads across P- and E-cores |

Notes on these two runs:
- Power plan Balanced, on a laptop; the first run started straight after its build
  (42% CPU load recorded). Ratios within a group are paired and interleaved, so
  they are robust to that; absolute times are less so.
- The thread run's `meta.json` records a dirty working tree. Its binaries were
  built from clean `a35bf97` by the preceding run; unrelated library sources were
  being edited when the second run started. The measured code is `a35bf97`.
