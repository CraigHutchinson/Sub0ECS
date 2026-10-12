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


def scale_fixture():
    return [
        {"workload": "scale_settings", "entities": 100000,
         **{key: 1 for key in consumer.SCALE_SETTING_KEYS}, "schema": 1,
         "columns": 400, "rows": 250, "cell_size": 1,
         "mission": False, "structural": False, "diagnostics": False,
         "observation_capacity": 0, "profile_stages": False, "warmup_ticks": 120, "measured_ticks": 300},
        {"workload": "scale_receipt", "entities": 100000,
         **{key: 0 for key in consumer.SCALE_STATE_KEYS}, "digest_schema": 1,
         "state_digest": "abcd", "initial_digest": "1234", "trace_digest": "ef56",
         "completed_tick": 420, "applied_commands": 50, "accepted_commands": 50},
        {"workload": "scale_state", "entities": 100000, "completed_tick": 420, "snapshot_samples": 100000, "trace": [{}] * 50},
        *[{"workload": name, "entities": 100000, "operations": operations,
           **{key: 1.0 for key in ("median_us", "min_us", "max_us", "p95_us", "p99_us", "total_us")}}
          for name, operations in (("scale_startup", 1), ("scale_tick_capture_steady", 300))]]


class ScaleReceipt(unittest.TestCase):
    def decode(self, rows):
        return consumer.decode("\n".join(map(json.dumps, rows)), 100000)

    def test_complete_receipt_passes(self):
        consumer.equivalent(self.decode(scale_fixture()), self.decode(scale_fixture()))

    def test_missing_digest_is_rejected(self):
        rows = scale_fixture()
        del rows[1]["state_digest"]
        with self.assertRaises(ValueError):
            self.decode(rows)

    def test_changed_bit_digest_is_rejected(self):
        rows = scale_fixture()
        rows[1]["state_digest"] = "abce"
        with self.assertRaises(ValueError):
            consumer.equivalent(self.decode(scale_fixture()), self.decode(rows))

    def test_fallback_dropped_time_and_observations_are_rejected(self):
        for key in ("row_fallbacks_start", "row_fallbacks_end", "discarded_scaled_nanoseconds",
                    "dropped_tick_observations", "pending_commands", "rejected_commands"):
            with self.subTest(key=key):
                rows = scale_fixture()
                rows[1][key] = 1
                with self.assertRaises(ValueError):
                    self.decode(rows)

    def test_diagnostic_timing_cannot_qualify(self):
        rows = scale_fixture()
        rows[0]["profile_stages"] = True
        with self.assertRaises(ValueError):
            self.decode(rows)

    def test_work_counts_must_match(self):
        for key in ("completed_tick", "applied_commands", "accepted_commands"):
            with self.subTest(key=key):
                rows = scale_fixture()
                rows[1][key] -= 1
                with self.assertRaises(ValueError):
                    self.decode(rows)

    def test_settings_must_match(self):
        rows = scale_fixture()
        rows[0]["workers"] = 2
        with self.assertRaises(ValueError):
            consumer.equivalent(self.decode(scale_fixture()), self.decode(rows))

    def test_missing_or_observed_settings_are_rejected(self):
        rows = scale_fixture()
        del rows[0]["resolved_workers"]
        with self.assertRaises(ValueError):
            self.decode(rows)
        for key in ("diagnostics", "observation_capacity"):
            rows = scale_fixture()
            rows[0][key] = 1
            with self.assertRaises(ValueError):
                self.decode(rows)
        rows = scale_fixture()
        rows[1]["tick_observation_count"] = 1
        with self.assertRaises(ValueError):
            self.decode(rows)

    def test_snapshot_and_geometry_are_checked(self):
        for index, key, value in ((0, "columns", 64), (2, "snapshot_samples", 99999),
                                  (2, "completed_tick", 419), (2, "trace", [])):
            rows = scale_fixture()
            rows[index][key] = value
            with self.assertRaises(ValueError):
                self.decode(rows)
