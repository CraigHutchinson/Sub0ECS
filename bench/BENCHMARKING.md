# Benchmarking

How to measure the library, and how to tell whether a measurement can be
trusted. The harness makes runs **reproducible, self-describing and comparable**:
the same suites run on any machine and record what they ran on.
[README.md](README.md) describes what is being compared.

```
bench/harness/            nanobench + names, --filter, paired group comparisons, results JSON
bench/tools/suites.json   what can be run (suites) and how (profiles)
bench/tools/run.py        build → fingerprint machine → run suites → result directory
bench/tools/compare.py    A/B comparison with noise-aware verdicts
bench/tools/summarize.py  per-group tables of one run (run.py writes them as summary.md)
bench/tools/rotate.py     several interleaved samples of several builds: medians, spread, busy-machine check
bench/tools/tables.py     a rotate.py capture as tables relative to each group's reference
bench/tools/profile.py    one case under Intel VTune: hot functions, processor metrics
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
- that count is sized so the *fastest* design fills an epoch. Sized by the slowest,
  a group holding a very slow design times the fast ones on a single cold pass
  straight after the others have flushed the caches;
- each design gets a ratio to the group's **baseline** (the first registered: the
  hand-written loop for iteration and lookup, SparseSet elsewhere, 1 thread for
  thread scaling) with a 95% interval corrected for the group's size;
- Skirmish runs one tick per epoch with no calibration, so every design plays
  exactly the same ticks and ends in the same state.

Cases that cannot repeat their operation as-is (Create: it needs a fresh world)
are measured alone, one call per epoch, with an untimed setup before each.

## Quick start

```bash
python3 bench/tools/run.py --suites list                  # what exists
python3 bench/tools/run.py --profile quick                # smoke test (~4 min, mostly build)
python3 bench/tools/run.py --profile standard --pin P     # one full run: summary.md with paired ratios
python3 bench/tools/run.py --profile reference --pin 2-15 --no-aslr --label ref-box   # dedicated hardware
python3 bench/tools/rotate.py --build a=build/a --build b=build/b --pin P   # compare builds (several samples)
python3 bench/tools/compare.py bench/results/runs/<host>/<runA> bench/results/runs/<host>/<runB>
```

## Profiles

| Profile | Epochs (paired rounds) | Min epoch | Sizes | Use |
|---|---:|---:|---|---|
| `quick` | 5 | default | 1K entities, 1K units | Check the harness works (seconds), not numbers |
| `compare` | 11 | 1 ms | 1K / 100K entities | `rotate.py`: about a minute per sample, so many interleaved samples are affordable |
| `standard` | 22 | 1 ms | 1K / 100K / 1M; 1K / 10K / 50K units | Everyday comparisons |
| `reference` | 52 | 5 ms | + 10M entities, + 200K units, thread ladder to all cores | Dedicated hardware |

Every design of a group is alive at once during its comparison, so the largest
group's memory is the sum of its worlds (about a dozen at 1M entities, fewer above:
the fixed-capacity and naive designs stop at 1M).

## Suites

| Suite | Binary | Measures |
|---|---|---|
| `baseline` | `sub0ecs_bench` | Every design and reference on the micro scenarios |
| `fusion` | `sub0ecs_bench` | A four-system frame: separate passes vs fused vs hand-merged |
| `fusion-exec` | `sub0ecs_bench` | Planners × executors |
| `skirmish` | `sub0ecs_skirmish_bench` | RTS ms/tick per design, per-system counters |
| `threads` | `sub0ecs_skirmish_bench` | Thread scaling: ladder 1, 2, 4 … up to `hardware_concurrency()` |
| `dynamic` | `sub0ecs_dynamic_timeline` | Adding a query at runtime: one stall vs bounded migration per frame (worst frame, frames to finish) |
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
they cancel drift, which a comparison across runs cannot. `compare.py` reads two
single runs, so use it to look, not to conclude; for a conclusion take several
interleaved samples (next section).

`--fail-on-regression` returns exit code 1, so the same script can gate CI
on a dedicated runner.

## Comparing builds: several samples, interleaved

Within one run the designs of a group are already paired and interleaved, so
their ratios are robust. A comparison **between runs** is not: different
compilers, a branch against its base, one flag against another. The machine is
shared, it warms up, and other work comes and goes, so one run of each proves
nothing. Never quote a between-run difference from single runs.

```bash
python bench/tools/rotate.py --build msvc=build/msvc-native --build gcc=build/gcc16-native \
    --rounds 5 --suites baseline --pin P --label compilers
```

`rotate.py` takes `--rounds` samples of every build (default 5, never fewer than
3). Each round runs every build once and the starting build rotates, so heat and
drift do not land on the same build each time. Before each sample it pauses, then
waits for the CPU to be quiet (`--max-load`, default 10% of the machine), and it
records the load before and after. A sample taken on a busy machine is kept in
`rotation.json` but left out of the figures while two clean samples of that build
remain. `rotation.md` gives, per case and build, the median time with its spread
over the samples and the median ratio to the group's baseline with its range.

Reading it:

- Prefer the **ratio to the baseline** to the time: it is measured inside one run.
- A spread above about 10%, or a ratio range that crosses 1.0, means the samples
  disagree: take more rounds or find what else is running, do not pick a side.
- A difference counts when the ranges of the two builds do not overlap.
- Build everything first. A compile just before a sample heats the CPU.

## Finding out why: profiles and processor metrics

A benchmark run says how long a case takes and how it ranks. It does not say
why. Before changing code for speed, and again before explaining a result,
collect evidence in this order (cheapest first):

| Step | Question it answers | How |
|---|---|---|
| 1. Paired ratio to the bars | Is there a gap at all, and is it the library's? | `HandWritten` in the same group: a ratio near 1.0 means the library adds nothing to a plain loop. `HandTuned` shows what is left to the layout and to SIMD |
| 2. Hot functions | Which functions hold the time: the kernel, or the library around it? | `profile.py --collect hotspots` |
| 3. Processor metrics | Is it mispredicted branches, cache misses, or just more instructions? | `profile.py --collect uarch` |
| 4. The compiler's own report | Did the loop vectorise, and if not, why? Was the kernel inlined into it? | MSVC `/Qvec-report:2`, GCC `-fopt-info-vec-missed`, Clang `-Rpass-missed=loop-vectorize`; the assembly (MSVC `/FAs`, others `-S`) for a `call` inside the loop |
| 5. A probe | Does the candidate fix work on every compiler? | The loop and its hand-written equivalent in one small file, built with each compiler |

```bash
python bench/tools/profile.py --build-dir build/bench-native \
    --filter "^Update2/Fragmented/QPartHinted/100000$" --pin P --label update2
python bench/tools/profile.py --build-dir build/bench-native --collect uarch \
    --filter "^RandomGet/Fragmented/QPartHinted/100000$" --pin P --label randomget
```

`profile.py` runs one benchmark case for about `--seconds` (default 10) under
[Intel VTune](https://www.intel.com/content/www/us/en/developer/tools/oneapi/vtune-profiler.html)
and writes `bench/results/profiles/<host>/<stamp>-<sha>[-label]/` (git-ignored):
the VTune result, `summary.txt`, `hotspots.csv` and the command. It prints the
headline metrics and the heaviest functions. `receipt.json` preserves source/binary
identity, commands, stdout/stderr paths and exit codes. Failed reports or unusable
samples return nonzero with `no-profile`; `samples-exported` still requires manual
verification of workload coverage and source attribution. Software Hotspots is
requested explicitly. The collection duration is **not** a finalization deadline:
use an owned external supervisor and preserve stalled results; do not run the
wrapper unattended. See the [consumer campaign](../docs/optimization/crucible.md)
for the remaining supervision and qualification gates.

- **Name one design in `--filter`.** A filter that matches a whole group profiles
  every design in it, and the hot-function list mixes them.
- **`hotspots` requests user-mode sampling**; verify installed capability and usable
  samples. **`uarch` needs hardware-counter support and access**; receive those
  separately on the profiling host. Do not change system security settings just
  to turn an unavailable counter collection into a pass.
- The benchmark binaries carry debug information (`/Z7`, `-g`), which is what lets
  a profiler name functions. It does not change the generated code.
- Read `uarch` on a hybrid CPU per core type, and pin (`--pin P`): a P-core and an
  E-core give different answers to the same question.
- Without VTune, Linux `perf` answers steps 2 and 3:
  `perf stat -d -- <bench> --filter=...` and `perf record -g` / `perf report`.
  `profile.py` does not wrap them yet.
- VTune credits inlined code to the function it came from, so a kernel shows under
  its own name whether or not it was inlined. Use the assembly (step 4) to tell.

Worked example: step 1 showed the store's Update2 slower than the hand-written
loop on MSVC only. Step 2 put nearly all the time in `kernel::updatePosition`, not
in library code. Step 4 found the cause:
the assembly had a `call` to the kernel inside the row loop, where the
hand-written loop had it inlined. One statement attribute on the row call fixed it
(FINDINGS, section 4).

## Windows / MSVC

Run from a **VS developer prompt** (`vcvars64.bat`), so the Ninja presets find
`cl.exe`; the same presets and suites apply. `SUB0ECS_NATIVE` maps to `/arch:AVX2`
(MSVC has no `-march=native`).

```bat
python bench/tools/run.py --profile reference --pin P --label ref-msvc
```

- The fingerprint records the CPU (CIM), SIMD support, the **hybrid P/E core map**,
  memory, power plan, AC/battery and CPU load, and warns on each noise source.
- CPU load is measured over one second (`GetSystemTimes`); `Win32_Processor.LoadPercentage`
  is not used, because on a hybrid CPU it reported 88% with 12% of the machine in use.
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

- **Tables for a write-up:** `python bench/tools/tables.py <rotation dir>` prints every
  design's speed relative to its group's reference, per build, with the range over
  the samples.
- **New suite:** add an entry to `suites.json` (`harness` for
  bench/harness binaries; `timeline` for tools that take `[args…] <out.json>`
  and write `{"rows": [...]}`).
- **New machine class:** no code change; the fingerprint and thread ladder
  adapt to the machine.
- **Embedded (ESP32-P4):** use a separate runner that
  flashes the device and captures serial output. It should keep the same
  `meta.json` schema (with device fields) so `compare.py` still works.
