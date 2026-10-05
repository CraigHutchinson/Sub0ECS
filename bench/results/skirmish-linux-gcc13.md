# Skirmish RTS — ms per tick (speed-up vs SparseSet)

- Host: 4 × 2100 MHz | 2026-09-30T15:27:37+00:00 | median of 5, random interleaving, ticks 150–350 of the game
- units per team → total units: 250 → 1K, 2 500 → 10K, 12 500 → 50K (plus projectiles and buildings)

| Design | 1,000 units | 10,000 units | 50,000 units |
|---|---:|---:|---:|
| SparseSet | 0.144 (1.00×) | 2.403 (1.00×) | 14.445 (1.00×) |
| Archetype | 0.140 (1.03×) | 2.227 (1.08×) | 12.238 (1.18×) |
| QueryPart | 0.137 (1.05×) | 2.323 (1.03×) | 12.672 (1.14×) |
| QueryPartFused | 0.130 (1.11×) | 2.194 (1.10×) | 11.967 (1.21×) |
| QPartHinted | 0.139 (1.04×) | 2.268 (1.06×) | 12.502 (1.16×) |
| QPartHintedFused | 0.131 (1.10×) | 2.255 (1.07×) | 11.783 (1.23×) |
| StaticBitmask | 0.142 (1.02×) | 2.205 (1.09×) | 11.554 (1.25×) |
| SortedSoA | 0.294 (0.49×) | n/a | n/a |

## Per-system µs/tick at 1,000 units

| System | SparseSet | Archetype | QueryPart | QueryPartFused | QPartHinted | QPartHintedFused | StaticBitmask | SortedSoA |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| population | 2 | 1 | 1 | 1 | 1 | 1 | 3 | 4 |
| production | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
| grid | 17 | 13 | 13 | 13 | 13 | 13 | 14 | 18 |
| selection | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
| acquire | 19 | 19 | 19 | 19 | 19 | 19 | 19 | 36 |
| commander | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |
| workers | 3 | 3 | 4 | 4 | 4 | 4 | 2 | 22 |
| movement | 73 | 76 | 70 | 63 | 72 | 64 | 76 | 80 |
| arrive | 1 | 1 | 1 | 1 | 1 | 1 | 3 | 7 |
| combat | 18 | 18 | 20 | 21 | 20 | 21 | 12 | 85 |
| projectiles | 5 | 4 | 4 | 4 | 4 | 4 | 4 | 24 |
| status | 0 | 1 | 0 | 0 | 1 | 0 | 1 | 3 |
| death | 2 | 1 | 1 | 1 | 1 | 1 | 3 | 9 |
| regen | 1 | 1 | 1 | 1 | 1 | 1 | 2 | 1 |

entities at end of window ≈ 856, projectiles ≈ 106

## Per-system µs/tick at 10,000 units

| System | SparseSet | Archetype | QueryPart | QueryPartFused | QPartHinted | QPartHintedFused | StaticBitmask |
|---|---:|---:|---:|---:|---:|---:|---:|
| population | 21 | 9 | 10 | 10 | 10 | 10 | 25 |
| production | 2 | 2 | 2 | 2 | 2 | 2 | 5 |
| grid | 180 | 160 | 165 | 161 | 162 | 165 | 142 |
| selection | 3 | 4 | 4 | 4 | 3 | 4 | 6 |
| acquire | 565 | 544 | 566 | 565 | 556 | 581 | 503 |
| commander | 2 | 2 | 2 | 2 | 3 | 3 | 1 |
| workers | 31 | 25 | 44 | 43 | 39 | 42 | 21 |
| movement | 1295 | 1280 | 1275 | 1174 | 1265 | 1193 | 1266 |
| arrive | 17 | 8 | 9 | 9 | 8 | 9 | 43 |
| combat | 191 | 153 | 189 | 182 | 179 | 195 | 99 |
| projectiles | 55 | 26 | 35 | 34 | 29 | 31 | 40 |
| status | 2 | 2 | 2 | 2 | 2 | 2 | 13 |
| death | 23 | 10 | 10 | 10 | 10 | 11 | 21 |
| regen | 12 | 8 | 8 | 8 | 7 | 8 | 17 |

entities at end of window ≈ 9328, projectiles ≈ 1039

## Per-system µs/tick at 50,000 units

| System | SparseSet | Archetype | QueryPart | QueryPartFused | QPartHinted | QPartHintedFused | StaticBitmask |
|---|---:|---:|---:|---:|---:|---:|---:|
| population | 121 | 79 | 84 | 82 | 86 | 80 | 106 |
| production | 10 | 5 | 6 | 5 | 6 | 5 | 22 |
| grid | 1141 | 851 | 893 | 870 | 882 | 848 | 847 |
| selection | 27 | 20 | 19 | 19 | 19 | 19 | 45 |
| acquire | 4321 | 4121 | 4305 | 4227 | 4235 | 4171 | 3750 |
| commander | 11 | 8 | 12 | 11 | 11 | 11 | 3 |
| workers | 193 | 173 | 235 | 223 | 225 | 202 | 152 |
| movement | 6744 | 6051 | 5979 | 5437 | 5951 | 5410 | 5654 |
| arrive | 248 | 60 | 73 | 71 | 72 | 68 | 135 |
| combat | 1096 | 635 | 785 | 743 | 789 | 767 | 314 |
| projectiles | 247 | 94 | 131 | 124 | 111 | 106 | 211 |
| status | 9 | 5 | 5 | 4 | 5 | 5 | 88 |
| death | 181 | 91 | 103 | 92 | 99 | 94 | 122 |
| regen | 97 | 44 | 47 | 45 | 47 | 44 | 67 |

entities at end of window ≈ 48282, projectiles ≈ 2400

⚠ = CV > 10%.
