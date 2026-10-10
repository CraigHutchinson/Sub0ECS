#!/usr/bin/env python3
"""Capture the existing Crucible headless backbone without copying its game policy.

Runs alternate arms in alternating process order and fails closed on process,
JSON, state/trace, or workload mismatch. This is not a native-frame qualification.
"""
import argparse
import hashlib
import json
import math
import os
import platform
import statistics
import subprocess
from pathlib import Path

STATE_KEYS = {"completed_tick", "snapshot_samples", "checksum", "closed", "outcome", "reclaimed", "trace",
              "accepted", "last_sequence", "dropped"}


def decode(text):
    rows = [json.loads(line) for line in text.splitlines() if line.strip()]
    if not rows:
        raise ValueError("empty consumer receipt")
    result = {}
    for row in rows:
        key = (row["workload"], row["entities"])
        if key in result:
            raise ValueError(f"duplicate workload {key}")
        if "operations" in row:
            if row["operations"] <= 0:
                raise ValueError("empty timing sample")
            for field in ("median_us", "p95_us", "p99_us", "min_us", "max_us", "total_us"):
                if not math.isfinite(row[field]) or row[field] < 0:
                    raise ValueError(f"invalid timing {field}")
        elif not STATE_KEYS.intersection(row):
            raise ValueError(f"unrecognized state receipt {key}")
        result[key] = row
    required = {(name, n) for name in ("inspector_state", "tick_capture_steady") for n in (64, 2048)}
    required.add(("pub_admission_state", 0))
    if not required <= result.keys():
        raise ValueError("incomplete consumer workloads")
    return result


def equivalent(reference, candidate):
    if reference.keys() != candidate.keys():
        raise ValueError("workload set differs")
    for key, left in reference.items():
        right = candidate[key]
        fields = STATE_KEYS.intersection(left.keys() | right.keys())
        if any(left.get(k) != right.get(k) for k in fields):
            raise ValueError(f"consumer state/trace differs: {key}")
        if left.get("operations") != right.get("operations"):
            raise ValueError(f"operation count differs: {key}")


def git(source, *args):
    return subprocess.check_output(["git", "-C", str(source), *args], text=True).strip()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--build", action="append", required=True, help="label=Crucible-build-directory")
    parser.add_argument("--rounds", type=int, default=5)
    parser.add_argument("--timeout", type=int, default=600)
    parser.add_argument("--skip-missions", action="store_true")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if args.rounds < 1 or args.timeout < 1:
        parser.error("rounds and timeout must be positive")
    if git(args.source, "status", "--porcelain", "--untracked-files=no"):
        parser.error("consumer source must be committed")
    builds = dict(item.split("=", 1) for item in args.build)
    if len(builds) != len(args.build) or not builds:
        parser.error("build labels must be unique")
    args.out.mkdir(parents=True, exist_ok=False)
    meta = {"source": git(args.source, "rev-parse", "HEAD"), "tree": git(args.source, "rev-parse", "HEAD^{tree}"),
            "platform": platform.platform(), "pins": (args.source / "cmake/DependencyPins.cmake").read_text(),
            "cpu_affinity": sorted(os.sched_getaffinity(0)) if hasattr(os, "sched_getaffinity") else None,
            "rounds": args.rounds, "skip_missions": args.skip_missions, "builds": {}}
    arms = []
    for label, directory in builds.items():
        if not label.replace("-", "").replace("_", "").isalnum():
            parser.error("labels must contain only letters, digits, hyphens and underscores")
        build = Path(directory).resolve()
        binary = build / "benchmarks/crucible_runtime_backbone_bench"
        if not binary.exists():
            binary = binary.with_suffix(".exe")
        meta["builds"][label] = {"path": str(build), "binary_sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
                                 "cache": (build / "CMakeCache.txt").read_text()}
        cache = meta["builds"][label]["cache"]
        home = next((line.split("=", 1)[1] for line in cache.splitlines()
                     if line.startswith("CMAKE_HOME_DIRECTORY:")), None)
        if home is None or Path(home).resolve() != args.source.resolve():
            raise ValueError("build does not name the supplied consumer source")
        dependencies = {}
        for line in cache.splitlines():
            if "=" not in line or ":" not in line:
                continue
            key, value = line.split("=", 1)
            if key.split(":", 1)[0].endswith("_SOURCE_DIR") and value and Path(value).is_dir():
                try:
                    dependencies[key] = {"path": value, "sha": git(value, "rev-parse", "HEAD"),
                                         "dirty": git(value, "status", "--porcelain", "--untracked-files=no")}
                except subprocess.CalledProcessError:
                    pass
        meta["builds"][label]["resolved_sources"] = dependencies
        meta["builds"][label]["compile_and_link"] = {
            str(p.relative_to(build)): p.read_text() for name in ("flags.make", "link.txt")
            for p in build.rglob(name)}
        for route in ("direct", "integrated"):
            arms.append((label, route, binary))
    (args.out / "meta.json").write_text(json.dumps(meta, indent=2))
    samples = {}
    reference = None
    for round_index in range(args.rounds):
        ordered = arms if round_index % 2 == 0 else list(reversed(arms))
        for label, route, binary in ordered:
            prefix = args.out / f"r{round_index + 1}-{label}-{route}"
            command = [str(binary), "--arm", route] + (["--skip-missions"] if args.skip_missions else [])
            (prefix.with_suffix(".command.json")).write_text(json.dumps(command))
            with prefix.with_suffix(".jsonl").open("w") as stdout, prefix.with_suffix(".stderr").open("w") as stderr:
                run = subprocess.run(command, stdout=stdout, stderr=stderr, timeout=args.timeout, check=False)
            if run.returncode:
                raise RuntimeError(f"consumer failed ({run.returncode}): {prefix}")
            rows = decode(prefix.with_suffix(".jsonl").read_text())
            if not args.skip_missions and not {("reference", 2048), ("structural", 2048)} <= rows.keys():
                raise ValueError("missing complete mission receipts")
            if reference is None:
                reference = rows
            equivalent(reference, rows)
            samples.setdefault(f"{label}/{route}", []).append(rows)
            print(prefix.name, "passed", flush=True)
    summary = {}
    for arm, runs in samples.items():
        summary[arm] = {}
        for key, row in runs[0].items():
            if "median_us" not in row:
                continue
            values = [run[key]["median_us"] for run in runs]
            summary[arm][f"{key[0]}/{key[1]}"] = {"median_us": statistics.median(values),
                "min_us": min(values), "max_us": max(values), "process_medians_us": values,
                "new_calls": [run[key]["new_calls"] for run in runs]}
    (args.out / "summary.json").write_text(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
