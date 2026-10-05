/** Use when: you want to see storage, scheduling and structural change working
 *  together in something game-shaped, rather than one feature at a time.
 *  Demonstrates: the maintained Skirmish RTS testbed (bench/skirmish/): four teams
 *  with workers, production, movement, combat, projectiles and death, written
 *  once against the store's interface.
 *  Story: the same battle is played twice on the library store, once with one pass
 *  per system and once with the movement systems fused, and must come out the same.
 *  Keep in mind: the testbed is the reference for structuring a real game (systems,
 *  command buffers, a spatial grid, per-worker buffers); read bench/skirmish/README.md
 *  and sim.hpp. Its full lockstep check, across every storage design, planner and
 *  executor, is tests/test_skirmish.cpp; this example is the two-line version.
 *  Run: ctest --preset default -R Sub0ECS_Example_skirmish
 */
#include <cstdio>
#include <memory>
#include <vector>

#include "skirmish/sim.hpp"
#include "skirmish/worlds.hpp"

namespace
{
    constexpr int kTicks = 360;   // the same battle tests/test_skirmish.cpp plays: long enough for combat
    constexpr int kEvery = 60;

    struct Battle
    {
        std::vector<double> checksums;   // one per checkpoint
        skirmish::Stats result;
    };

    template <bool Fused>
    Battle play()
    {
        skirmish::Config config;
        config.unitsPerTeam = 300;
        auto world = std::make_unique<skirmish::QPartHintedWorld>();
        skirmish::Sim<skirmish::QPartHintedWorld, Fused> sim(*world, config);
        Battle battle;
        for (int tick = 1; tick <= kTicks; ++tick)
        {
            sim.tick();
            if (tick % kEvery == 0) battle.checksums.push_back(sim.checksum());
        }
        battle.result = sim.stats();
        return battle;
    }
} // namespace

int main()
{
    const Battle sequential = play<false>();
    const Battle fused = play<true>();

    int failures = 0;
    const auto expect = [&](bool ok, const char* what) {
        if (!ok)
        {
            ++failures;
            std::printf("FAILED: %s\n", what);
        }
    };
    const auto& s = sequential.result;
    expect(s.kills[0] + s.kills[1] + s.kills[2] + s.kills[3] > 0, "the battle really happened: units were killed");
    expect(fused.checksums == sequential.checksums, "fused and sequential play the identical game at every checkpoint");
    expect(fused.result.entities == s.entities && fused.result.kills == s.kills, "and end with the same armies");

    std::printf("%d ticks: %lld entities alive, kills per team %d/%d/%d/%d\n", kTicks, static_cast<long long>(s.entities),
                s.kills[0], s.kills[1], s.kills[2], s.kills[3]);
    return failures == 0 ? 0 : 1;
}
