#!/usr/bin/env python3
"""Compare builds (compilers, branches, flags) on one machine, robustly.

One run of one build says little: the machine is shared, it heats up, and other
work comes and goes. This takes several samples of every build, interleaved:

    python bench/tools/rotate.py --build msvc=build/msvc --build gcc=build/gcc \
        --rounds 5 --suites baseline --pin P --label compilers

- Each round runs every build once; the starting build rotates, so heat and
  drift do not land on the same build every time.
- Before each sample it pauses (--cooldown) and then waits until the CPU is
  quiet (--max-load); the load before and after is recorded with the sample.
- A sample taken on a busy machine is kept but marked, and left out of the
  figures while at least two clean samples of that build remain.

Writes <out>/<host>/<stamp>-<sha>[-label]/rotation.json (every sample) and
rotation.md: per case and build, the median time and its spread over the
samples, and the median of the within-run ratio to the group's baseline with
its range. Trust the ratio, and trust neither when the spread is wide.
Builds are not rebuilt here; build them first. Standard library only.
"""
import argparse
import datetime as dt
import json
import re
import socket
import statistics
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run as bench   # bench/tools/run.py


def cpu_load():
    """Current CPU load in percent, or None if it cannot be read."""
    if bench.WINDOWS:
        return bench.windows_power_state().get("cpu_load_percent")
    try:
        import os
        return round(100.0 * os.getloadavg()[0] / (os.cpu_count() or 1), 1)
    except (AttributeError, OSError):
        return None


def wait_until_quiet(max_load, timeout):
    """Poll until the load is at most max_load; returns (load, seconds waited, quiet?)."""
    start = time.monotonic()
    while True:
        load = cpu_load()
        if load is None or load <= max_load:
            return load, round(time.monotonic() - start), True
        if time.monotonic() - start >= timeout:
            return load, round(time.monotonic() - start), False
        time.sleep(5)


def load_results(run_dir):
    """{case name: (median ns, paired ratio or None, baseline design)} of one run."""
    rows = {}
    for f in Path(run_dir).glob("*.json"):
        if f.name == "meta.json":
            continue
        for r in json.loads(f.read_text(encoding="utf-8")).get("results", []):
            paired = r.get("paired") or {}
            rows[r["name"]] = (r["median_ns"], paired.get("ratio"), paired.get("baseline"))
    return rows


def unit(ns):
    for scale, name in ((1e9, "s"), (1e6, "ms"), (1e3, "µs")):
        if ns >= scale:
            return f"{ns / scale:.3g} {name}"
    return f"{ns:.3g} ns"


def report(samples, labels, case_filter):
    """Markdown: for every case, each build's time (median, spread) and ratio to baseline (median, range)."""
    usable = {}
    for label in labels:
        mine = [s for s in samples if s["build"] == label and s["status"] == "ok"]
        clean = [s for s in mine if s["quiet"]]
        usable[label] = clean if len(clean) >= 2 else mine
    lines = ["| Build | Samples used | Left out (busy machine) |", "|---|---:|---:|"]
    for label in labels:
        total = sum(1 for s in samples if s["build"] == label and s["status"] == "ok")
        lines.append(f"| {label} | {len(usable[label])} | {total - len(usable[label])} |")
    lines.append("")

    cases = sorted({name for label in labels for s in usable[label] for name in s["results"]})
    cases = [c for c in cases if case_filter.search(c)]
    head = "| Case | " + " | ".join(f"{label} time (spread)" for label in labels) + " | " + " | ".join(f"{label} vs baseline (range)" for label in labels) + " |"
    lines += [head, "|---|" + "---:|" * (2 * len(labels))]
    for case in cases:
        times, ratios = [], []
        for label in labels:
            t = [s["results"][case][0] for s in usable[label] if case in s["results"]]
            r = [s["results"][case][1] for s in usable[label] if case in s["results"] and s["results"][case][1]]
            if not t:
                times.append("")
                ratios.append("")
                continue
            median = statistics.median(t)
            times.append(f"{unit(median)} (±{100 * (max(t) - min(t)) / (2 * median):.0f}%)")
            ratios.append(f"{statistics.median(r):.2f}× ({min(r):.2f}–{max(r):.2f})" if r else "baseline")
        lines.append(f"| {case} | " + " | ".join(times) + " | " + " | ".join(ratios) + " |")
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--build", action="append", required=True, metavar="LABEL=DIR", help="a build to compare; give at least two")
    ap.add_argument("--rounds", type=int, default=5, help="samples per build (default 5; never fewer than 3)")
    ap.add_argument("--profile", default="compare", help="suites.json profile (default: compare, sized for many samples)")
    ap.add_argument("--suites", default="baseline")
    ap.add_argument("--pin", default="")
    ap.add_argument("--cooldown", type=int, default=20, help="seconds to pause before every sample")
    ap.add_argument("--max-load", type=float, default=10.0, help="CPU load (percent) above which the machine counts as busy")
    ap.add_argument("--quiet-timeout", type=int, default=180, help="seconds to wait for a quiet machine before sampling anyway")
    ap.add_argument("--filter", default=".", help="regex on case names for the report")
    ap.add_argument("--label", default="")
    ap.add_argument("--out", default=str(bench.BENCH / "results" / "runs"))
    args = ap.parse_args()
    if args.rounds < 3:
        ap.error("--rounds must be at least 3: fewer samples cannot show whether a difference is repeatable")
    sys.stdout.reconfigure(encoding="utf-8")

    builds = dict(b.split("=", 1) for b in args.build)
    labels = list(builds)
    host = re.sub(r"[^A-Za-z0-9_.-]", "_", socket.gethostname())[:40]
    sha = bench.git_info()["sha"][:7] or "nogit"
    stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S")
    out = Path(args.out) / host / f"{stamp}-{sha}-rotation{'-' + args.label if args.label else ''}"
    out.mkdir(parents=True, exist_ok=True)
    print(f"rotation dir: {out}")

    import os
    one_core = 100.0 / (os.cpu_count() or 1)   # the benchmark's own thread, as a share of the machine
    samples = []
    for round_index in range(args.rounds):
        order = labels[round_index % len(labels):] + labels[:round_index % len(labels)]
        for label in order:
            time.sleep(args.cooldown)
            before, waited, quiet = wait_until_quiet(args.max_load, args.quiet_timeout)
            cmd = [sys.executable, str(bench.HERE / "run.py"), "--build-dir", builds[label], "--profile", args.profile,
                   "--suites", args.suites, "--out", str(out / "samples"), "--label", f"r{round_index + 1}-{label}"]
            if args.pin:
                cmd += ["--pin", args.pin]
            proc = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", errors="replace")
            after = cpu_load()
            # The benchmark is one pinned thread, so a quiet machine stays well under max-load during it too.
            quiet = quiet and (after is None or after <= args.max_load + one_core)
            found = re.search(r"run dir: (.+)", proc.stdout)
            sample = {"round": round_index + 1, "build": label, "status": "ok" if proc.returncode == 0 and found else "failed",
                      "load_before": before, "load_after": after, "waited_s": waited, "quiet": quiet,
                      "run_dir": found.group(1).strip() if found else None}
            sample["results"] = load_results(sample["run_dir"]) if sample["status"] == "ok" else {}
            samples.append(sample)
            print(f"  round {round_index + 1} {label:12s} {sample['status']}  load {before}% -> {after}%"
                  f"{'' if quiet else '  BUSY'}{f'  (waited {waited}s)' if waited else ''}")
            (out / "rotation.json").write_text(json.dumps({"schema": "sub0ecs-bench-rotation/1", "builds": builds,
                                                           "settings": vars(args), "samples": samples}, indent=1) + "\n", encoding="utf-8")

    text = f"# Rotation {out.name}\n\n{args.rounds} rounds, builds rotated, pin `{args.pin or 'none'}`, profile `{args.profile}`.\n\n"
    text += report(samples, labels, re.compile(args.filter))
    (out / "rotation.md").write_text(text, encoding="utf-8")
    print(f"done: {out / 'rotation.md'}")
    return 0 if all(s["status"] == "ok" for s in samples) else 1


if __name__ == "__main__":
    sys.exit(main())
