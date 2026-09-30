#!/usr/bin/env python3
"""Summarise spike benchmark JSON into markdown tables.

Usage: summarize.py results.json [baseline_design=V1] > RESULTS.md

Rows: <Scenario>/<Pattern>/<N>; columns: designs. Cells show median time per
iteration and speed-up vs the baseline design (>1.00x = faster than baseline).
"""
import json
import sys
from collections import defaultdict

DESIGN_ORDER = ["V1", "SortedSoA", "SparseSet", "Archetype", "QueryPart", "QPartHinted", "StaticBitmask", "RawSoA"]
SCENARIO_ORDER = ["Create", "Iter1", "Update2", "Frame3", "SparseQuery", "RandomGet", "AddRemove", "TagChurn", "DestroyCreate"]


def fmt_time(us):
    if us >= 1000.0:
        return f"{us / 1000.0:.2f} ms"
    return f"{us:.2f} µs"


def main():
    path = sys.argv[1]
    baseline = sys.argv[2] if len(sys.argv) > 2 else "V1"
    data = json.load(open(path))

    medians = {}
    cv = {}
    counters = defaultdict(dict)
    skipped = set()
    for b in data["benchmarks"]:
        name = b.get("run_name", b["name"])
        parts = name.split("/")
        if len(parts) < 4:
            continue
        scenario, pattern, design, n = parts[0], parts[1], parts[2], int(parts[3])
        key = (scenario, pattern, n)
        if b.get("error_occurred"):
            skipped.add((key, design))
            continue
        agg = b.get("aggregate_name")
        if agg == "median":
            t = b["real_time"]
            unit = b.get("time_unit", "us")
            t_us = t * {"ns": 1e-3, "us": 1.0, "ms": 1e3, "s": 1e6}[unit]
            medians[(key, design)] = t_us
            for c in ("bytes_per_entity", "allocs_per_entity"):
                if c in b:
                    counters[(key, design)][c] = b[c]
        elif agg == "cv":
            cv[(key, design)] = b["real_time"]

    ctx = data.get("context", {})
    print("# SubzeroECS v2 spike — benchmark baseline\n")
    print(f"- Host: {ctx.get('num_cpus')} × {ctx.get('mhz_per_cpu')} MHz, "
          f"caches: " + ", ".join(f"L{c['level']} {c['type'][0]} {c['size'] // 1024} KiB" for c in ctx.get("caches", [])))
    print(f"- Date: {ctx.get('date')}  |  Build: {ctx.get('library_build_type')}  |  "
          f"Statistic: median of repetitions; cell = time/iteration (speed-up vs {baseline})")
    print("- `—` = unsupported by that design; `n/a` = not run for that design\n")

    keys = sorted({k for (k, _) in list(medians) + list(skipped)},
                  key=lambda k: (SCENARIO_ORDER.index(k[0]) if k[0] in SCENARIO_ORDER else 99, k[1], k[2]))
    designs = [d for d in DESIGN_ORDER if any(dd == d for (_, dd) in list(medians) + list(skipped))]

    current = None
    for key in keys:
        scenario = key[0]
        if scenario != current:
            current = scenario
            print(f"\n## {scenario}\n")
            print("| Pattern | N | " + " | ".join(designs) + " |")
            print("|---|---:|" + "---:|" * len(designs))
        base = medians.get((key, baseline))
        cells = []
        for d in designs:
            if (key, d) in skipped:
                cells.append("—")
                continue
            t = medians.get((key, d))
            if t is None:
                cells.append("n/a")
                continue
            s = fmt_time(t)
            if base and d != baseline:
                s += f" ({base / t:.2f}×)"
            noisy = cv.get((key, d), 0.0)
            if noisy > 0.10:
                s += " ⚠"
            cells.append(s)
        print(f"| {key[1]} | {key[2]:,} | " + " | ".join(cells) + " |")

    # Memory table from Create
    print("\n## Memory (from Create)\n")
    print("Live heap bytes per entity after populate (world only, excludes handle list), "
          "and heap allocations per entity.\n")
    print("| Pattern | N | " + " | ".join(d for d in designs if d != "RawSoA") + " |")
    print("|---|---:|" + "---:|" * len([d for d in designs if d != "RawSoA"]))
    for key in keys:
        if key[0] != "Create":
            continue
        cells = []
        for d in designs:
            if d == "RawSoA":
                continue
            c = counters.get((key, d))
            cells.append(f"{c['bytes_per_entity']:.1f} B / {c['allocs_per_entity']:.3f}" if c else "n/a")
        print(f"| {key[1]} | {key[2]:,} | " + " | ".join(cells) + " |")
    print("\n⚠ = coefficient of variation > 10% across repetitions (noisy host).")


if __name__ == "__main__":
    main()
