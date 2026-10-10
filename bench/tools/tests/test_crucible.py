import importlib.util
import json
import unittest
from pathlib import Path

spec = importlib.util.spec_from_file_location("consumer", Path(__file__).parents[1] / "crucible.py")
consumer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(consumer)


def fixture():
    rows = [{"workload": "pub_admission_state", "entities": 0, "accepted": 10001, "last_sequence": 10001}]
    for n in (64, 2048):
        rows += [{"workload": "inspector_state", "entities": n, "completed_tick": 136, "trace": [{"sequence": 1}]},
                 {"workload": "tick_capture_steady", "entities": n, "operations": 120,
                  **{key: 1.0 for key in ("median_us", "min_us", "max_us", "p95_us", "p99_us", "total_us")}}]
    return rows


class ConsumerReceipt(unittest.TestCase):
    def decode(self, rows):
        return consumer.decode("\n".join(map(json.dumps, rows)))

    def test_changed_trace_is_rejected(self):
        left, right = fixture(), fixture()
        right[1]["trace"][0]["sequence"] = 2
        with self.assertRaises(ValueError):
            consumer.equivalent(self.decode(left), self.decode(right))

    def test_missing_workload_is_rejected(self):
        with self.assertRaises(ValueError):
            self.decode(fixture()[:-1])

    def test_nonfinite_timing_is_rejected(self):
        rows = fixture()
        rows[2]["median_us"] = float("nan")
        with self.assertRaises(ValueError):
            self.decode(rows)

    def test_duplicate_is_rejected(self):
        rows = fixture()
        with self.assertRaises(ValueError):
            self.decode(rows + rows[:1])

    def test_timing_variation_is_allowed(self):
        left, right = fixture(), fixture()
        right[2]["median_us"] = 2.0
        consumer.equivalent(self.decode(left), self.decode(right))
