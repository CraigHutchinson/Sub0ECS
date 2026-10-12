# Correctness receiving

- Mapping: local full Release 50/50 and Debug 50/50.
- Combined: local full Release 50/50.
- Sort-only: 49 tests passed initially; runtime_inspector failed to launch because its executable bit was missing. The wrapper retry also failed; manual execute-bit repair of that owned ELF followed by the narrow retry passed. Original failed logs are retained.
- Collector/profile Python harness: all 20 tests passed.
- Mapping hosted head 7060690de0f9807b0e0c86e9c6b6e56379abba5b and combined head 3754092943e5386fb8774a8f41011c4989de8fcc: all ten checks passed, including three-platform Debug/Release, headless, GPU, race and sanitizer.
- Local sanitizer capability probe failed under the host ptrace/proc restriction; leak checks were not disabled. Hosted sanitizer is the supported receiving gate.

Independent C++ source review covers mapping/oracle and all three sortedness guards. Numeric behavior, arbitrary-order fallback, duplicate rejection and all-or-nothing spatial commit are preserved. Semantic benchmark hashes are drift detectors; the per-sample independent steering oracle is the stronger correctness test.
