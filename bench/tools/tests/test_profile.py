"""Failure receiving for the profiler wrapper, without an installed VTune."""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("ecs_profile", TOOLS / "profile.py")
profile = importlib.util.module_from_spec(spec)
spec.loader.exec_module(profile)


class ProfileTests(unittest.TestCase):
    def parse(self, contents):
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "hotspots.csv"
            path.write_text(contents, encoding="utf-8")
            return profile.top_functions(path, 1)

    def test_positive_samples_sorted_and_total_includes_unresolved(self):
        functions, total = self.parse('\ufeffFunction,CPU Time:Self,Module\nslow,2,app\nfast,1,app\n[Unknown],3,app\n')
        self.assertEqual(total, 6)
        self.assertEqual(functions, [("[Unknown]", 3, "app")])

    def test_unusable_exports_are_rejected(self):
        cases = ["", "Function,CPU Time\n", "Function,CPU Time\nf,0\n",
                 "Function,CPU Time\n[Unknown],2\n", "Function,CPU Time\n,2\n",
                 "Function,CPU Time\nf,nan\n", "Function,CPU Time\nf,inf\n",
                 "Function,CPU Time\nf,-1\n", "Function,CPU Time\nf,garbage\n",
                 "Function,CPU Time (%)\nf,100\n", "Module,CPU Time\napp,1\n",
                 "Function,CPU Time,CPU Time:Self\nf,1,2\n"]
        for contents in cases:
            with self.subTest(contents=contents), self.assertRaises(ValueError):
                self.parse(contents)

    def run_cli(self, failure=None, csv_text="Function,CPU Time,Module\nuseful,1,app\n"):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            exe = root / "build" / "bench" / ("sub0ecs_bench" + profile.bench.EXE)
            exe.parent.mkdir(parents=True)
            exe.write_bytes(b"fake binary, never executed")
            commands = []

            def fake_run(command, stdout, stderr):
                commands.append(command)
                kind = "collect" if "-collect" in command else command[1]
                if "-report" in command:
                    kind = command[2]
                if kind == failure:
                    stderr.write("retained failure evidence\n")
                    return subprocess.CompletedProcess(command, 7)
                if kind == "collect":
                    Path(command[command.index("-result-dir") + 1]).mkdir()
                stdout.write(csv_text if kind == "hotspots" else "received fake output\n")
                return subprocess.CompletedProcess(command, 0)

            argv = ["profile.py", "--build-dir", str(root / "build"), "--filter", "^one$", "--out", str(root / "out")]
            with patch.object(sys, "argv", argv), patch.object(profile, "find_vtune", return_value="fake-vtune"), \
                 patch.object(profile.bench, "git_info", return_value={"sha": "abc", "dirty": False}), \
                 patch.object(profile.subprocess, "run", side_effect=fake_run):
                rc = profile.main()
            receipt_path = next((root / "out").rglob("receipt.json"))
            receipt = json.loads(receipt_path.read_text())
            logs = {p.name: p.read_text() for p in receipt_path.parent.glob("*.log")}
            self.assertEqual(len(receipt["executable_sha256"]), 64)
            for entry in receipt["commands"]:
                self.assertTrue((receipt_path.parent / entry["stdout"]).exists())
                self.assertTrue((receipt_path.parent / entry["stderr"]).exists())
            return rc, receipt, logs, commands

    def test_export_is_not_automatic_attribution_acceptance(self):
        rc, receipt, _, commands = self.run_cli()
        self.assertEqual(rc, 0)
        self.assertEqual(receipt["status"], "samples-exported")
        self.assertIn("not reviewed", receipt["attribution"])
        collect = next(c for c in commands if "-collect" in c)
        self.assertIn("sampling-mode=sw", collect)
        self.assertIn("-duration", collect)
        self.assertEqual(commands[0][1], "-version")
        self.assertEqual(commands[1][1], "-help")

    def test_failed_stages_keep_logs_and_stop(self):
        for stage in ("-version", "-help", "collect", "summary", "hotspots"):
            with self.subTest(stage=stage):
                rc, receipt, logs, _ = self.run_cli(failure=stage)
                self.assertEqual(rc, 1)
                self.assertEqual(receipt["status"], "no-profile")
                self.assertEqual(receipt["commands"][-1]["returncode"], 7)
                self.assertTrue(any("retained failure" in value for value in logs.values()))

    def test_zero_exit_with_empty_report_fails(self):
        rc, receipt, _, _ = self.run_cli(csv_text="Function,CPU Time\n")
        self.assertEqual(rc, 1)
        self.assertEqual(receipt["status"], "no-profile")
        self.assertIn("no samples", receipt["error"])

    def test_invalid_duration_rejected_before_tool_discovery(self):
        for value in ("nan", "inf", "0", "-1"):
            with self.subTest(value=value), patch.object(sys, "argv", ["profile.py", "--build-dir", "unused", "--filter", "x", "--seconds", value]), \
                 patch.object(profile, "find_vtune") as discover, self.assertRaises(SystemExit) as raised:
                profile.main()
            self.assertEqual(raised.exception.code, 2)
            discover.assert_not_called()


if __name__ == "__main__":
    unittest.main()
