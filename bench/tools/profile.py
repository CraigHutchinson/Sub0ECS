#!/usr/bin/env python3
"""Profile one benchmark case with Intel VTune: where the time goes, and why.

A benchmark run says how long a case takes. This says which functions the time
is in (hotspots) and what the processor was doing meanwhile (uarch: retiring,
bad speculation, front-end and back-end bound, cache and branch misses).

    python bench/tools/profile.py --build-dir build/bench-native \
        --filter "^Update2/Fragmented/QPartHinted/100000$" --pin P --label update2

Writes <out>/<host>/<YYYYmmdd-HHMMSS>-<git sha>[-<label>]/:
    vtune/          the VTune result (open it in the VTune GUI for source and assembly views)
    summary.txt     elapsed and CPU time; with --collect uarch the top-down metrics
    hotspots.csv    CPU time per function
    command.txt     what was run

`hotspots` samples in user mode and needs no privileges. `uarch` reads the
hardware counters: on Windows run it from an elevated prompt (or install
VTune's sampling driver); on Linux lower kernel.perf_event_paranoid.
Only the Python standard library is used; see BENCHMARKING.md.
"""
import argparse
import csv
import datetime as dt
import os
import re
import shutil
import socket
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run as bench   # bench/tools/run.py: paths, CPU list parsing, hybrid core map

VTUNE_HOMES = (r"C:\Program Files (x86)\Intel\oneAPI\vtune\latest\bin64\vtune.exe", "/opt/intel/oneapi/vtune/latest/bin64/vtune")
COLLECTIONS = {"hotspots": "hotspots", "uarch": "uarch-exploration"}
EPOCHS = 20   # the profiled case runs EPOCHS epochs of seconds/EPOCHS each


def find_vtune():
    found = shutil.which("vtune") or next((p for p in VTUNE_HOMES if Path(p).exists()), None)
    if not found:
        sys.exit("vtune not found: install Intel VTune Profiler (oneAPI) or put vtune on PATH")
    return found


def pin_self(pin):
    """Restrict this process to the CPUs in `pin`; VTune and the benchmark inherit it."""
    cpus = bench.parse_cpulist(pin)
    if bench.WINDOWS:
        import ctypes
        k32 = ctypes.windll.kernel32
        k32.GetCurrentProcess.restype = ctypes.c_void_p
        if not k32.SetProcessAffinityMask(ctypes.c_void_p(k32.GetCurrentProcess()), ctypes.c_size_t(sum(1 << c for c in cpus))):
            print(f"  ! could not pin to {pin}")
    elif hasattr(os, "sched_setaffinity"):
        os.sched_setaffinity(0, cpus)


def top_functions(path, count):
    """([(function, seconds, module)] heaviest first, total seconds) from VTune's hotspots CSV."""
    with open(path, newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
    if not rows:
        return [], 1.0
    time_key = next(k for k in rows[0] if k.startswith("CPU Time"))
    rows.sort(key=lambda r: -float(r[time_key] or 0))
    total = sum(float(r[time_key] or 0) for r in rows) or 1.0
    return [(r.get("Function", "?"), float(r[time_key] or 0), r.get("Module", "")) for r in rows[:count]], total


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--build-dir", required=True, help="a benchmark build dir, e.g. build/bench-native")
    ap.add_argument("--exe", default="sub0ecs_bench", help="benchmark binary (default: sub0ecs_bench)")
    ap.add_argument("--filter", required=True, help="regex selecting the case(s); name one design to keep the profile about it")
    ap.add_argument("--collect", default="hotspots", choices=sorted(COLLECTIONS))
    ap.add_argument("--seconds", type=float, default=10.0, help="approximate measured time per selected case")
    ap.add_argument("--pin", default="", help="CPU list, or 'P' for the performance cores of a hybrid CPU")
    ap.add_argument("--label", default="")
    ap.add_argument("--out", default=str(bench.BENCH / "results" / "profiles"))
    ap.add_argument("--top", type=int, default=12, help="functions to print")
    args = ap.parse_args()

    vtune = find_vtune()
    exe = Path(args.build_dir) / "bench" / (args.exe + bench.EXE)
    if not exe.exists():
        sys.exit(f"{exe} not built")
    if args.pin == "P":
        args.pin = bench.cpu_info().get("hybrid", {}).get("P", "")
        if not args.pin:
            sys.exit("--pin P needs a hybrid CPU with detectable performance cores")
    if args.pin:
        pin_self(args.pin)

    sha = bench.git_info()["sha"][:7] or "nogit"
    host = re.sub(r"[^A-Za-z0-9_.-]", "_", socket.gethostname())[:40]
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    out = Path(args.out) / host / f"{stamp}-{sha}{'-' + args.label if args.label else ''}"
    out.mkdir(parents=True, exist_ok=True)
    result = out / "vtune"

    target = [str(exe), f"--filter={args.filter}", f"--epochs={EPOCHS}", f"--min-epoch-ms={max(1, round(args.seconds * 1000 / EPOCHS))}"]
    collect = [vtune, "-collect", COLLECTIONS[args.collect], "-result-dir", str(result), "-quiet", "--", *target]
    (out / "command.txt").write_text(f"pin: {args.pin or '(none)'}\n{' '.join(collect)}\n", encoding="utf-8")
    print(f"profile dir: {out}")
    with open(out / "target.log", "w", encoding="utf-8") as log:
        rc = subprocess.run(collect, stdout=log, stderr=subprocess.STDOUT).returncode
    if rc != 0 or not result.exists():
        print((out / "target.log").read_text(encoding="utf-8")[-1500:])
        sys.exit(f"vtune -collect {COLLECTIONS[args.collect]} failed (rc={rc}); for uarch see the note on privileges in --help")

    def report(kind, dest, *extra):
        with open(out / dest, "w", encoding="utf-8") as f:
            return subprocess.run([vtune, "-report", kind, "-result-dir", str(result), *extra], stdout=f, stderr=subprocess.DEVNULL).returncode

    report("summary", "summary.txt")
    report("hotspots", "hotspots.csv", "-format", "csv", "-csv-delimiter", "comma")

    wanted = re.compile(r"Elapsed Time|CPU Time|Effective CPU|Retiring|Front-End Bound|Bad Speculation|Back-End Bound|Memory Bound|Core Bound"
                        r"|Branch Mispredict|L1 Bound|L2 Bound|L3 Bound|DRAM Bound|Clockticks|Instructions Retired|CPI Rate|Average CPU Frequency")
    for line in (out / "summary.txt").read_text(encoding="utf-8", errors="replace").splitlines():
        if wanted.search(line) and not line.lstrip().startswith("Function"):   # not the embedded table's header
            print("  " + line.rstrip())
    functions, total = top_functions(out / "hotspots.csv", args.top)
    print(f"  top {len(functions)} functions by CPU time:")
    for name, seconds, module in functions:
        print(f"    {seconds:7.3f} s {100 * seconds / total:5.1f}%  {name[:110]}  [{module}]")
    print(f"done: {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
