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
import hashlib
import json
import math
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
    """Read usable CPU samples; an empty or unresolved export is not a profile."""
    with open(path, newline="", encoding="utf-8-sig") as f:
        rows = list(csv.DictReader(f))
    if not rows:
        raise ValueError("hotspots export has no samples")
    # Do not accidentally read a utilization/percentage column as seconds.
    time_keys = [key for key in rows[0] if key and key in ("CPU Time", "CPU Time:Self", "CPU Time:Self (sec)")]
    if len(time_keys) != 1 or "Function" not in rows[0]:
        raise ValueError("unsupported hotspots schema; inspect the preserved raw CSV")
    time_key = time_keys[0]
    functions = []
    for row in rows:
        seconds = float(row[time_key] or 0)
        if not math.isfinite(seconds) or seconds < 0:
            raise ValueError("invalid CPU time in hotspots export")
        functions.append((row.get("Function", "").strip(), seconds, row.get("Module", "")))
    total = sum(row[1] for row in functions)
    unresolved = {"", "?", "unknown", "[unknown]", "[unknown function]", "[outside any known module]"}
    if not math.isfinite(total) or total <= 0:
        raise ValueError("hotspots export has no positive CPU samples")
    if not any(name.lower() not in unresolved and seconds > 0 for name, seconds, _ in functions):
        raise ValueError("hotspots export has no resolved function samples")
    functions.sort(key=lambda row: -row[1])
    return functions[:count], total


def run_logged(command, out, name, receipt):
    """Keep both output streams and exit status, including failed exports."""
    entry = {"command": command, "stdout": name + ".log", "stderr": name + ".stderr.log"}
    receipt["commands"].append(entry)
    (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    try:
        with open(out / entry["stdout"], "w", encoding="utf-8") as stdout, open(out / entry["stderr"], "w", encoding="utf-8") as stderr:
            entry["returncode"] = subprocess.run(command, stdout=stdout, stderr=stderr).returncode
    except OSError as error:
        entry["error"] = str(error)
        raise
    finally:
        (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    if entry["returncode"] != 0:
        raise ValueError(f"{name} failed (rc={entry['returncode']}); inspect {out}")
    return out / entry["stdout"]


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
    if not math.isfinite(args.seconds) or args.seconds <= 0 or args.top <= 0:
        ap.error("--seconds must be finite and positive; --top must be positive")
    if args.label and not re.fullmatch(r"[A-Za-z0-9_.-]+", args.label):
        ap.error("--label must contain only letters, digits, underscore, dot or hyphen")

    vtune = find_vtune()
    exe = (Path(args.build_dir) / "bench" / (args.exe + bench.EXE)).resolve()
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
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S-%f")
    out = Path(args.out) / host / f"{stamp}-{sha}{'-' + args.label if args.label else ''}"
    out.mkdir(parents=True, exist_ok=False)
    result = out / "vtune"

    target = [str(exe), f"--filter={args.filter}", f"--epochs={EPOCHS}", f"--min-epoch-ms={max(1, round(args.seconds * 1000 / EPOCHS))}"]
    collect = [vtune, "-collect", COLLECTIONS[args.collect]]
    if args.collect == "hotspots":
        collect += ["-knob", "sampling-mode=sw"]
    collect += ["-duration", str(args.seconds), "-result-dir", str(result), "-quiet", "--", *target]
    (out / "command.txt").write_text(f"pin: {args.pin or '(none)'}\n{' '.join(collect)}\n", encoding="utf-8")
    print(f"profile dir: {out}")
    digest = hashlib.sha256()
    with exe.open("rb") as binary:
        for block in iter(lambda: binary.read(1024 * 1024), b""):
            digest.update(block)
    binary_hash = digest.hexdigest()
    receipt = {"status": "no-profile", "git": bench.git_info(), "executable": str(exe),
               "executable_sha256": binary_hash, "pin": args.pin, "commands": [],
               "scope": "benchmark diagnostic; consumer attribution requires manual review"}
    (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    try:
        run_logged([vtune, "-version"], out, "version", receipt)
        run_logged([vtune, "-help", "collect", COLLECTIONS[args.collect]], out, "help", receipt)
        run_logged(collect, out, "target", receipt)
        if not result.is_dir():
            raise ValueError("collection returned without a result directory")
        summary = run_logged([vtune, "-report", "summary", "-result-dir", str(result)], out, "summary", receipt)
        summary.rename(out / "summary.txt")
        receipt["commands"][-1]["stdout"] = "summary.txt"
        hotspots = run_logged([vtune, "-report", "hotspots", "-result-dir", str(result),
                               "-format", "csv", "-csv-delimiter", "comma"], out, "hotspots", receipt)
        hotspots.rename(out / "hotspots.csv")
        receipt["commands"][-1]["stdout"] = "hotspots.csv"
        functions, total = top_functions(out / "hotspots.csv", args.top)
        receipt.update(status="samples-exported", cpu_seconds=total,
                       attribution="not reviewed; verify intended workload and source/symbol coverage")
    except (OSError, ValueError, KeyError, TypeError, csv.Error) as error:
        receipt["error"] = str(error)
        print(f"no profile: {error}", file=sys.stderr)
        return 1
    finally:
        (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")

    wanted = re.compile(r"Elapsed Time|CPU Time|Effective CPU|Retiring|Front-End Bound|Bad Speculation|Back-End Bound|Memory Bound|Core Bound"
                        r"|Branch Mispredict|L1 Bound|L2 Bound|L3 Bound|DRAM Bound|Clockticks|Instructions Retired|CPI Rate|Average CPU Frequency")
    for line in (out / "summary.txt").read_text(encoding="utf-8", errors="replace").splitlines():
        if wanted.search(line) and not line.lstrip().startswith("Function"):   # not the embedded table's header
            print("  " + line.rstrip())
    print(f"  top {len(functions)} functions by CPU time:")
    for name, seconds, module in functions:
        print(f"    {seconds:7.3f} s {100 * seconds / total:5.1f}%  {name[:110]}  [{module}]")
    print(f"done: {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
