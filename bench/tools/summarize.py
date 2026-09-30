#!/usr/bin/env python3
"""Summarise benchmark results (schema sub0ecs-bench-results/1) as markdown.

    summarize.py <results.json | run-dir> [...] > SUMMARY.md

One table per group (<Scenario>/<Pattern>/<N>). "vs baseline" is nanobench's
paired comparison: t_baseline / t_design, >1.00x = faster than the group's
baseline, with a 95% interval corrected for the group's comparisons; bold when
the interval excludes 1. Only the Python standard library is used.
"""
import json
import sys
from pathlib import Path

SCHEMA = "sub0ecs-bench-results/1"


def load(paths):
    """Records from result files and/or run directories, in file order."""
    files = []
    for p in map(Path, paths):
        files += sorted(f for f in p.glob("*.json") if f.name != "meta.json") if p.is_dir() else [p]
    records = []
    for f in files:
        data = json.loads(f.read_text())
        if data.get("schema") == SCHEMA:
            records += data["results"]
    return records


def fmt_time(ns):
    for unit, scale in (("s", 1e9), ("ms", 1e6), ("µs", 1e3)):
        if ns >= scale:
            return f"{ns / scale:.3g} {unit}"
    return f"{ns:.3g} ns"


def fmt_rate(per_second):
    for unit, scale in (("G", 1e9), ("M", 1e6), ("k", 1e3)):
        if per_second >= scale:
            return f"{per_second / scale:.3g} {unit}"
    return f"{per_second:.3g}"


def paired_cell(r):
    p = r.get("paired")
    if not p:
        return ""
    if p["baseline"] == r["design"]:
        return "baseline"
    cell = f"{p['ratio']:.2f}× [{p['low']:.2f}–{p['high']:.2f}]"
    return f"**{cell}**" if p["significant"] else cell


def extras(r):
    parts = [f"{k}={v:.4g}" for k, v in r.get("counters", {}).items()]
    parts += [f"{k}: {v}" for k, v in r.get("notes", {}).items()]
    return ", ".join(parts)


def summarize(records):
    lines = []
    groups = {}
    for r in records:
        groups.setdefault(r["group"], []).append(r)
    for group, rows in groups.items():
        unit = rows[0].get("unit", "op")
        lines += [f"## {group}", "", f"| Design | time/{unit} | err% | vs baseline | items/s | notes |",
                  "|---|---:|---:|---:|---:|---|"]
        for r in rows:
            lines.append(f"| {r['design']} | {fmt_time(r['median_ns'])} | {r['err_pct']:.1f}% | {paired_cell(r)} | "
                         f"{fmt_rate(r['items_per_second'])} | {extras(r)} |")
        lines.append("")
    return "\n".join(lines)


def main():
    if len(sys.argv) < 2:
        print(__doc__, file=sys.stderr)
        return 2
    records = load(sys.argv[1:])
    if not records:
        print(f"no {SCHEMA} results found", file=sys.stderr)
        return 1
    print(summarize(records))
    return 0


if __name__ == "__main__":
    sys.exit(main())
