/** Skirmish demo: play one game headless and report what happened.
 *
 *   sub0ecs_skirmish_demo [unitsPerTeam=1000] [ticks=900]
 *
 * Runs on the recommended design (query partitions, hinted, fused movement).
 */
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#if defined(__SSE__) || defined(_M_X64)
#    include <xmmintrin.h>
#endif

#include "sim.hpp"
#include "worlds.hpp"

using namespace skirmish;

int main(int argc, char** argv)
{
#if defined(__SSE__) || defined(_M_X64)
    _mm_setcsr(_mm_getcsr() | 0x8040u);
#endif
    Config cfg;
    cfg.unitsPerTeam = argc > 1 ? std::atoi(argv[1]) : 1000;
    const int ticks = argc > 2 ? std::atoi(argv[2]) : 900;

    auto world = std::make_unique<QPartHintedWorld>();
    Sim<QPartHintedWorld, true> sim(*world, cfg);
    sim.enableTimings(true);

    std::printf("Skirmish: %d teams x %d units, map %.0f x %.0f, %d ticks @30Hz (%.0fs game time)\n\n", cfg.teams,
                cfg.unitsPerTeam, sim.mapSize(), sim.mapSize(), ticks, ticks / 30.0);
    std::printf("%6s %8s %6s | %-23s | %-23s | %-23s\n", "tick", "entities", "shots", "units / team",
                "resources / team", "kills / team");
    for (int t = 0; t <= ticks; ++t)
    {
        if (t % 100 == 0 || t == ticks)
        {
            const Stats s = sim.stats();
            std::printf("%6lld %8lld %6lld | %5d %5d %5d %5d | %5d %5d %5d %5d | %5d %5d %5d %5d   sel=%d\n",
                        static_cast<long long>(s.tick), static_cast<long long>(s.entities),
                        static_cast<long long>(s.projectiles), s.units[0], s.units[1], s.units[2], s.units[3],
                        s.resources[0], s.resources[1], s.resources[2], s.resources[3], s.kills[0], s.kills[1],
                        s.kills[2], s.kills[3], s.selected);
        }
        if (t < ticks) sim.tick();
    }

    // ASCII minimap: majority team per cell ('.' empty)
    constexpr int W = 64, H = 32;
    std::vector<std::array<int, kMaxTeams>> cells(W * H);
    const float L = sim.mapSize();
    sim.world().template each<Id, Position, Team, Health>([&](Id&, Position& p, Team& t, Health&) {
        const int cx = std::min(W - 1, static_cast<int>(p.x / L * W));
        const int cy = std::min(H - 1, static_cast<int>(p.y / L * H));
        ++cells[cy * W + cx][t.value];
    });
    std::printf("\nMinimap (A-D = majority team, lower-case = contested):\n");
    for (int y = 0; y < H; ++y)
    {
        std::string row;
        for (int x = 0; x < W; ++x)
        {
            const auto& c = cells[y * W + x];
            int best = -1, total = 0, bestN = 0;
            for (int t = 0; t < kMaxTeams; ++t) { total += c[t]; if (c[t] > bestN) { bestN = c[t]; best = t; } }
            row += best < 0 ? '.' : static_cast<char>((bestN * 3 >= total * 2 ? 'A' : 'a') + best);
        }
        std::printf("  %s\n", row.c_str());
    }

    const auto& secs = sim.timings();
    double total = 0.0;
    for (double s : secs) total += s;
    std::printf("\nTime per system (%.2f ms/tick total):\n", 1e3 * total / ticks);
    for (int i = 0; i < kSysCount; ++i)
        std::printf("  %-12s %7.3f ms/tick  %5.1f%%\n", kSystemNames[i], 1e3 * secs[i] / ticks, 100.0 * secs[i] / total);
    std::printf("\nchecksum %.6f\n", sim.checksum());
    return 0;
}
