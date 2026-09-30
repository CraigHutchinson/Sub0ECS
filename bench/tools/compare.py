#!/usr/bin/env python3
"""Compare two benchmark runs produced by run.py (A = baseline, B = candidate).

    python3 tools/bench/compare.py <runA> <runB> [--threshold 0.05] [--fail-on-regression]

For every benchmark present in both runs the median times are compared.
A change counts as significant only if it exceeds both the threshold and the
measured noise (2 x the larger coefficient of variation of the two runs).
Machine differences between the runs (CPU, compiler, flags, git) are listed
first, so cross-machine comparisons are never mistaken for code changes.
"""
import argparse
import json
import sys
from pathlib import Path


def load_run(d):
    d = Path(d)
    meta = json.loads((d / "meta.json").read_text())
    suites = {}
    for f in d.glob("*.json"):
        if f.name == "meta.json":
            continue
        data = json.loads(f.read_text())
        if "benchmarks" not in data:
            continue
        rows = {}
        has_aggregates = any(b.get("aggregate_name") for b in data["benchmarks"])
        for b in data["benchmarks"]:
            agg = b.get("aggregate_name")
            if agg in ("median", "cv"):
                rows.setdefault(b["run_name"], {})[agg] = b["real_time"]
            elif not has_aggregates and b.get("run_type") == "iteration":   # repetitions=1: single sample
                rows.setdefault(b.get("run_name", b["name"]), {})["median"] = b["real_time"]
        suites[f.stem] = rows
    return meta, suites


def machine_diff(ma, mb):
    keys = [("cpu", "Model name"), ("cpu", "logical_cpus"), ("build", "cxx_version"), ("build", "cxx_flags_release"),
            ("build", "spike_native"), ("os", "release"), ("git", "sha"), ("git", "dirty"), ("run", "profile")]
    out = []
    for a, b in keys:
        va, vb = ma.get(a, {}).get(b), mb.get(a, {}).get(b)
        if va != vb:
            out.append(f"| {a}.{b} | {va} | {vb} |")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--threshold", type=float, default=0.05, help="minimum relative change to report (default 5%%)")
    ap.add_argument("--fail-on-regression", action="store_true", help="exit 1 if any significant slowdown")
    args = ap.parse_args()

    ma, sa = load_run(args.a)
    mb, sb = load_run(args.b)
    print(f"# Benchmark comparison\n\nA: `{args.a}`  \nB: `{args.b}`\n")
    diff = machine_diff(ma, mb)
    if diff:
        print("## Environment differences (not code changes!)\n\n| Field | A | B |\n|---|---|---|")
        print("\n".join(diff) + "\n")

    regressions = 0
    for suite in sorted(set(sa) & set(sb)):
        ra, rb = sa[suite], sb[suite]
        common = [k for k in ra if k in rb and "median" in ra[k] and "median" in rb[k]]
        if not common:
            continue
        print(f"## {suite}\n\n| Benchmark | A | B | B vs A | Verdict |\n|---|---:|---:|---:|---|")
        for k in common:
            ta, tb = ra[k]["median"], rb[k]["median"]
            speedup = ta / tb if tb else float("inf")
            noise = 2 * max(ra[k].get("cv", 0.0), rb[k].get("cv", 0.0))
            change = abs(speedup - 1.0)
            if change <= max(args.threshold, noise):
                verdict = "≈ (within noise)"
            elif speedup > 1:
                verdict = "**faster**"
            else:
                verdict = "**SLOWER**"
                regressions += 1
            print(f"| {k} | {ta:.4g} | {tb:.4g} | {speedup:.2f}× | {verdict} |")
        print()
    only_a = set(sa) - set(sb)
    only_b = set(sb) - set(sa)
    if only_a or only_b:
        print(f"Suites only in A: {sorted(only_a)}; only in B: {sorted(only_b)}")
    print(f"\n{regressions} significant regression(s).")
    return 1 if args.fail_on_regression and regressions else 0


if __name__ == "__main__":
    sys.exit(main())
