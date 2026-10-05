# SubzeroECS

A header-only C++20 Entity Component System whose storage is laid out by the
**systems you declare**, not only by the components entities happen to have.

The current API is the successor to the historical v1 API, preserved at the
[`v1.0.0` tag](https://github.com/CraigHutchinson/Sub0ECS/tree/v1.0.0). The
design evidence is in [docs/FINDINGS.md](docs/FINDINGS.md); implementation
examples and remaining work are tracked in [docs/EXAMPLES.md](docs/EXAMPLES.md)
and [docs/BACKLOG.md](docs/BACKLOG.md).

## Why this design

- **Iteration at hand-written-SoA speed.** Every declared query iterates whole
  partitions of dense, typed columns. The compiler sees a plain loop and
  vectorises it.
- **Cheap churn where it matters.** Components no query filters on never move an
  entity between partitions, so adding and removing them is a sparse-set operation.
  Declare churn-heavy components `Volatile` and they stay out of the layout.
- **System fusion.** Small single-purpose systems that share columns run as one
  pass: planners decide what fuses, executors decide where it runs (inline, tiled,
  thread pool, offload). Results are bit-identical whichever plan or executor is
  chosen.
- **Runtime systems.** Queries added at runtime work immediately, with a bounded
  per-frame migration budget (no level-load stall).

## Quick start

```cpp
#include <tuple>
#include <sub0ecs/sub0ecs.hpp>

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
`remove` of one it lacks does nothing. Components must be trivially copyable, at
most 64 bytes, and a World type supports up to 64 component types.

## Performance

The current store against the v1 API at 100K entities of mixed shapes (GCC 13,
`-O3 -march=native`, median of 5;
[full tables](bench/results/h1-query-partition-linux-gcc13.md)):

| Scenario | v1 API | Current store | |
|---|---:|---:|---:|
| Update two components (v1's headline benchmark) | 294 µs | 42.9 µs | **6.9×**, equal to hand-written SoA |
| Three systems per frame | 459 µs | 118 µs | 3.9× |
| Query on a tag held by 1% of entities | 79 µs | 0.39 µs | 203× |
| Random `find` | 11.1 ms | 1.8 ms | 6.1× |
| Add + remove a component on 10% of entities | unsupported | 89 µs | |

Every number comes from the comparison suite in [bench/](bench/). It runs the
current store beside the alternatives it was chosen over, and v1 unmodified, and
checks they all produce bit-identical results first.
[bench/BENCHMARKING.md](bench/BENCHMARKING.md) explains how to reproduce the
numbers on your hardware.

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
bench/             comparison benchmarks, comparator designs, v1 baseline, harness, results
docs/              design evidence, example plan, research notes, backlog
```

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
Releases prior to v1.0 were published under the Unlicense (public domain).

**Contributions:**
See [CONTRIBUTING.md](CONTRIBUTING.md) for dual-license inbound contribution terms.
