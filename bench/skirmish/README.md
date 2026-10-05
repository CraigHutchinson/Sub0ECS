# Skirmish: RTS testbed for SubzeroECS

*Working name. This is intended to become the parent project: the first
real user of SubzeroECS / Sub0DataStore, and its test bed.*

A deterministic, headless, 4-team real-time-strategy simulation in the
classic RTS style:
- each team has an HQ, barracks and resource mines;
- workers harvest and deposit resources, and HQs and barracks train
  soldiers, archers and knights;
- units march on enemy HQs, acquire targets and fire homing projectiles;
- attacks can burn or stun, and killers gain veterancy.

It is written against the spike's storage-adapter surface, so **every
storage design plays the same game**, and the conformance test proves it
bit for bit.

```
sub0ecs_skirmish_demo 1000 900      # play 30 s of game time, print stats, a minimap and per-system costs
sub0ecs_tests        # all designs + fused/sequential must produce the identical game
sub0ecs_skirmish_bench              # ms per tick per design at 1K / 10K / 50K units, with per-system counters
```

## Why an RTS

A classic RTS converted to ECS exercises almost every property the library
design has to get right, in realistic proportions rather than as isolated
micro-benchmarks:

| Property | Where it appears |
|---|---|
| Many overlapping queries | 18 declared queries over 19 component types |
| Fusable row-local system chains | Movement: Seek → Separation → StunFreeze → Integrate → Friction → Bounds |
| Non-row-local reads | Spatial grid (separation, target acquisition); homing projectiles; random access to targets, mines and HQs |
| Read-only snapshot shared by systems | Grid built once per tick, read by Separation (fused) and Acquire |
| Structural churn on **queried** components | `MoveOrder`, `Target` (orders given, targets acquired and lost); `Stunned`, `Burning` |
| Structural churn on **unqueried** components | `Carrying` (worker loop): the non-fragmenting case from H1 |
| Spawn/destroy churn | Projectiles born and destroyed every tick; units die; buildings produce |
| Rare components | `Selected` (~1%, reselected every 128 ticks), `Veteran`, status effects |
| Reductions | Population per team/kind; selection centroid (float, Id-ordered); resources; kills |
| Shared ("authority") data | `kUnitStats`, one record per unit kind, read by many systems |
| Staggered work | Target acquisition runs for 1/8 of units per tick |
| Determinism (lockstep RTS) | Identical checksums across 7 storage/execution variants |

## Systems (per 30 Hz tick)

| # | System | Query | Kind |
|---|---|---|---|
| 1 | population | `Id, Team, UnitType` | reduction |
| 2 | production | `Id, Building, Producer, Team, Position` | spawns (commands) |
| 3 | grid | `Id, Position, Team, Health` | builds the spatial snapshot |
| 4 | selection | `Id, Selected, Position` | rare-component reduction + reselect churn |
| 5 | acquire | `Id, Position, Team, Weapon, UnitType` | spatial search, staggered, adds `Target` |
| 6 | commander | same as acquire | every 64 ticks, gives `MoveOrder` |
| 7 | workers | `Id, Worker, Position` | harvest loop, random access, `Carrying` churn |
| 8 | movement | 6 fusable systems (above) | row-local, fused on query-partition designs |
| 9 | arrive | `Id, MoveOrder, Position` | removes `MoveOrder` |
| 10 | combat | `Id, Position, Weapon, Target, Team, UnitType` | range checks, fires projectiles |
| 11 | projectiles | `Id, Projectile, Position, Velocity` | homing, hits → damage, expiry |
| 12 | status | `Id, Stunned` / `Id, Burning, Health` | countdowns, removals |
| 13 | death | `Id, Health, Team` | kills, veterancy, destroy |
| 14 | regen | `Health, UnitType` | row-local |

## Determinism rules (how 7 storage variants play the identical game)

- Structural changes go through a **command buffer** that is sorted by the
  stable `Id` component and applied at each system's end (a commit point).
- Spatial grid cells are sorted by `Id`, and neighbour scans visit cells in
  a fixed order, so float sums are order-stable.
- Float reductions (the selection centroid) sum in `Id` order.
- Randomness is `hash(id, tick, salt)`, with no shared RNG stream.
- Denormals are flushed (FTZ/DAZ) in the demo and benchmark.

These rules are also what a real lockstep networked RTS needs, so they
belong to the game, not just the test.

## Designs exercised

SparseSet, Archetype, SortedSoA (1K only in the benchmark: its O(n) flushes
per structural batch dominate), StaticBitmask (capacity per size),
QueryPart and QPartHinted (both sequential and **fused movement**).

## Roadmap for the parent project

1. **Extract** into its own repository once named, depending on SubzeroECS
   (and, through it, Sub0DataStore) instead of the spike's adapters.
2. **Schedule systems with Sub0Pipeline**: derive DAG edges from declared
   access, fused groups as single jobs, parallel partitions. Emit
   add/remove events via **Sub0Pub** (e.g. a HUD subscribing to kills).
3. **Replays and lockstep**: the command stream plus the seed reproduce a
   game exactly, which gives regression tests from recorded games.
4. **Optional front-end** (e.g. a raylib or terminal renderer) as a separate
   target; the simulation stays headless.
5. **Embedded profile**: a 2-team, ≤ 512-unit variant on ESP32-P4 with the
   static-capacity design (spike H4).
6. **More scenarios on the same harness**: tower defence (paths, waves),
   boids (pure fusion), a particle-heavy variant (spawn/destroy stress),
   and hierarchies (squads and garrisons, spike H6′).

Name ideas: **Sub0Skirmish**, **Sub0Front**, **ZeroFront**, **Sub0Siege**.
Your call.
