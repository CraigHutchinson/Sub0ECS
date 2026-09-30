/** Skirmish determinism: every storage design (and fused vs sequential
 * execution) must play the *identical* game — same checksum at every
 * checkpoint. This is the lockstep-RTS requirement, and the guard that makes
 * the benchmark comparison meaningful.
 */
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "sim.hpp"
#include "worlds.hpp"

using namespace skirmish;

namespace
{
    constexpr int kTicks = 360;
    constexpr int kEvery = 60;

    struct Trace
    {
        std::vector<double> checksums;
        Stats final;
    };

    template <typename W, bool Fused>
    Trace play(Config cfg)
    {
        auto world = std::make_unique<W>();
        Sim<W, Fused> sim(*world, cfg);
        Trace tr;
        for (int t = 1; t <= kTicks; ++t)
        {
            sim.tick();
            if (t % kEvery == 0) tr.checksums.push_back(sim.checksum());
        }
        tr.final = sim.stats();
        return tr;
    }

    int failures = 0;

    void compare(const char* name, const Trace& ref, const Trace& tr)
    {
        bool ok = tr.checksums == ref.checksums && tr.final.entities == ref.final.entities &&
                  tr.final.projectiles == ref.final.projectiles && tr.final.units == ref.final.units &&
                  tr.final.resources == ref.final.resources && tr.final.kills == ref.final.kills;
        std::printf("  %-22s %s  final checksum %.6f\n", name, ok ? "identical" : "DIVERGED ", tr.checksums.back());
        if (!ok)
        {
            ++failures;
            for (std::size_t i = 0; i < tr.checksums.size(); ++i)
                if (tr.checksums[i] != ref.checksums[i])
                {
                    std::printf("    first divergence at tick %d\n", static_cast<int>((i + 1) * kEvery));
                    break;
                }
        }
    }
} // namespace

int main()
{
    Config cfg;
    cfg.unitsPerTeam = 300;
    std::printf("Skirmish determinism: %d teams x %d units, %d ticks\n", cfg.teams, cfg.unitsPerTeam, kTicks);

    const Trace ref = play<SparseSetWorld, false>(cfg);
    const Stats& s = ref.final;
    std::printf("  reference (SparseSet): entities=%lld projectiles=%lld kills=%d/%d/%d/%d resources=%d/%d/%d/%d\n",
                static_cast<long long>(s.entities), static_cast<long long>(s.projectiles), s.kills[0], s.kills[1],
                s.kills[2], s.kills[3], s.resources[0], s.resources[1], s.resources[2], s.resources[3]);

    // The game must actually exercise the systems it claims to.
    const int kills = s.kills[0] + s.kills[1] + s.kills[2] + s.kills[3];
    if (kills == 0 || s.projectiles == 0 || s.resources[0] == 500)
    {
        std::printf("  FAIL: reference game did not exercise combat/projectiles/economy\n");
        ++failures;
    }

    compare("Archetype", ref, play<ArchetypeWorld, false>(cfg));
    compare("SortedSoA", ref, play<SortedWorld, false>(cfg));
    compare("StaticBitmask", ref, play<StaticWorld<(1 << 13)>, false>(cfg));
    compare("QueryPart", ref, play<QueryPartWorld, false>(cfg));
    compare("QueryPart (fused)", ref, play<QueryPartWorld, true>(cfg));
    compare("QPartHinted", ref, play<QPartHintedWorld, false>(cfg));
    compare("QPartHinted (fused)", ref, play<QPartHintedWorld, true>(cfg));

    std::printf(failures ? "\n%d FAILURE(S)\n" : "\nALL DESIGNS PLAY THE IDENTICAL GAME\n", failures);
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
