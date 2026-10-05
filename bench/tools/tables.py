#!/usr/bin/env python3
"""Markdown tables from a rotate.py capture, for write-ups.

    python bench/tools/tables.py <rotation-dir> [--n=100000] [--pattern=Fragmented]

Speed relative to the reference design, per compiler: the median over the samples of the within-run
ratio t_reference / t_design, so > 1 is faster than the reference. Then the spread of every figure.
"""
import json, statistics, sys
from pathlib import Path

d = Path(sys.argv[1])
opts = dict(a[2:].split("=", 1) for a in sys.argv[2:] if a.startswith("--"))
n = opts.get("n", "100000")
pattern = opts.get("pattern", "Fragmented")
sys.stdout.reconfigure(encoding="utf-8")

j = json.loads((d / "rotation.json").read_text(encoding="utf-8"))
labels = list(j["builds"])
usable = {}
for label in labels:
    mine = [s for s in j["samples"] if s["build"] == label and s["status"] == "ok"]
    clean = [s for s in mine if s["quiet"]]
    usable[label] = clean if len(clean) >= 2 else mine
    print(f"<!-- {label}: {len(usable[label])} of {len(mine)} samples used -->")

def rel(label, scenario, design, reference):
    """(median, min, max) of t_reference / t_design over the samples, or None."""
    a, b = f"{scenario}/{pattern}/{design}/{n}", f"{scenario}/{pattern}/{reference}/{n}"
    v = [s["results"][b][0] / s["results"][a][0] for s in usable[label] if a in s["results"] and b in s["results"]]
    return (statistics.median(v), min(v), max(v)) if v else None

def time_us(label, scenario, design):
    a = f"{scenario}/{pattern}/{design}/{n}"
    v = [s["results"][a][0] / 1e3 for s in usable[label] if a in s["results"]]
    return (statistics.median(v), min(v), max(v)) if v else None

def fmt(x):
    return f"{x:.3f}" if x < 0.1 else f"{x:.2f}" if x < 10 else f"{x:.0f}" if x >= 100 else f"{x:.1f}"

def table(title, scenarios, designs, reference):
    print(f"\n### {title} (reference: {reference} = 1.00; N = {n}, {pattern})\n")
    print("| Scenario | Design | " + " | ".join(labels) + " | across compilers |")
    print("|---|---|" + "---:|" * (len(labels) + 1))
    for sc in scenarios:
        for design in designs:
            cells, meds = [], []
            for label in labels:
                r = rel(label, sc, design, reference)
                if r is None:
                    cells.append("")
                else:
                    cells.append(f"{fmt(r[0])}× ({fmt(r[1])}–{fmt(r[2])})")
                    meds.append(r[0])
            if not meds:
                continue
            across = f"{fmt(min(meds))}–{fmt(max(meds))}×" if fmt(min(meds)) != fmt(max(meds)) else f"{fmt(meds[0])}×"
            print(f"| {sc} | {design} | " + " | ".join(cells) + f" | {across} |")

def times(title, scenarios, designs):
    print(f"\n### {title}: time per pass, µs (median, spread over samples)\n")
    print("| Scenario | Design | " + " | ".join(labels) + " |")
    print("|---|---|" + "---:|" * len(labels))
    for sc in scenarios:
        for design in designs:
            cells = []
            for label in labels:
                t = time_us(label, sc, design)
                cells.append("" if t is None else f"{fmt(t[0])} (±{100 * (t[2] - t[1]) / (2 * t[0]):.0f}%)")
            if any(cells):
                print(f"| {sc} | {design} | " + " | ".join(cells) + " |")

iteration = ["Iter1", "Update2", "Frame3", "RandomGet"]
flexible = ["SparseQuery", "AddRemove", "TagChurn", "DestroyCreate", "Create"]
everything = ["HandWritten", "HandTuned", "QPartHinted", "QPartHinted3Seq", "QPartHinted3Fused", "QPartHinted5Seq", "QPartHinted5Fused", "QueryPart", "Archetype", "SparseSet", "SortedSoA", "StaticBitmask", "OOP", "NaiveObjects"]
table("Iteration and lookup", iteration, [x for x in everything if x != "HandWritten"], "HandWritten")
table("Flexibility", flexible, [x for x in everything if x not in ("HandWritten", "HandTuned", "SparseSet")], "SparseSet")
times("Iteration and lookup", iteration, everything)
times("Flexibility", flexible, everything)

