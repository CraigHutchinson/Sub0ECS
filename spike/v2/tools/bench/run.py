#!/usr/bin/env python3
"""Reproducible benchmark runs for the SubzeroECS v2 spike.

Builds the benchmark preset, fingerprints the machine, runs suites from
suites.json under a profile, and writes a self-describing result directory:

    <out>/<host>/<YYYYmmdd-HHMMSS>-<git sha>[-<label>]/
        meta.json          machine, OS, compiler, flags, git, run config, warnings
        <suite>.json       raw Google Benchmark JSON (or timeline JSON)
        <suite>.log        stdout/stderr of the run
        summary.md         median / CV table per suite

Typical use on dedicated hardware:
    python3 tools/bench/run.py --profile reference --pin 2-15 --label ref-box
    python3 tools/bench/compare.py <runA> <runB>

Only the Python standard library is used.
"""
import argparse
import datetime as dt
import json
import os
import platform
import re
import shutil
import socket
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
SPIKE = HERE.parent.parent            # spike/v2
REPO = SPIKE.parent.parent            # repository root


def sh(cmd, **kw):
    """Run a command, return stdout (stripped) or '' on failure."""
    try:
        return subprocess.run(cmd, capture_output=True, text=True, check=False, **kw).stdout.strip()
    except (OSError, ValueError):
        return ""


def read(path):
    try:
        return Path(path).read_text().strip()
    except OSError:
        return None


# ---------------------------------------------------------------------------
# Machine fingerprint
# ---------------------------------------------------------------------------
def cpu_info():
    info = {"logical_cpus": os.cpu_count(), "machine": platform.machine(), "processor": platform.processor()}
    if shutil.which("lscpu"):
        try:
            rows = json.loads(sh(["lscpu", "-J"]))["lscpu"]
            flat = {r["field"].rstrip(":"): r.get("data") for r in rows}
            for key in ("Model name", "Architecture", "CPU(s)", "Thread(s) per core", "Core(s) per socket",
                        "Socket(s)", "NUMA node(s)", "CPU max MHz", "CPU min MHz", "L1d cache", "L1i cache",
                        "L2 cache", "L3 cache", "Hypervisor vendor", "Virtualization type", "Flags"):
                if key in flat:
                    info[key] = flat[key] if key != "Flags" else " ".join(
                        f for f in (flat[key] or "").split() if f in ("avx", "avx2", "avx512f", "fma", "sse4_2", "neon", "asimd"))
        except (ValueError, KeyError):
            pass
    elif sys.platform == "darwin":
        info["Model name"] = sh(["sysctl", "-n", "machdep.cpu.brand_string"])
    return info


def power_state():
    """Frequency governor / turbo / SMT: the usual sources of benchmark noise (Linux)."""
    state = {}
    govs = sorted({read(p) for p in Path("/sys/devices/system/cpu").glob("cpu[0-9]*/cpufreq/scaling_governor")} - {None})
    if govs:
        state["governor"] = govs
    no_turbo = read("/sys/devices/system/cpu/intel_pstate/no_turbo")
    if no_turbo is not None:
        state["intel_no_turbo"] = no_turbo
    boost = read("/sys/devices/system/cpu/cpufreq/boost")
    if boost is not None:
        state["cpufreq_boost"] = boost
    smt = read("/sys/devices/system/cpu/smt/active")
    if smt is not None:
        state["smt_active"] = smt
    aslr = read("/proc/sys/kernel/randomize_va_space")
    if aslr is not None:
        state["aslr"] = aslr
    try:
        state["loadavg"] = list(os.getloadavg())
    except OSError:
        pass
    return state


def memory_info():
    mem = read("/proc/meminfo")
    if mem:
        m = re.search(r"MemTotal:\s+(\d+) kB", mem)
        if m:
            return {"total_gib": round(int(m.group(1)) / 1024 / 1024, 1)}
    return {}


def git_info():
    def g(*a):
        return sh(["git", "-C", str(REPO), *a])
    return {"sha": g("rev-parse", "HEAD"), "branch": g("rev-parse", "--abbrev-ref", "HEAD"),
            "dirty": bool(g("status", "--porcelain", "--untracked-files=no")), "describe": g("describe", "--always", "--dirty")}


def build_info(build_dir):
    cache = read(Path(build_dir) / "CMakeCache.txt") or ""
    def var(name):
        m = re.search(rf"^{name}(?::\w+)?=(.*)$", cache, re.M)
        return m.group(1) if m else None
    cxx = var("CMAKE_CXX_COMPILER")
    return {"build_dir": str(build_dir), "build_type": var("CMAKE_BUILD_TYPE"), "cxx": cxx,
            "cxx_version": sh([cxx, "--version"]).splitlines()[0] if cxx else None,
            "cxx_flags": var("CMAKE_CXX_FLAGS"), "cxx_flags_release": var("CMAKE_CXX_FLAGS_RELEASE"),
            "spike_native": var("SPIKE_NATIVE"), "generator": var("CMAKE_GENERATOR")}


def warnings_for(meta):
    w = []
    p = meta["power"]
    if "governor" in p and any(g != "performance" for g in p["governor"]):
        w.append(f"CPU governor is {p['governor']} (use 'performance' for stable numbers)")
    if p.get("intel_no_turbo") == "0" or p.get("cpufreq_boost") == "1":
        w.append("turbo/boost enabled: frequency varies with temperature and load")
    if p.get("smt_active") == "1":
        w.append("SMT active: sibling threads share cores (thread-scaling results are per logical CPU)")
    if p.get("aslr") not in (None, "0") and not meta["run"].get("no_aslr"):
        w.append("ASLR enabled: small run-to-run layout noise")
    if "loadavg" in p and meta["cpu"].get("logical_cpus") and p["loadavg"][0] > 0.25 * meta["cpu"]["logical_cpus"]:
        w.append(f"system load {p['loadavg'][0]:.1f} before the run: other work may perturb results")
    if meta["cpu"].get("Hypervisor vendor"):
        w.append(f"virtualised host ({meta['cpu']['Hypervisor vendor']}): expect noisy neighbours")
    if meta["git"].get("dirty"):
        w.append("working tree has uncommitted changes: results are not reproducible from the SHA alone")
    return w


# ---------------------------------------------------------------------------
# Running
# ---------------------------------------------------------------------------
def build(preset):
    subprocess.run(["cmake", "--preset", preset], cwd=SPIKE, check=True)
    subprocess.run(["cmake", "--build", "--preset", preset], cwd=SPIKE, check=True)


def run_suite(name, suite, profile, build_dir, run_dir, pin, extra_env, no_aslr=False):
    exe = Path(build_dir) / suite["exe"]
    if not exe.exists():
        print(f"  ! {name}: {exe} not built, skipped")
        return {"status": "missing"}
    env = dict(os.environ)
    env.update(profile.get("env", {}))
    env.update(suite.get("env", {}))
    env.update(extra_env)
    prefix = ["taskset", "-c", pin] if pin and shutil.which("taskset") else []
    if no_aslr and shutil.which("setarch"):
        prefix = ["setarch", platform.machine(), "-R", *prefix]
    out = run_dir / f"{name}.json"
    if suite["kind"] == "gbench":
        cmd = prefix + [str(exe), f"--benchmark_filter={suite['filter']}",
                        f"--benchmark_repetitions={profile['repetitions']}",
                        "--benchmark_enable_random_interleaving=true",
                        "--benchmark_report_aggregates_only=true",
                        f"--benchmark_out={out}", "--benchmark_out_format=json"]
        if not suite.get("fixed_iterations"):
            cmd.append(f"--benchmark_min_time={profile['min_time']}")
    else:
        cmd = prefix + [str(exe), *suite.get("args", []), str(out)]
    print(f"  > {name}: {' '.join(cmd[-8:])}")
    t0 = dt.datetime.now()
    with open(run_dir / f"{name}.log", "w") as log:
        rc = subprocess.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT).returncode
    secs = (dt.datetime.now() - t0).total_seconds()
    print(f"    {'ok' if rc == 0 else f'FAILED rc={rc}'} in {secs:.0f}s")
    return {"status": "ok" if rc == 0 else "failed", "returncode": rc, "seconds": round(secs, 1),
            "command": cmd, "env": {k: env[k] for k in {**profile.get("env", {}), **suite.get("env", {}), **extra_env}}}


def summarize(run_dir, results):
    lines = [f"# Benchmark run {run_dir.name}\n"]
    for name, res in results.items():
        path = run_dir / f"{name}.json"
        lines.append(f"\n## {name} ({res['status']})\n")
        if res["status"] != "ok" or not path.exists():
            continue
        data = json.loads(path.read_text())
        if "benchmarks" not in data:   # timeline tool
            rows = data.get("rows", [])
            if rows:
                keys = list(rows[0].keys())
                lines.append("| " + " | ".join(keys) + " |")
                lines.append("|" + "---|" * len(keys))
                for r in rows:
                    lines.append("| " + " | ".join(str(r[k]) for k in keys) + " |")
            continue
        med, cv = {}, {}
        has_aggregates = any(b.get("aggregate_name") for b in data["benchmarks"])
        for b in data["benchmarks"]:
            if b.get("aggregate_name") == "median" or (not has_aggregates and b.get("run_type") == "iteration"):
                med[b.get("run_name", b["name"])] = (b["real_time"], b.get("time_unit", "ns"))
            elif b.get("aggregate_name") == "cv":
                cv[b["run_name"]] = b["real_time"]
        lines.append("| Benchmark | Median | CV |")
        lines.append("|---|---:|---:|")
        for k, (t, u) in med.items():
            c = cv.get(k)
            lines.append(f"| {k} | {t:.4g} {u} | {'' if c is None else f'{100 * c:.1f}%'} |")
    (run_dir / "summary.md").write_text("\n".join(lines) + "\n")


def main():
    cfg = json.loads((HERE / "suites.json").read_text())
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--profile", default="standard", choices=sorted(cfg["profiles"]))
    ap.add_argument("--suites", default="", help="comma list (default: the profile's suites); 'list' to show")
    ap.add_argument("--preset", default="bench-native", help="CMake preset (bench-native | bench-portable)")
    ap.add_argument("--build-dir", default=None, help="use an existing build dir (skips building)")
    ap.add_argument("--skip-build", action="store_true")
    ap.add_argument("--out", default=str(SPIKE / "results" / "runs"))
    ap.add_argument("--label", default="")
    ap.add_argument("--pin", default="", help="taskset CPU list, e.g. 2-15 (Linux)")
    ap.add_argument("--no-aslr", action="store_true", help="run benchmarks under 'setarch -R' (Linux)")
    ap.add_argument("--repetitions", type=int, default=None, help="override the profile")
    ap.add_argument("--env", action="append", default=[], help="extra KEY=VALUE for the benchmark processes")
    args = ap.parse_args()

    if args.suites == "list":
        for n, s in cfg["suites"].items():
            print(f"{n:12s} {s['description']}")
        return 0

    profile = dict(cfg["profiles"][args.profile])
    if args.repetitions:
        profile["repetitions"] = args.repetitions
    suites = [s for s in (args.suites.split(",") if args.suites else profile["suites"]) if s]
    unknown = [s for s in suites if s not in cfg["suites"]]
    if unknown:
        ap.error(f"unknown suites: {unknown}")

    build_dir = Path(args.build_dir) if args.build_dir else REPO / "build" / f"spike-{args.preset}"
    if not args.build_dir and not args.skip_build:
        build(args.preset)

    meta = {
        "schema": "sub0ecs-spike-bench/1",
        "timestamp_utc": dt.datetime.now(dt.timezone.utc).isoformat(timespec="seconds"),
        "label": args.label,
        "host": socket.gethostname(),
        "os": {"system": platform.system(), "release": platform.release(), "version": platform.version(),
               "libc": " ".join(platform.libc_ver()), "python": platform.python_version()},
        "cpu": cpu_info(),
        "memory": memory_info(),
        "power": power_state(),
        "git": git_info(),
        "build": build_info(build_dir),
        "run": {"profile": args.profile, "profile_settings": profile, "suites": suites, "pin": args.pin,
                "no_aslr": args.no_aslr,
                "extra_env": args.env},
    }
    meta["warnings"] = warnings_for(meta)

    host = re.sub(r"[^A-Za-z0-9_.-]", "_", meta["host"])[:40]
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    run_dir = Path(args.out) / host / f"{stamp}-{meta['git']['sha'][:7] or 'nogit'}{'-' + args.label if args.label else ''}"
    run_dir.mkdir(parents=True, exist_ok=True)
    print(f"run dir: {run_dir}")
    for w in meta["warnings"]:
        print(f"  warning: {w}")

    extra_env = dict(kv.split("=", 1) for kv in args.env)
    results = {}
    for name in suites:
        results[name] = run_suite(name, cfg["suites"][name], profile, build_dir, run_dir, args.pin, extra_env, args.no_aslr)
    meta["results"] = results
    (run_dir / "meta.json").write_text(json.dumps(meta, indent=2) + "\n")
    summarize(run_dir, results)
    print(f"done: {run_dir / 'summary.md'}")
    return 0 if all(r["status"] == "ok" for r in results.values()) else 1


if __name__ == "__main__":
    sys.exit(main())
