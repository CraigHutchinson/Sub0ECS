#!/usr/bin/env python3
"""Reproducible benchmark runs for SubzeroECS v2 and its comparators.

Builds the benchmark preset, fingerprints the machine, runs suites from
suites.json under a profile, and writes a self-describing result directory:

    <out>/<host>/<YYYYmmdd-HHMMSS>-<git sha>[-<label>]/
        meta.json          machine, OS, compiler, flags, git, run config, warnings
        <suite>.json       raw Google Benchmark JSON (or timeline JSON)
        <suite>.log        stdout/stderr of the run
        summary.md         median / CV table per suite

Typical use on dedicated hardware:
    python3 bench/tools/run.py --profile reference --pin 2-15 --label ref-box
    python3 bench/tools/compare.py <runA> <runB>

Linux, macOS and Windows (MSVC: run from a VS developer prompt so the
Ninja presets find cl.exe). Only the Python standard library is used.
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
BENCH = HERE.parent                   # bench/
REPO = BENCH.parent                   # repository root
WINDOWS = sys.platform == "win32"
EXE = ".exe" if WINDOWS else ""


def sh(cmd, **kw):
    """Run a command, return stdout (stripped) or '' on failure."""
    try:
        return subprocess.run(cmd, capture_output=True, text=True, check=False, **kw).stdout.strip()
    except (OSError, ValueError):
        return ""


def powershell(expr):
    """Evaluate a PowerShell expression and parse its output as JSON (Windows fingerprinting)."""
    out = sh(["powershell", "-NoProfile", "-NonInteractive", "-Command", f"{expr} | ConvertTo-Json -Compress"])
    try:
        return json.loads(out) if out else None
    except ValueError:
        return None


def parse_cpulist(text):
    """'0-3,8,10-11' -> [0, 1, 2, 3, 8, 10, 11] (taskset syntax)."""
    cpus = []
    for part in filter(None, text.split(",")):
        lo, _, hi = part.partition("-")
        cpus.extend(range(int(lo), int(hi or lo) + 1))
    return cpus


def cpulist(cpus):
    """[0, 1, 2, 3, 8] -> '0-3,8'."""
    runs, start = [], None
    for i, c in enumerate(cpus):
        if start is None:
            start = c
        if i + 1 == len(cpus) or cpus[i + 1] != c + 1:
            runs.append(f"{start}" if start == c else f"{start}-{c}")
            start = None
    return ",".join(runs)


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
    elif WINDOWS:
        info.update(windows_cpu_info())
    return info


def windows_cpu_info():
    import ctypes
    info = {}
    cpu = powershell("Get-CimInstance Win32_Processor | Select-Object Name,NumberOfCores,"
                     "NumberOfLogicalProcessors,MaxClockSpeed,L2CacheSize,L3CacheSize")
    if isinstance(cpu, list):
        info["Socket(s)"] = len(cpu)
        cpu = cpu[0]
    if cpu:
        info.update({"Model name": cpu["Name"].strip(), "Core(s) per socket": cpu["NumberOfCores"],
                     "CPU(s)": cpu["NumberOfLogicalProcessors"], "CPU max MHz": cpu["MaxClockSpeed"],
                     "L2 cache": f"{cpu['L2CacheSize']} KiB", "L3 cache": f"{cpu['L3CacheSize']} KiB",
                     "Thread(s) per core": cpu["NumberOfLogicalProcessors"] // max(1, cpu["NumberOfCores"])})
    present = ctypes.windll.kernel32.IsProcessorFeaturePresent
    features = {"sse4_2": 38, "avx": 39, "avx2": 40, "avx512f": 41}   # PF_*_INSTRUCTIONS_AVAILABLE
    info["Flags"] = " ".join(name for name, pf in features.items() if present(pf))
    info.update(windows_core_classes())
    system = powershell("Get-CimInstance Win32_ComputerSystem | Select-Object Manufacturer,Model")
    if system and any(v in f"{system['Manufacturer']} {system['Model']}"
                      for v in ("Virtual", "VMware", "QEMU", "KVM", "Xen", "Parallels")):
        info["Hypervisor vendor"] = f"{system['Manufacturer']} {system['Model']}"
    return info


def windows_core_classes():
    """Logical CPUs per efficiency class on a hybrid CPU; the highest class is the P-cores.

    GetLogicalProcessorInformationEx(RelationProcessorCore) returns one record per
    physical core: {DWORD Relationship, DWORD Size}, then PROCESSOR_RELATIONSHIP
    {BYTE Flags, BYTE EfficiencyClass, BYTE Reserved[20], WORD GroupCount,
    GROUP_AFFINITY GroupMask[]} with GROUP_AFFINITY {KAFFINITY Mask, WORD Group, ...}
    starting at record offset 32 (8-byte aligned).
    """
    import ctypes
    k32 = ctypes.windll.kernel32
    size = ctypes.c_ulong(0)
    k32.GetLogicalProcessorInformationEx(0, None, ctypes.byref(size))   # 0 = RelationProcessorCore
    buf = ctypes.create_string_buffer(size.value)
    if not size.value or not k32.GetLogicalProcessorInformationEx(0, buf, ctypes.byref(size)):
        return {}
    raw, off, classes = buf.raw, 0, {}
    while off < size.value:
        record_size = int.from_bytes(raw[off + 4:off + 8], "little")
        mask = int.from_bytes(raw[off + 32:off + 40], "little")
        group = int.from_bytes(raw[off + 40:off + 42], "little")
        classes.setdefault(raw[off + 9], []).extend(group * 64 + b for b in range(64) if mask >> b & 1)
        off += record_size
    if len(classes) < 2:
        return {}
    top = max(classes)
    return {"hybrid": {("P" if c == top else f"E{c}"): cpulist(sorted(v)) for c, v in sorted(classes.items(), reverse=True)}}


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
    except (AttributeError, OSError):   # AttributeError: not provided on Windows
        pass
    if WINDOWS:
        state.update(windows_power_state())
    return state


def windows_power_state():
    """Power plan, AC/battery and CPU load: the Windows counterparts of governor, turbo and loadavg."""
    import ctypes

    class SystemPowerStatus(ctypes.Structure):
        _fields_ = [("ACLineStatus", ctypes.c_ubyte), ("BatteryFlag", ctypes.c_ubyte),
                    ("BatteryLifePercent", ctypes.c_ubyte), ("SystemStatusFlag", ctypes.c_ubyte),
                    ("BatteryLifeTime", ctypes.c_ulong), ("BatteryFullLifeTime", ctypes.c_ulong)]
    state = {}
    m = re.search(r"\((.+)\)\s*$", sh(["powercfg", "/getactivescheme"]))
    if m:
        state["power_plan"] = m.group(1)
    status = SystemPowerStatus()
    if ctypes.windll.kernel32.GetSystemPowerStatus(ctypes.byref(status)):
        state["on_ac_power"] = {0: False, 1: True}.get(status.ACLineStatus)
    load = powershell("(Get-CimInstance Win32_Processor | Measure-Object -Property LoadPercentage -Average).Average")
    if load is not None:
        state["cpu_load_percent"] = load
    return state


def memory_info():
    mem = read("/proc/meminfo")
    if mem:
        m = re.search(r"MemTotal:\s+(\d+) kB", mem)
        if m:
            return {"total_gib": round(int(m.group(1)) / 1024 / 1024, 1)}
    if WINDOWS:
        total = powershell("(Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory")
        if total:
            return {"total_gib": round(total / 1024 ** 3, 1)}
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
    # CMake's own compiler detection: exact, and portable (cl.exe has no --version).
    detected = next(iter(sorted(Path(build_dir).glob("CMakeFiles/*/CMakeCXXCompiler.cmake"))), None)
    ident = (read(detected) or "") if detected else ""
    def ident_var(name):
        m = re.search(rf'^set\({name} "([^"]*)"\)', ident, re.M)
        return m.group(1) if m else ""
    version = f"{ident_var('CMAKE_CXX_COMPILER_ID')} {ident_var('CMAKE_CXX_COMPILER_VERSION')}".strip()
    return {"build_dir": str(build_dir), "build_type": var("CMAKE_BUILD_TYPE"), "cxx": var("CMAKE_CXX_COMPILER"),
            "cxx_version": version or None,
            "cxx_flags": var("CMAKE_CXX_FLAGS"), "cxx_flags_release": var("CMAKE_CXX_FLAGS_RELEASE"),
            "native": var("SUB0ECS_NATIVE"), "generator": var("CMAKE_GENERATOR")}


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
    if p.get("on_ac_power") is False:
        w.append("running on battery: the CPU is power-limited and throttles")
    if "power_plan" in p and not re.search(r"high performance|ultimate", p["power_plan"], re.I):
        w.append(f"power plan is '{p['power_plan']}' (High performance / Best performance gives steadier clocks)")
    if p.get("cpu_load_percent", 0) > 10:
        w.append(f"CPU load {p['cpu_load_percent']}% before the run: other work may perturb results")
    if "hybrid" in meta["cpu"] and not meta["run"].get("pin"):
        w.append(f"hybrid CPU {meta['cpu']['hybrid']} and no --pin: threads migrate between core types")
    if meta["cpu"].get("Hypervisor vendor"):
        w.append(f"virtualised host ({meta['cpu']['Hypervisor vendor']}): expect noisy neighbours")
    if meta["git"].get("dirty"):
        w.append("working tree has uncommitted changes: results are not reproducible from the SHA alone")
    return w


# ---------------------------------------------------------------------------
# Running
# ---------------------------------------------------------------------------
def build(preset):
    subprocess.run(["cmake", "--preset", preset], cwd=REPO, check=True)
    subprocess.run(["cmake", "--build", "--preset", preset], cwd=REPO, check=True)


def run_suite(name, suite, profile, build_dir, run_dir, pin, extra_env, no_aslr=False):
    exe = Path(build_dir) / (suite["exe"] + EXE)
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
    if suite["kind"] == "harness":   # bench/harness: nanobench, paired group comparisons
        cmd = prefix + [str(exe), f"--filter={suite['filter']}", f"--epochs={profile['epochs']}",
                        f"--min-epoch-ms={profile['min_epoch_ms']}", f"--out={out}"]
    else:
        cmd = prefix + [str(exe), *suite.get("args", []), str(out)]
    print(f"  > {name}: {' '.join(cmd[-8:])}")
    t0 = dt.datetime.now()
    with open(run_dir / f"{name}.log", "w") as log:
        proc = subprocess.Popen(cmd, env=env, stdout=log, stderr=subprocess.STDOUT)
        if pin and WINDOWS:
            pin_process(proc, pin)
        rc = proc.wait()
    secs = (dt.datetime.now() - t0).total_seconds()
    print(f"    {'ok' if rc == 0 else f'FAILED rc={rc}'} in {secs:.0f}s")
    return {"status": "ok" if rc == 0 else "failed", "returncode": rc, "seconds": round(secs, 1),
            "command": cmd, "env": {k: env[k] for k in {**profile.get("env", {}), **suite.get("env", {}), **extra_env}}}


def pin_process(proc, pin):
    """Windows counterpart of taskset: restrict a just-started process to the CPUs in `pin`.

    A process affinity mask also moves threads that already exist, and the
    benchmark binaries spend far longer in startup and registration than this
    call takes, so no measured work runs unpinned."""
    import ctypes
    mask = sum(1 << c for c in parse_cpulist(pin))
    if not ctypes.windll.kernel32.SetProcessAffinityMask(ctypes.c_void_p(int(proc._handle)), ctypes.c_size_t(mask)):
        print(f"    ! could not pin to {pin}")


def summarize(run_dir, results):
    """summary.md: per-suite status, then summarize.py's per-group tables."""
    sys.path.insert(0, str(HERE))
    import summarize as summary   # bench/tools/summarize.py
    lines = [f"# Benchmark run {run_dir.name}", ""]
    for name, res in results.items():
        lines.append(f"- {name}: {res['status']}" + (f" in {res['seconds']:.0f} s" if "seconds" in res else ""))
    records = summary.load([run_dir])
    lines += ["", summary.summarize(records) if records else "(no harness results)"]
    for name, res in results.items():   # timeline tools write their own {"rows": [...]}
        path = run_dir / f"{name}.json"
        if res["status"] != "ok" or not path.exists():
            continue
        rows = json.loads(path.read_text()).get("rows")
        if rows:
            keys = list(rows[0].keys())
            lines += [f"## {name}", "", "| " + " | ".join(keys) + " |", "|" + "---|" * len(keys)]
            lines += ["| " + " | ".join(str(r[k]) for k in keys) + " |" for r in rows] + [""]
    (run_dir / "summary.md").write_text("\n".join(lines) + "\n", encoding="utf-8")


def main():
    cfg = json.loads((HERE / "suites.json").read_text())
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--profile", default="standard", choices=sorted(cfg["profiles"]))
    ap.add_argument("--suites", default="", help="comma list (default: the profile's suites); 'list' to show")
    ap.add_argument("--preset", default="bench-native", help="CMake preset (bench-native | bench-portable)")
    ap.add_argument("--build-dir", default=None, help="use an existing build dir (skips building)")
    ap.add_argument("--skip-build", action="store_true")
    ap.add_argument("--out", default=str(BENCH / "results" / "runs"))
    ap.add_argument("--label", default="")
    ap.add_argument("--pin", default="", help="CPU list, e.g. 2-15 (taskset on Linux, affinity mask on Windows); "
                    "'P' = the performance cores of a hybrid CPU (Windows)")
    ap.add_argument("--no-aslr", action="store_true", help="run benchmarks under 'setarch -R' (Linux)")
    ap.add_argument("--epochs", type=int, default=None, help="override the profile's epochs (paired rounds)")
    ap.add_argument("--env", action="append", default=[], help="extra KEY=VALUE for the benchmark processes")
    args = ap.parse_args()

    if args.suites == "list":
        for n, s in cfg["suites"].items():
            print(f"{n:12s} {s['description']}")
        return 0

    profile = dict(cfg["profiles"][args.profile])
    if args.epochs:
        profile["epochs"] = args.epochs
    suites = [s for s in (args.suites.split(",") if args.suites else profile["suites"]) if s]
    unknown = [s for s in suites if s not in cfg["suites"]]
    if unknown:
        ap.error(f"unknown suites: {unknown}")

    build_dir = Path(args.build_dir) if args.build_dir else REPO / "build" / args.preset
    if not args.build_dir and not args.skip_build:
        build(args.preset)

    meta = {
        "schema": "sub0ecs-bench/1",
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
    if args.pin == "P":
        args.pin = meta["run"]["pin"] = meta["cpu"].get("hybrid", {}).get("P", "")
        if not args.pin:
            ap.error("--pin P needs a hybrid CPU with detectable performance cores")
    if WINDOWS and not (args.build_dir or args.skip_build) and not shutil.which("cl"):
        ap.error("cl.exe is not on PATH: run from a VS developer prompt so the Ninja presets find MSVC")
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
