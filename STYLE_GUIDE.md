# SubzeroECS Code Style Guide

The conventions the code already follows. When in doubt, match the surrounding code.

## Files: one responsibility each

- **One type, concept or alternative per header**, grouped by concept in a
  directory, so a reader can find what exists from the file tree alone. Examples:
  one executor per file in `fusion/executors/`, one planner per file in
  `fusion/planners/`, one store building block per file in `store/`.
- Each group has an **umbrella header** (`fusion/executors.hpp`, `store.hpp`) whose
  comment maps the files it includes. `sub0ecs.hpp` includes everything.
- Internal helpers go in a `detail/` directory and namespace.
- Every header compiles on its own (includes what it uses).
- Split before landing. Don't wait for a file to grow first.

## Naming

| Kind | Style | Example |
|---|---|---|
| Namespaces | lowercase | `sub0ecs`, `sub0ecs::store`, `sub0ecs::fusion` |
| Types, templates | `PascalCase` | `BasicWorld`, `SidePool`, `ShareColumns` |
| Functions, variables | `camelCase` | `runFused`, `partitionFor`, `liveCount` |
| Private data members | `camelCase_` | `records_`, `byQuery_` |
| Constants | `kPascalCase` | `kMaxTypes`, `kNoColumn`, `kName` |
| Macros (avoid) | `SUB0ECS_` prefix | `SUB0ECS_NATIVE` (CMake options too) |
| Headers | `snake_case.hpp` | `side_pool.hpp`, `share_columns.hpp` |

## Formatting

- 4 spaces, no tabs; braces on their own line (Allman) for namespaces, types and
  functions; short single-statement bodies may stay on one line.
- `#pragma once` in every header.
- Include order: standard headers, a blank line, then project headers. Inside the
  library, include siblings by relative path (`"partition.hpp"`); from tests and
  benchmarks, use `<sub0ecs/...>`.

## Language

- C++20. The library is header-only and free of RTTI and exceptions.
- Decide at compile time what is known at compile time: query sets, planners and
  capabilities are types, dispatched with `if constexpr`.
- No heap allocation in per-row or per-frame hot paths. Reuse scratch storage
  (see `chunks_` and `fchunks_` in `store/world.hpp`); grow geometrically on
  structural change only.
- Components are trivially copyable; the store moves them with `memcpy`.

## Documentation

- `/** ... */` on every public type and non-obvious function. Say *why*, and cite
  the evidence (a FINDINGS section or a measured number) when a choice was
  measured rather than assumed.
- Document edge-case semantics at the API: stale handles, re-adding and removing
  absent components.

## Tests

- doctest, in `tests/`, one `sub0ecs_tests` binary run by ctest.
- New behaviour needs a test. A fixed bug needs a test that fails without the fix.
- Test the store in both storage modes (`TEST_CASE_TEMPLATE`).
- Performance claims go through the benchmark harness
  ([bench/BENCHMARKING.md](bench/BENCHMARKING.md)), never a one-off timing.
