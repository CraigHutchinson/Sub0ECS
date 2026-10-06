/** Use when: you cannot tell in advance which plan is fastest for a workload, or it
 *  changes with the data (entity counts, shapes) or the machine.
 *  Demonstrates: AutoTuner<Planners...>. It trials each compile-time candidate plan
 *  on the live workload, then keeps running the fastest.
 *  Story: a three-system frame is tuned over NeverFuse, ShareColumns and AlwaysFuse
 *  while the simulation keeps running.
 *  Keep in mind: trialling is safe because every legal plan produces the same
 *  state; the frames spent measuring are real frames. Which plan wins is a timing
 *  result and may differ between runs and machines, so the check here is on the
 *  state and on the tuner reaching a decision, never on which plan it picked.
 *  Run: ctest --preset default -R Sub0ECS_Example_auto_tuner
 */
#include <cstddef>
#include <cstdio>
#include <tuple>
#include <vector>

#include "sub0ecs/sub0ecs.hpp"

namespace fz = sub0ecs::fusion;

struct Position { float x = 0.0f, y = 0.0f; };
struct Velocity { float dx = 0.0f, dy = 0.0f; };
struct Rotation { float angle = 0.0f; };

struct Integrate
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position& p, Velocity& v) const { p.x += v.dx; p.y += v.dy; }
};
struct Drag
{
    using Query = sub0ecs::Query<Position, Velocity>;
    void operator()(Position&, Velocity& v) const { v.dx *= 0.5f; v.dy *= 0.5f; }
};
struct Spin
{
    using Query = sub0ecs::Query<Rotation>;
    void operator()(Rotation& r) const { r.angle += 0.25f; }
};

using Queries = std::tuple<sub0ecs::Query<Position, Velocity>, sub0ecs::Query<Rotation>>;
using World = sub0ecs::store::World<Queries>;
using Tuner = fz::AutoTuner<fz::NeverFuse, fz::ShareColumns, fz::AlwaysFuse>;

namespace
{
    constexpr int kEntities = 5000;

    std::vector<sub0ecs::Entity> populate(World& world)
    {
        std::vector<sub0ecs::Entity> es;
        for (int i = 0; i < kEntities; ++i)
            es.push_back(world.create(Position{ float(i), 0.0f }, Velocity{ float(i % 5), 1.0f }, Rotation{ float(i) }));
        return es;
    }

    std::vector<float> state(World& world, const std::vector<sub0ecs::Entity>& es)
    {
        std::vector<float> out;
        for (const sub0ecs::Entity e : es)
        {
            const Position* p = world.find<Position>(e);
            const Velocity* v = world.find<Velocity>(e);
            out.insert(out.end(), { p->x, p->y, v->dx, v->dy, world.find<Rotation>(e)->angle });
        }
        return out;
    }
} // namespace

int main()
{
    // Enough frames for every candidate's trials, plus some under the chosen plan.
    constexpr int kTrialFrames = static_cast<int>(Tuner::kCandidates) * Tuner::kTrialsPerCandidate;
    constexpr int kFrames = kTrialFrames + 10;

    World tuned;
    const auto tunedEntities = populate(tuned);
    Tuner tuner;
    fz::Inline exec;
    bool decidedOnTime = false;
    for (int frame = 0; frame < kFrames; ++frame)
    {
        if (frame == kTrialFrames) decidedOnTime = tuner.decided();
        tuner.run(tuned, exec, exec, Integrate{}, Drag{}, Spin{});
    }

    World reference;
    const auto referenceEntities = populate(reference);
    for (int frame = 0; frame < kFrames; ++frame)
    {
        reference.each<Position, Velocity>([](Position& p, Velocity& v) { Integrate{}(p, v); });
        reference.each<Position, Velocity>([](Position& p, Velocity& v) { Drag{}(p, v); });
        reference.each<Rotation>([](Rotation& r) { Spin{}(r); });
    }

    int failures = 0;
    const auto expect = [&](bool ok, const char* what) {
        if (!ok)
        {
            ++failures;
            std::printf("FAILED: %s\n", what);
        }
    };
    expect(decidedOnTime, "the tuner decides after trialling every candidate");
    expect(tuner.chosen() < Tuner::kCandidates, "the chosen plan is one of the candidates");
    expect(state(tuned, tunedEntities) == state(reference, referenceEntities),
           "tuning (including the trial frames) produces the sequential state");

    std::printf("chose %s after %d trial frames\n", Tuner::names()[tuner.chosen()], kTrialFrames);
    return failures == 0 ? 0 : 1;
}
