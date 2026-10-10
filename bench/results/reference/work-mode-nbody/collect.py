"""Reproduce diagnostic native A/B receipts; run only on an otherwise idle host."""
import hashlib
import json
import os
from pathlib import Path
import platform
import subprocess
import sys
import time

binary = Path(sys.argv[1]).resolve()
out = Path(sys.argv[2]).resolve()
out.mkdir(parents=True, exist_ok=True)
env = dict(os.environ, NBODY_SIZES="64,1024,4096", NBODY_THREADS="2")
receipt = {"binary_sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
           "platform": platform.platform(), "machine": platform.machine(),
           "cpu": Path("/proc/cpuinfo").read_text(),
           "cpu_quota": Path("/sys/fs/cgroup/cpu.max").read_text().strip(),
           "workload": {k: env[k] for k in ("NBODY_SIZES", "NBODY_THREADS")},
           "thermal_power": "not controlled; shared virtual host, diagnostic only", "runs": []}
for pair in range(5):
    for arm in (["default", "grain64"] if pair % 2 == 0 else ["grain64", "default"]):
        stem = f"pair{pair + 1}-{arm}"
        designs = "HandWritten|ECS|Native2|Pipeline2" if arm == "default" else "HandWritten|ECS|NativeG64x2|PipelineG64x2"
        command = [str(binary), f"--filter=({designs})/", "--epochs=11", "--quiet", f"--out={out / (stem + '.json')}"]
        run = {"pair": pair + 1, "arm": arm, "command": command, "load_before": os.getloadavg(), "start": time.time()}
        with (out / (stem + ".log")).open("w") as log:
            result = subprocess.run(command, env=env, stdout=log, stderr=subprocess.STDOUT, check=False)
        run.update(returncode=result.returncode, end=time.time(), load_after=os.getloadavg())
        receipt["runs"].append(run)
        (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
        if result.returncode:
            raise SystemExit(result.returncode)
