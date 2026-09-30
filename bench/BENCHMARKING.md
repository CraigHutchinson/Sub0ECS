# Benchmarking

The first numbers came from a shared 4-core cloud VM. That was
good enough to rank designs, but not to publish or to measure scaling. This
harness makes runs **reproducible, self-describing and comparable**, so the
same suites can run on dedicated hardware with more cores (and later on
embedded targets).

```
bench/harness/            nanobench + names, --filter, paired group comparisons, results JSON
bench/tools/suites.json   what can be run (suites) and how (profiles)
bench/tools/run.py        build → fingerprint machine → run suites → result directory
bench/tools/compare.py    A/B comparison with noise-aware verdicts
CMakePresets.json         bench-native | bench-portable | sanitize
```

Everything uses CMake, Ninja, a C++20 compiler, [nanobench](https://github.com/martinus/nanobench)
(fetched) and the Python 3 standard library.

## How a measurement works

Cases are named `<Scenario>/<Pattern>/<Design>/<N>`; one operation is one pass
over the world (or one Skirmish tick). Cases sharing scenario, pattern and N form
a **group**, and a group's designs are measured **against each other, paired**
(nanobench `Bench::compare`):

- one iteration count for every design, epochs interleaved in rotating order,
  so frequency ramps, thermal drift and noisy neighbours hit every design alike
  and cancel out of the ratios;
- each design gets a ratio to the group's **baseline** (the first registered: v1
  wherever v1 supports the scenario, SparseSet for Skirmish, 1 thread for
  thread scaling) with a 95% interval corrected for the group's size;
- Skirmish runs one tick per epoch with no calibration, so every design plays
  exactly the same ticks and ends in the same state.

Cases that cannot repeat their operation as-is (Create: it needs a fresh world)
are measured alone, one call per epoch, with an untimed setup before each.

## Quick start

```bash
python3 bench/tools/run.py --suites list                  # what exists
python3 bench/tools/run.py --profile quick                # smoke test (~4 min, mostly build)
python3 bench/tools/run.py --profile standard             # the published FINDINGS settings
python3 bench/tools/run.py --profile reference --pin 2-15 --no-aslr --label ref-box   # dedicated hardware
python3 bench/tools/compare.py bench/results/runs/<host>/<runA> bench/results/runs/<host>/<runB>
```

## Profiles

| Profile | Epochs (paired rounds) | Min epoch | Sizes | Use |
|---|---:|---:|---|---|
| `quick` | 5 | default | 1K entities, 1K units | Check the harness works (seconds), not numbers |
| `standard` | 22 | 1 ms | 1K / 100K / 1M; 1K / 10K / 50K units | Everyday comparisons |
| `reference` | 52 | 5 ms | + 10M entities, + 200K units, thread ladder to all cores | Dedicated hardware |

Every design of a group is alive at once during its comparison, so the largest
group's memory is the sum of its worlds (about 8 worlds at the largest N).

## Suites

| Suite | Binary | Measures |
|---|---|---|
| `baseline` | `sub0ecs_bench` | Storage designs on micro scenarios (baseline + H1) |
| `fusion` | `sub0ecs_bench` | H7 sequential vs fused vs hand-merged |
| `fusion-exec` | `sub0ecs_bench` | Planners × executors |
| `skirmish` | `sub0ecs_skirmish_bench` | RTS ms/tick per design, per-system counters |
| `threads` | `sub0ecs_skirmish_bench` | Thread scaling: ladder 1, 2, 4 … up to `hardware_concurrency()` |
| `dynamic` | `sub0ecs_dynamic_timeline` | H9 stall vs incremental relayout (worst frame, frames to flip) |
| `spans` | `sub0ecs_spans_bench` | Partition-count (span) overhead |

The binaries read these environment variables (set by the profiles, or
with `--env KEY=VALUE`):

| Variable | Meaning | Default |
|---|---|---|
| `BENCH_SIZES` | entity counts for `sub0ecs_bench` (comma list, or `small`) | `1000,100000,1000000` |
| `SKIRMISH_UPT` | units per team (4 teams) for `Tick/*` | `250,2500,12500` |
| `SKIRMISH_THREADS` | thread counts for `Threads/*` | `1,2,4,…,hw` |
| `SKIRMISH_THREADS_UPT` | units per team for `Threads/*` | `2500,12500` |

Static-capacity designs are only instantiated up to the capacities
compiled in (1M entities / 200K units); larger sizes skip them.

## Result directory

```
bench/results/runs/<host>/<YYYYmmdd-HHMMSS>-<sha>[-label]/
    meta.json      schema "sub0ecs-bench/1"
    <suite>.json   results, schema "sub0ecs-bench-results/1" (or a timeline tool's rows)
    <suite>.log    console output (nanobench's tables)
    summary.md     per group: time/op, err%, paired ratio vs baseline [95% CI], counters
```

`meta.json` records:
- **CPU:** model, sockets/cores/threads, NUMA, caches, SIMD flags,
  hypervisor.
- **Memory and OS:** memory size, kernel/OS, libc.
- **Power state:** governor, turbo/boost, SMT, ASLR, load average.
- **Compiler and build:** compiler and version, build type, flags,
  `SUB0ECS_NATIVE`.
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
2. Per case: the medians, B-vs-A speed-up, and a verdict. A change
   is significant only if it exceeds **both** `--threshold` (default 5%)
   **and** twice the larger err% of the two runs.

Within one run, the paired ratios in `summary.md` are the sharper instrument:
they cancel drift, which a comparison across runs cannot.

`--fail-on-regression` returns exit code 1, so the same script can gate CI
on a dedicated runner.

## Windows / MSVC

Run from a **VS developer prompt** (`vcvars64.bat`), so the Ninja presets find
`cl.exe`; the same presets and suites apply. `SUB0ECS_NATIVE` maps to `/arch:AVX2`
(MSVC has no `-march=native`).

```bat
python bench/tools/run.py --profile reference --pin P --label ref-msvc
```

- The fingerprint records the CPU (CIM), SIMD support, the **hybrid P/E core map**,
  memory, power plan, AC/battery and CPU load, and warns on each noise source.
- `--pin` sets the process affinity mask (taskset syntax). On a hybrid CPU,
  `--pin P` pins to the performance cores. Unpinned threads migrate between core
  types, which shows up as bimodal timings.
- There is no ASLR switch (`--no-aslr` is Linux only).
- Thread scaling (`threads`) should run **unpinned**, so the ladder can reach every
  core. Run it as a separate invocation from the pinned single-threaded suites.
- Checklist equivalents: plug in AC power, choose the *Best performance* power mode
  or the *High performance* plan, close background apps, and keep the machine idle.

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
| Paired, interleaved rounds (≥ 52) | Drift cancels out of the design ratios | `--profile reference` (default in all profiles) |
| Commit the curated run | Reproducibility | Copy to `results/reference/<host>/`, note it in FINDINGS |

## Extending

- **New suite:** add an entry to `suites.json` (`harness` for
  bench/harness binaries; `timeline` for tools that take `[args…] <out.json>`
  and write `{"rows": [...]}`).
- **New machine class:** no code change; the fingerprint and thread ladder
  adapt to the machine.
- **Embedded (ESP32-P4, H4/H8e):** use a separate runner that
  flashes the device and captures serial output. It should keep the same
  `meta.json` schema (with device fields) so `compare.py` still works.
