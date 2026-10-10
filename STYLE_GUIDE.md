# SubzeroECS Code Style Guide

style-profile: sub0

SubzeroECS follows the Sub0 family style profile. This guide summarises how
the rules apply here; where it and the profile disagree, the profile wins. The
profile lives with the family tooling (`sub0-profile.md`, selected by the
`style-profile:` line above) and numbers each rule (S01 and so on).

---

## Files: one responsibility each

- **One primary type per header**, named after it in snake_case
  (`class SidePool` lives in `side_pool.hpp`), grouped by concept in a directory,
  so a reader finds what exists from the file tree alone: one executor per file in
  `fusion/executors/`, one planner per file in `fusion/planners/`, one store
  building block per file in `store/`.
- Each group has an **umbrella header** (`fusion/executors.hpp`, `store.hpp`) whose
  comment maps the files it includes. `sub0ecs.hpp` includes everything.
- Internal helpers go in a `detail/` directory and namespace.
- Every header compiles on its own (includes what it uses).
- Split before landing. Don't wait for a file to grow first.

## Naming

| Element | Convention | Example |
|---------|-----------|---------|
| Namespaces | lowercase | `sub0ecs`, `sub0ecs::store`, `sub0ecs::fusion`, `detail` |
| Classes/Structs/Enums/Aliases | PascalCase | `BasicWorld`, `SidePool`, `ShareColumns` |
| Interfaces (pure virtual) | `I` prefix | `ISidePool` |
| Functions and methods | camelCase | `runFused()`, `eachParallel()`, `liveCount()` |
| Member variables | camelCase with `_` suffix | `records_`, `byQuery_` |
| Local variables | camelCase | `newCols`, `dstRow` |
| Constants and enumerators | prefix + PascalCase (see Open) | `kMaxTypes`, `kNoColumn` |
| Macros | UPPER_SNAKE_CASE with `SUB0ECS_` prefix | `SUB0ECS_FLATTEN_CALLS` |
| CMake options and targets | `SUB0ECS_` prefix; alias `Sub0ECS::Sub0ECS` | `SUB0ECS_NATIVE` |
| Files | snake_case `.hpp` / `.cpp` | `side_pool.hpp`, `test_store.cpp` |

Names that mirror the standard library's protocols keep the standard's
spelling so generic code keeps working. Calls into the standard library are
unaffected.

## Formatting

- 4 spaces, no tabs.
- Allman braces: the opening brace is on its own line for namespaces, types,
  functions and control flow (`if`, `else`, `for`, `while`, `switch`). `else`
  starts its own line after the closing brace. Single-line bodies
  (`if (x) return;`, `{ return x_; }`) and lambda bodies are not covered.
- Line width: about 120 characters, soft limit.
- Pointer and reference: `Type* name`, `const Type& name`.

```cpp
template <typename C>
void remove(Entity e)
{
    if (!entities_.alive(e)) return;
    const Mask newCols = columnsFor(newHas);
    if (newCols == part(r).columnsMask)
    {
        poolOf<C>(t).remove(e);
    }
    else
    {
        moveTo(e, r, newCols, newHas, part(r).removeEdge[t]);
    }
}
```

## Includes

- The library's own headers use quotes and a library-rooted path:
  `#include "sub0ecs/store/partition.hpp"`. Never bare relative
  (`"partition.hpp"`, `"../entity.hpp"`) and never angle brackets. This holds
  inside the library and in tests, examples and benchmarks alike.
- System and third-party headers use angle brackets.
- Headers local to the tests or benchmarks, which have no library root
  (`"common/scenarios.hpp"`, `"harness/harness.hpp"`), are quoted and relative.
- Header guards are `#pragma once`.

## Documentation

- Every public declaration has a Doxygen `/** ... */` comment. There is no
  `@brief`: the first sentence is the brief and ends in a full stop.
- `@param` for every parameter, `@return` for every non-void result,
  `@tparam` for caller-supplied template parameters. Add `@note` for thread
  safety, ownership and lifetime where they matter. Directional tags such as
  `@param[in]` are allowed but not required.
- Exempt from the tags: `override` declarations (they inherit the base
  documentation), defaulted or deleted special members, and operators with
  their conventional meaning.
- Plain `//` comments are for private members and implementation rationale.
  Say *why*; cite a FINDINGS section when a choice was measured.
- Document edge-case semantics at the API: stale handles, re-adding and removing
  absent components, limits.
- No stale numbers: a figure stays in a comment or document only while the
  current code produces it.
- Use `[[nodiscard]]` where discarding the result is a wasted computation or
  a likely usage error (queries, factories, status returns). Not on functions
  called for their effect.

## Language

- C++20. The library is header-only and free of RTTI and exceptions; it
  terminates (`std::abort`) on unrecoverable misuse and uses `assert()` for
  internal invariants.
- The opt-in `Sub0ECS::Pipeline` adapter follows Pipeline's C++23 and exception
  contract: it joins accepted work before propagating failures. The standalone
  ECS target and umbrella remain C++20 without this dependency.
- Decide at compile time what is known at compile time: query sets, planners and
  capabilities are types, dispatched with `if constexpr`.
- Prefer concept constraints over SFINAE; `using` aliases over `typedef`.
- No heap allocation in per-row or per-frame hot paths. Reuse scratch storage
  (see `chunks_` and `fchunks_` in `store/world.hpp`); grow geometrically on
  structural change only.
- Components are trivially copyable; the store moves them with `memcpy`.
- Fixed-width integer types from `<cstdint>`; cast explicitly when narrowing.

## Tests

- doctest, in `tests/`, one file per topic named `test_<topic>.cpp`, one
  `sub0ecs_tests` binary run by ctest.
- New behaviour needs a test. A fixed bug needs a test that fails without the fix.
- Test the store in both storage modes (`TEST_CASE_TEMPLATE`).
- Performance claims go through the benchmark harness
  ([bench/BENCHMARKING.md](bench/BENCHMARKING.md)), never a one-off timing.

## Open in the family profile

These are undecided there, so this guide does not fix them: the prefix on
constants and enumerators (`k` here), the namespace scheme, the spacing inside
`template <...>` (the code uses `template <typename T>`), the error handling
style, include order (standard headers first here), how test dependencies are
supplied (fetched with CPM here), and whether to add a formatter configuration.
