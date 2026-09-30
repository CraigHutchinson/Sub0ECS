# Benchmarking the v2 spike

The spike's numbers so far come from a shared 4-core cloud VM. That was
good enough to rank designs, but not to publish or to measure scaling. This
harness makes runs **reproducible, self-describing and comparable**, so the
same suites can run on dedicated hardware with more cores (and later on
embedded targets).

```
tools/bench/suites.json   what can be run (suites) and how (profiles)
tools/bench/run.py        build → fingerprint machine → run suites → result directory
tools/bench/compare.py    A/B comparison with noise-aware verdicts
CMakePresets.json         bench-native | bench-portable | sanitize
```

Everything uses only CMake, Ninja, a C++20 compiler and the Python 3
standard library.

## Quick start

```bash
cd spike/v2
python3 tools/bench/run.py --suites list                  # what exists
python3 tools/bench/run.py --profile quick                # smoke test (~4 min, mostly build)
python3 tools/bench/run.py --profile standard             # the spike's published settings
python3 tools/bench/run.py --profile reference --pin 2-15 --no-aslr --label ref-box   # dedicated hardware
python3 tools/bench/compare.py results/runs/<host>/<runA> results/runs/<host>/<runB>
```

## Profiles

| Profile | Repetitions | Min time | Sizes | Use |
|---|---:|---:|---|---|
| `quick` | 1 | 0.05 s | 1K entities, 1K units | Check the harness works, not numbers |
| `standard` | 5 | 0.5 s | 1K / 100K / 1M; 1K / 10K / 50K units | What FINDINGS.md was measured with |
| `reference` | 10 | 1 s | + 10M entities, + 200K units, thread ladder to all cores | Dedicated hardware |

## Suites

| Suite | Binary | Measures |
|---|---|---|
| `baseline` | `spike_bench` | Storage designs on micro scenarios (baseline + H1) |
| `fusion` | `spike_bench` | H7 sequential vs fused vs hand-merged |
| `fusion-exec` | `spike_bench` | Planners × executors |
| `skirmish` | `skirmish_bench` | RTS ms/tick per design, per-system counters |
| `threads` | `skirmish_bench` | Thread scaling: ladder 1, 2, 4 … up to `hardware_concurrency()` |
| `dynamic` | `spike_dynamic_timeline` | H9 stall vs incremental relayout (worst frame, frames to flip) |
| `spans` | `spike_spans_micro` | Partition-count (span) overhead |

The binaries read these environment variables (set by the profiles, or
with `--env KEY=VALUE`):

| Variable | Meaning | Default |
|---|---|---|
| `SPIKE_SIZES` | entity counts for `spike_bench` (comma list, or `small`) | `1000,100000,1000000` |
| `SKIRMISH_UPT` | units per team (4 teams) for `Tick/*` | `250,2500,12500` |
| `SKIRMISH_THREADS` | thread counts for `Threads/*` | `1,2,4,…,hw` |
| `SKIRMISH_THREADS_UPT` | units per team for `Threads/*` | `2500,12500` |

Static-capacity designs are only instantiated up to the capacities
compiled in (1M entities / 200K units); larger sizes skip them.

## Result directory

```
results/runs/<host>/<YYYYmmdd-HHMMSS>-<sha>[-label]/
    meta.json      schema "sub0ecs-spike-bench/1"
    <suite>.json   raw Google Benchmark JSON (or timeline JSON)
    <suite>.log    console output
    summary.md     median + CV per benchmark
```

`meta.json` records:
- **CPU:** model, sockets/cores/threads, NUMA, caches, SIMD flags,
  hypervisor.
- **Memory and OS:** memory size, kernel/OS, libc.
- **Power state:** governor, turbo/boost, SMT, ASLR, load average.
- **Compiler and build:** compiler and version, build type, flags,
  `SPIKE_NATIVE`.
- **Git:** SHA, branch, dirty flag.
- **Run:** profile settings, suites, pinning, extra environment, and per
  suite the exact command line, exit code and duration.

It also lists **warnings** for anything that makes numbers less trustworthy.

`results/runs/` is git-ignored. Curate runs worth keeping into
`results/reference/<host>/` and commit them together with a note in
FINDINGS.md.

## Comparing runs

`compare.py A B` prints:
1. **Environment differences first** (CPU, compiler, flags, SHA,
   profile), so a cross-machine difference is never mistaken for a code
   change.
2. Per benchmark: the medians, B-vs-A speed-up, and a verdict. A change
   is significant only if it exceeds **both** `--threshold` (default 5%)
   **and** twice the larger coefficient of variation of the two runs.

`--fail-on-regression` returns exit code 1, so the same script can gate CI
on a dedicated runner.

## Dedicated-hardware checklist

| Step | Why | How (Linux) |
|---|---|---|
| Idle machine, no GUI or background jobs | Noise | `uptime`: load ≈ 0 |
| `performance` governor | Frequency stability | `cpupower frequency-set -g performance` |
| Turbo/boost off (or record it) | Thermal drift between runs | `echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo` |
| Decide on SMT | Thread scaling differs per logical vs physical core | Record it (meta shows `smt_active`); run both ways when studying scaling |
| Isolate and pin CPUs | Scheduler noise | Boot with `isolcpus=2-15 nohz_full=2-15`, then `--pin 2-15` |
| No ASLR | Layout noise | `--no-aslr` (uses `setarch -R`) |
| Same build flavour for cross-machine comparisons | `-march=native` differs per CPU | `--preset bench-portable` |
| ≥ 10 repetitions, random interleaving | Robust medians, drift spread across benchmarks | `--profile reference` (default in all profiles) |
| Commit the curated run | Reproducibility | Copy to `results/reference/<host>/`, note it in FINDINGS |

## Extending

- **New suite:** add an entry to `suites.json` (`gbench` for Google
  Benchmark binaries; `timeline` for tools that take `[args…] <out.json>`
  and write `{"rows": [...]}`).
- **New machine class:** no code change; the fingerprint and thread ladder
  adapt to the machine. For MSVC/Windows, add configure presets using the
  Visual Studio generator; `run.py` works there without `taskset` or
  `setarch`.
- **Embedded (ESP32-P4, spike H4/H8e):** use a separate runner that
  flashes the device and captures serial output. It should keep the same
  `meta.json` schema (with device fields) so `compare.py` still works.
