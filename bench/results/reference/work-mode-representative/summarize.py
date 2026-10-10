"""Summarize all successful raw captures; no busy/outlier samples are discarded."""
from collections import defaultdict
import json
from pathlib import Path
import statistics
import sys

root = Path(sys.argv[1])
values = defaultdict(list)
for path in sorted(root.glob('pair*.json')):
    for row in json.loads(path.read_text())['results']:
        values[row['name']].append(row['median_ns'])
summary = {}
for name, samples in sorted(values.items()):
    summary[name] = dict(count=len(samples), median_ns=statistics.median(samples),
                         min_ns=min(samples), max_ns=max(samples), samples_ns=samples)
(root / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
lines = ['# All process medians', '', 'Times are microseconds per complete tick; ranges are full ranges, not confidence intervals.', '',
         '| Case | Count | Median [min–max], µs |', '|---|---:|---:|']
for name, s in summary.items():
    lines.append(f"| {name} | {s['count']} | {s['median_ns']/1000:.3f} [{s['min_ns']/1000:.3f}–{s['max_ns']/1000:.3f}] |")
(root / 'summary.md').write_text('\n'.join(lines) + '\n')
