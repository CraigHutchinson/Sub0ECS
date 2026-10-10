"""Five alternating matched-build pairs, using the common whole-tick harness.

Arguments: benchmark directory, receipt directory.
Both kernels use the merged PR19 library in the same binary. Control and candidate
run in separate alternating processes with identical resource counts and work.
Run after all builds/tests stop. Instrumented timings are never included.
"""
import hashlib
import json
import os
from pathlib import Path
import platform
import subprocess
import sys
import time

build = Path(sys.argv[1]).resolve()
out = Path(sys.argv[2]).resolve()
out.mkdir(parents=True, exist_ok=True)
receipt = {"platform": platform.platform(), "cpu": Path('/proc/cpuinfo').read_text(),
           "quota": Path('/sys/fs/cgroup/cpu.max').read_text().strip(),
           "thermal_power": "not controlled; shared virtual host; diagnostic only",
           "binary_sha256": {}, "runs": []}
for exe in ['sub0ecs_nbody_bench', 'sub0ecs_row_workloads_bench']:
    receipt['binary_sha256'][exe] = hashlib.sha256((build / exe).read_bytes()).hexdigest()

def capture(stem, exe, filter_, settings, pair, label):
    path = out / (stem + '.json')
    command = [str(build / exe), '--filter=' + filter_, '--epochs=11', '--quiet', '--out=' + str(path)]
    run = dict(pair=pair, label=label, command=command, env=settings,
               start=time.time(), load_before=os.getloadavg())
    with (out / (stem + '.log')).open('w') as log:
        result = subprocess.run(command, env=dict(os.environ, **settings), stdout=log, stderr=subprocess.STDOUT)
    run.update(returncode=result.returncode, end=time.time(), load_after=os.getloadavg())
    receipt['runs'].append(run)
    (out / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    if result.returncode:
        raise SystemExit(result.returncode)
    groups = {}
    for record in json.loads(path.read_text())['results']:
        counters = record['counters']
        groups.setdefault(record['group'], set()).add((counters['completed_ticks'], counters['state_checksum']))
    if not groups or any(len(values) != 1 for values in groups.values()):
        raise RuntimeError(f'{path}: unequal evolving work or checksum across compared designs')
    return groups

for pair in range(1, 6):
    paired = {}
    for label in (['control', 'candidate'] if pair % 2 else ['candidate', 'control']):
        prefix = '' if label == 'control' else 'Compact'
        paired[label] = capture(f'pair{pair}-{label}-nbody', 'sub0ecs_nbody_bench',
            '^NBody/Compact/' + prefix + '(HandWritten|ECS|NativeG64x2|PipelineG64x2)/',
            dict(NBODY_SIZES='64,1024,4096', NBODY_THREADS='2'), pair, label)
    if paired['control'] != paired['candidate']:
        raise RuntimeError(f'pair {pair}: unequal evolving work across processes')
    capture(f'pair{pair}-matched', 'sub0ecs_nbody_bench', '^NBody/(Compact)?Matched',
            dict(NBODY_SIZES='64,1024,4096', NBODY_THREADS='2'), pair, 'matched')
    capture(f'pair{pair}-rows', 'sub0ecs_row_workloads_bench', '^RowWork/',
            dict(ROW_SIZES='1024,16384,65536', ROW_THREADS='1,2'), pair, 'rows')
