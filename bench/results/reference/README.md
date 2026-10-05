# Reference runs

Curated captures, kept because a claim in
[docs/FINDINGS.md](../../../docs/FINDINGS.md) or the README rests on them.

| Capture | What |
|---|---|
| [`CrogLegion/20261006-000543-rotation-bars`](CrogLegion/20261006-000543-rotation-bars/tables.md) | **The current headline figures.** `rotate.py`, five interleaved samples each of MSVC 19.51, clang-cl 22.1 and GCC 16.2 (host-tuned, no FP contraction), `compare` profile, pinned to the P-cores of a Core Ultra 9 275HX. `tables.md`: every design relative to its reference, with the range over the samples; `rotation.md` / `rotation.json`: every case and sample; `meta-*.json`: machine, compiler and flags per build |
| `CrogLegion/20261005-163357-a35bf97-ref-msvc-pcores` | One MSVC run of the `reference` profile (52 paired rounds), pinned to the P-cores: sizes up to 10M entities, fusion, planners x executors, Skirmish, runtime-system timeline, spans. Source for FINDINGS section 5 and the spans figures |
| `CrogLegion/20261005-165440-a35bf97-ref-msvc-threads` | Same build, unpinned: thread scaling from 1 to 24 threads across P- and E-cores |

Notes:

- The rotation was taken from the working tree of the commit that added it; all
  fifteen samples started with the machine at 2-6% load.
- The two `a35bf97` runs are single runs, taken before `rotate.py` existed, and
  before a harness fix that sizes a group's samples by its fastest design. Their
  groups had no very slow member, so that fix does not change them, but ratios
  between runs of that vintage were never sampled repeatedly. They also contain a
  `V1` design, the previous API, which has since been removed from the tree (it is
  at the `v1.0.0` tag). Power plan Balanced; the first run started straight after
  its build.
- The thread run's `meta.json` records a dirty working tree. Its binaries were
  built from clean `a35bf97` by the preceding run.
