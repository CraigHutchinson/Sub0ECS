# SubzeroECS

A header-only C++20 Entity Component System whose storage is laid out by the
**systems you declare**, not only by the components entities happen to have.

You get the iteration speed of a hand-written loop and keep the flexibility that a
hand-written loop gives up: any entity can gain or lose any component at any time,
systems can be added while the program runs, and the same systems run inline, on a
thread pool or on a device without changing their results.

## What it gives you

- **Storage shaped by your queries.** Each declared query iterates whole
  partitions of dense, typed columns, so a system is a plain loop over arrays.
- **Cheap change where it matters.** A component no query filters on never moves
  an entity between partitions: adding and removing it is a sparse-set operation.
  Declare churn-heavy components `Volatile` and they stay out of the layout
  altogether.
- **Many small systems at the cost of one pass.** Systems that share columns can be
  fused into one loop. *Planners* decide what fuses; *executors* decide where it
  runs (inline, tiled, thread pool, offload).
- **The same answer however it runs.** Results are bit-identical whichever plan or
  executor is chosen, so lockstep simulation and replay survive a change of
  hardware.
- **Systems added at runtime.** A query added while running works immediately,
  with a bounded per-frame migration budget instead of a level-load stall.
- **Few limits to design around.** Any number of component types; up to 16.7M
  live entities; generational handles, so a stale handle is rejected rather than
  reaching the entity that reused its slot (for the first 255 reuses of that slot).

## Quick start

```cpp
#include <tuple>
#include "sub0ecs/sub0ecs.hpp"

struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Selected { int group; };

// The systems' queries, declared up front: they decide the storage layout.
using Queries = std::tuple<sub0ecs::Query<Position, Velocity>>;
using World = sub0ecs::store::World<Queries, sub0ecs::store::Volatile<Selected>>;

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position& p, Velocity& v) const { p.x += v.dx; p.y += v.dy; }
};

int main()
{
    World world;
    const sub0ecs::Entity e = world.create(Position{ 0, 0 }, Velocity{ 1, 2 });
    world.add(e, Selected{ 1 });                 // Volatile: no data moves

    world.each<Position, Velocity>([](Position& p, Velocity& v) { p.x += v.dx; });
    world.runFused(Integrate{});                 // fused pass over matching partitions

    const Position* p = world.find<Position>(e); // nullptr once e is destroyed
    world.destroy(e);
    return p && !world.alive(e) ? 0 : 1;
}
```

Twelve runnable examples, one per feature, are in [examples/](examples/) and
described in [docs/EXAMPLES.md](docs/EXAMPLES.md).

## Library layout

| Header | Contents |
|---|---|
| [`sub0ecs/sub0ecs.hpp`](include/sub0ecs/sub0ecs.hpp) | Everything |
| [`sub0ecs/store/world.hpp`](include/sub0ecs/store/world.hpp) | `World` / `BasicWorld`: entities, components, queries, fused and parallel iteration, runtime queries |
| [`sub0ecs/store/`](include/sub0ecs/store/) | Its building blocks: partitions, columns, side pools, type indices, the `Volatile` hint |
| [`sub0ecs/entity.hpp`](include/sub0ecs/entity.hpp) | Generational 32-bit `Entity` handle and allocator |
| [`sub0ecs/query.hpp`](include/sub0ecs/query.hpp) | `Query<Cs...>` |
| [`sub0ecs/fusion/`](include/sub0ecs/fusion/) | `Access` declarations, [planners](include/sub0ecs/fusion/planners/) (what fuses), [executors](include/sub0ecs/fusion/executors/) (where it runs) |

Handle rules: operations on a destroyed entity's handle are no-ops and `find`
returns `nullptr`; `add` of a component the entity already has overwrites it;
`remove` of one it lacks does nothing. Components must be trivially copyable and at most 64 bytes.

There is no fixed limit on the number of component types. Up to 64 of a World
type's components can be dense columns: the ones its queries and `Volatile` list
name (checked at compile time), then others in the order they are first used. Any
further type is kept in side storage, where everything still works and only
`find` locality differs.

## Performance

The bar is **code written by hand for exactly this workload**, with no ECS: a
plain loop over arrays, and a hand-tuned version of it with one array per field
and explicit SIMD. Those have none of the library's flexibility, which is the
point of measuring against them. Figures are speed relative to the plain
hand-written loop (1.00 = equal, above 1 = faster), at 100,000 entities of three
mixed shapes, as the range across MSVC 19.51, clang-cl 22 and GCC 16.2 on one
machine (Core Ultra 9 275HX, P-cores, AVX2). Every design computes bit-identical
results.

| | One field | Two-component physics | Three systems per frame | Random lookup |
|---|---:|---:|---:|---:|
| Hand-tuned SIMD (the ceiling) | 2.8–4.0× | 3.7–5.7× | 2.7–4.8× | 1.2–1.3× |
| **Plain hand-written loop** | **1.00** | **1.00** | **1.00** | **1.00** |
| **SubzeroECS** | **0.96–1.02×** | **1.00–1.07×** | **1.02–1.35×** | **0.23–0.28×** |
| SubzeroECS, same work as 3 / 5 small systems, fused | | 0.99–1.03× | 0.95–1.39× | |
| Class hierarchy, virtual update | 0.10–0.14× | 0.42–0.60× | 0.45–0.81× | 0.44–0.50× |
| Game objects owning components | 0.02–0.03× | 0.07–0.09× | 0.03–0.05× | 0.04–0.06× |

What that says, honestly:

- **Iteration costs nothing over a hand-written loop**, on all three compilers,
  and stays there when the work is split into small systems and fused. It is
  1.7–10× faster than a class hierarchy and 12–40× faster than objects that own
  their components.
- **The ceiling is a further 3–6× up** (2.7–5.7× measured). Hand-tuned code gets it from storing one
  array per field and from SIMD. The library does neither yet; closing that gap is
  on the [backlog](docs/BACKLOG.md).
- **Random lookup by handle is 4× slower than indexing an array**, and about half
  the speed of following a pointer to an object. A handle is checked and resolved
  through two tables; that is the price of entities that can change shape and be
  destroyed safely.

Flexibility is where the hand-written code has no answer at all, so here the
reference is a sparse-set ECS (our own implementation of the EnTT model):

| | SubzeroECS vs sparse set |
|---|---:|
| Query on a component 1% of entities have | 3.2–4.0× |
| Create entities | 1.8–2.6× |
| Destroy and create 10% of entities | 1.1–1.5× |
| Add and remove a component no system queries | 0.84–1.23× |
| Add and remove a component a system does query | 0.12–0.21× |

Changing a component that a system queries moves the entity between partitions,
and is 5–8× slower than a sparse set. Mark such components `Volatile`, or expect
that cost.

Every figure is the median of five interleaved runs per compiler; the spread and
the full tables are in
[bench/results/reference/](bench/results/reference/README.md), the design evidence
in [docs/FINDINGS.md](docs/FINDINGS.md), and the method (and how to reproduce it on
your hardware) in [bench/BENCHMARKING.md](bench/BENCHMARKING.md). The references
are described in [bench/README.md](bench/README.md). Comparisons with EnTT and
flecs themselves have not been run yet.

## Build

```bash
cmake --preset default          # Release: tests + benchmarks (Ninja; on Windows use a VS developer prompt)
cmake --build --preset default
ctest --preset default
```

Other presets: `debug`, `sanitize` (ASan/UBSan), `bench-native`, `bench-portable`,
`ci-msvc` (Visual Studio generator). To use the library from another CMake project,
`add_subdirectory` (or FetchContent) it and link `Sub0ECS::Sub0ECS`.
The examples are built and registered with CTest whenever
`SUB0ECS_BUILD_TESTING=ON`; run them with
`ctest --preset default -R '^Sub0ECS_Example_'`.

## Repository layout

```
include/sub0ecs/   the library (header-only)
examples/          runnable feature examples, registered with CTest
tests/             unit and conformance tests (doctest)
bench/             benchmarks, the reference designs they compare against, harness, results
docs/              design and evidence, examples guide, research notes, backlog
```

Design and evidence: [docs/FINDINGS.md](docs/FINDINGS.md). Open work:
[docs/BACKLOG.md](docs/BACKLOG.md).

## Related projects

- [EnTT](https://github.com/skypjack/entt): sparse-set ECS, C++17
- [flecs](https://github.com/SanderMertens/flecs): archetype ECS with relationships

## Licensing

SubzeroECS is dual-licensed:

- **Open Source**: AGPL-3.0-only (see [LICENSE](LICENSE))
- **Commercial**: Proprietary license for closed-source embedding, private modifications, or SaaS operation without AGPL disclosure

**Why AGPL?**
Ensures improvements used in network-accessible deployments remain available to the community.

**How to Request Commercial Terms:**
Open a GitHub issue titled "Commercial License Request: [Your Organization Name]" with your intended use, scale, and timeline.

**Historical Versions:**
Releases prior to v1.0 were published under the Unlicense (public domain). The
earlier API is available at the [`v1.0.0` tag](https://github.com/CraigHutchinson/Sub0ECS/tree/v1.0.0).

**Contributions:**
See [CONTRIBUTING.md](CONTRIBUTING.md) for dual-license inbound contribution terms.
