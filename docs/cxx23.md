# Common C++23 baseline

The active Sub0ECS, Sub0Pub, Sub0Pipeline and Crucible targets all require C++23.
The default-branch CMake audit on 2026-10-10 found Pub, Pipeline and Crucible
already explicit; this change aligns ECS rather than relying on an optional
integration to raise its language mode.

| Project | Target declaring the requirement | Visibility |
|---|---|---|
| Sub0ECS | `Sub0ECS` / `Sub0ECS::Sub0ECS` | `INTERFACE cxx_std_23` |
| Sub0Pub | `Sub0Pub` / `Sub0Pub::Sub0Pub` | `INTERFACE cxx_std_23` |
| Sub0Pipeline | `Sub0Pipeline` / `Sub0Pipeline::Sub0Pipeline` | `PUBLIC cxx_std_23` |
| Crucible | `crucible_options`, linked by its project targets | `INTERFACE cxx_std_23` |

Source audit: the default-branch CMake file blob hashes were Pub
`16602500800932c64a24775bddd5ae594cb9dbfb`, Pipeline
`425185be86d95caf12b73ab48d59b28b7280ce46`, and Crucible
`308093eb7393ced0abc0f1b34e639ab0d9f72f63`. No change to those three projects is
necessary to declare the requested language level.

This is a **minimum-language compatibility change for standalone ECS consumers**:
C++20 is no longer its supported baseline. Link `Sub0ECS::Sub0ECS` to inherit the
requirement. Direct header consumers must select their compiler's C++23 mode.
CMake selects the corresponding compiler switch (some MSVC versions use
`/std:c++latest`); a language-mode flag is not a claim of complete standard-library
feature support on every compiler. The project's GCC, Clang, MSVC, macOS and
sanitizer CI validates the features actually used.

The change does not introduce Pub/Pipeline dependencies to the core, force global
compiler flags on parent projects, or change ECS type signatures. New C++23
facilities should be adopted where they simplify a consumed contract, with normal
correctness and measured performance gates; merely enabling the language mode is
not an optimization claim. Existing integrated benchmarks already compiled as
C++23. Historical C++20 receipts retain their original labels and are not relabeled
as new measurements.

The standalone CI leg compiles without optional integrations, so it checks the
core's own requirement. The integration legs keep Pub/Pipeline composition and
sanitizer coverage. A downstream CMake consumer can request C++20 locally and link
the ECS target; CMake must promote that consumer to at least C++23 through usage
requirements. This was also checked locally with optional integrations disabled.

Local GCC 13.3 validation configured an independent `add_subdirectory` consumer
with `CXX_STANDARD 20`, `CXX_STANDARD_REQUIRED YES`, and `CXX_EXTENSIONS NO`.
Linking only `Sub0ECS::Sub0ECS` produced `-std=c++23`; an executable using both
`std::expected` and ECS create/iterate returned success. Configuration also checked
that neither optional library target existed. No core algorithm changed.
