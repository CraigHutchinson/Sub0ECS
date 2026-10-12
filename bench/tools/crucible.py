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
import time
from pathlib import Path

STATE_KEYS = {"completed_tick", "snapshot_samples", "checksum", "closed", "outcome", "reclaimed", "trace",
              "accepted", "last_sequence", "dropped"}


SCALE_STATE_KEYS = {"digest_schema", "state_digest", "initial_digest", "trace_digest",
    "applied_commands", "accepted_commands", "pending_commands", "rejected_commands",
    "discarded_scaled_nanoseconds", "work_actions", "harvested", "row_fallbacks_start",
    "row_fallbacks_end", "tick_observation_count", "dropped_tick_observations"}
STATE_KEYS |= SCALE_STATE_KEYS
SCALE_SETTING_KEYS = set("schema columns rows cell_size field_capacity neighbor_radius separation_strength maximum_acceleration maximum_speed initial_stock_per_cell mass_per_sample initial_reserve actions_per_tick mission structural diagnostics requested_workers requested_partitions resolved_workers resolved_partitions query_scratch_capacity task_capacity queue_capacity warmup_ticks measured_ticks tick_ns route mutation_interval tool_radius tool_magnitude command_capacity trace_capacity observation_capacity profile_stages".split())


def decode(text, scale=None):
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
        elif row["workload"] != "scale_settings" and not STATE_KEYS.intersection(row):
            raise ValueError(f"unrecognized state receipt {key}")
        result[key] = row
    if scale is None:
        required = {(name, n) for name in ("inspector_state", "tick_capture_steady") for n in (64, 2048)}
        required.add(("pub_admission_state", 0))
    else:
        required = {(name, scale) for name in ("scale_settings", "scale_receipt", "scale_state",
                                               "scale_startup", "scale_tick_capture_steady")}
    if not required <= result.keys():
        raise ValueError("incomplete consumer workloads")
    if scale is not None:
        settings, receipt = result["scale_settings", scale], result["scale_receipt", scale]
        if not SCALE_STATE_KEYS <= receipt.keys() or settings.get("schema") != 1 or receipt["digest_schema"] != 1:
            raise ValueError("incomplete scale state or unsupported schema")
        if settings.get("profile_stages") is not False or any(receipt[k] for k in
                ("pending_commands", "rejected_commands", "discarded_scaled_nanoseconds",
                 "row_fallbacks_start", "row_fallbacks_end", "dropped_tick_observations")):
            raise ValueError("scale diagnostic, fallback, dropped work or incomplete command receipt")
        if not SCALE_SETTING_KEYS <= settings.keys():
            raise ValueError("incomplete scale settings")
        geometry = {2048: (64, 32), 100000: (400, 250), 150000: (500, 300)}
        if (settings["columns"], settings["rows"]) != geometry[scale] or settings["cell_size"] != 1:
            raise ValueError("scale geometry differs")
        if any(settings[k] is not False for k in ("mission", "structural", "diagnostics")) or settings["observation_capacity"] != 0 or receipt["tick_observation_count"] != 0:
            raise ValueError("scale contains diagnostics or unexpected scenario features")
        if settings["resolved_workers"] != settings["requested_workers"] or settings["resolved_partitions"] < 1:
            raise ValueError("scale execution settings unresolved")
        state = result["scale_state", scale]
        ticks = settings["measured_ticks"]
        if receipt["completed_tick"] != settings["warmup_ticks"] + ticks or ticks <= 0:
            raise ValueError("scale completed tick count differs")
        if result["scale_tick_capture_steady", scale]["operations"] != ticks:
            raise ValueError("scale measured operation count differs")
        if state.get("completed_tick") != receipt["completed_tick"] or state.get("snapshot_samples") != scale:
            raise ValueError("scale snapshot does not match completion")
        mutations = (ticks + 5) // 6
        if len(state.get("trace", [])) != mutations:
            raise ValueError("scale command trace incomplete")
        if receipt["applied_commands"] != mutations or receipt["accepted_commands"] != mutations:
            raise ValueError("scale command accounting differs")
        for key in ("state_digest", "initial_digest", "trace_digest"):
            digest = receipt[key]
            if not isinstance(digest, str) or not 1 <= len(digest) <= 16 or any(c not in "0123456789abcdef" for c in digest):
                raise ValueError("invalid state digest")
    return result


def equivalent(reference, candidate):
    if reference.keys() != candidate.keys():
        raise ValueError("workload set differs")
    for key, left in reference.items():
        right = candidate[key]
        if key[0] == "scale_settings" and {k: v for k, v in left.items() if k != "arm"} != {k: v for k, v in right.items() if k != "arm"}:
            raise ValueError("scale settings differ")
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
    parser.add_argument("--scale", type=int, choices=(2048, 100000, 150000))
    parser.add_argument("--workers", type=int, default=1)
    parser.add_argument("--warmup", type=int, default=120)
    parser.add_argument("--ticks", type=int, default=300)
    parser.add_argument("--field-route", choices=("flow", "gather"), default="gather")
    parser.add_argument("--arms", nargs="+", choices=("direct", "integrated"), default=["direct", "integrated"])
    parser.add_argument("--cooldown", type=float, default=2, help="seconds between independent processes")
    args = parser.parse_args()
    if not math.isfinite(args.cooldown) or not 0 <= args.cooldown <= 60:
        parser.error("cooldown must be finite and between zero and 60 seconds")
    if args.scale and (args.skip_missions or not 1 <= args.workers <= 32 or not 0 <= args.warmup <= 10000 or not 1 <= args.ticks <= 10000):
        parser.error("invalid scale settings")
    if len(set(args.arms)) != len(args.arms):
        parser.error("duplicate execution arms")
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
            "rounds": args.rounds, "skip_missions": args.skip_missions, "builds": {},
            "scale": args.scale, "workers": args.workers, "warmup": args.warmup, "ticks": args.ticks,
            "field_route": args.field_route, "cooldown_seconds": args.cooldown,
            "host_limits": "External load, power and thermal state uncontrolled; headless only"}
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
        if home is None:
            raise ValueError("build does not name its consumer source")
        source = Path(home).resolve()
        if git(source, "status", "--porcelain", "--untracked-files=no"):
            raise ValueError("build source must be committed")
        meta["builds"][label]["source"] = {"path": str(source), "sha": git(source, "rev-parse", "HEAD"),
            "tree": git(source, "rev-parse", "HEAD^{tree}"),
            "pins": (source / "cmake/DependencyPins.cmake").read_text(),
            "benchmark_sha256": hashlib.sha256((source / "benchmarks/runtime_backbone.cpp").read_bytes()).hexdigest()}
        if "CMAKE_BUILD_TYPE:STRING=Release" not in cache:
            raise ValueError("capture requires a Release build")
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
        for route in args.arms:
            arms.append((label, route, binary))
    sources = [build["source"] for build in meta["builds"].values()]
    if any(s["benchmark_sha256"] != sources[0]["benchmark_sha256"] for s in sources[1:]):
        raise ValueError("comparison needs identical benchmark source")
    if args.scale is not None and any(s["pins"] != sources[0]["pins"] for s in sources[1:]):
        raise ValueError("production scale comparison needs identical dependency pins")
    (args.out / "meta.json").write_text(json.dumps(meta, indent=2))
    samples = {}
    reference = None
    for round_index in range(args.rounds):
        ordered = arms if round_index % 2 == 0 else list(reversed(arms))
        for label, route, binary in ordered:
            prefix = args.out / f"r{round_index + 1}-{label}-{route}"
            command = [str(binary), "--arm", route] + (["--skip-missions"] if args.skip_missions else [])
            if args.scale is not None:
                command += ["--scale", str(args.scale), "--workers", str(args.workers),
                            "--warmup", str(args.warmup), "--ticks", str(args.ticks), "--route", args.field_route]
            (prefix.with_suffix(".command.json")).write_text(json.dumps(command))
            time.sleep(args.cooldown)
            invocation = {"command": command, "load_before": os.getloadavg() if hasattr(os, "getloadavg") else None}
            started = time.monotonic()
            try:
                with prefix.with_suffix(".jsonl").open("w") as stdout, prefix.with_suffix(".stderr").open("w") as stderr:
                    run = subprocess.run(command, stdout=stdout, stderr=stderr, timeout=args.timeout, check=False)
                invocation["returncode"] = run.returncode
            except subprocess.TimeoutExpired:
                invocation["timeout"] = True
                raise
            finally:
                invocation["wall_seconds"] = time.monotonic() - started
                invocation["load_after"] = os.getloadavg() if hasattr(os, "getloadavg") else None
                prefix.with_suffix(".invocation.json").write_text(json.dumps(invocation, indent=2))
            if run.returncode:
                raise RuntimeError(f"consumer failed ({run.returncode}): {prefix}")
            rows = decode(prefix.with_suffix(".jsonl").read_text(), args.scale)
            if args.scale is not None:
                settings = rows["scale_settings", args.scale]
                expected = {"requested_workers": args.workers, "warmup_ticks": args.warmup,
                            "measured_ticks": args.ticks, "route": args.field_route}
                if any(settings.get(k) != v for k, v in expected.items()):
                    raise ValueError("scale receipt does not match requested settings")
            if args.scale is None and not args.skip_missions and not {("reference", 2048), ("structural", 2048)} <= rows.keys():
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
                "new_calls": [run[key]["new_calls"] for run in runs],
                "process_p95_us": [run[key]["p95_us"] for run in runs],
                "process_p99_us": [run[key]["p99_us"] for run in runs]}
    (args.out / "summary.json").write_text(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
