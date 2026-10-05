/** H9 dynamic system lifetimes: a system registered at runtime whose query
 * requires a side-stored (Volatile) component forces a relayout. Whatever the
 * migration budget — never migrate (fully degraded), bounded incremental
 * steps, or one stall — results must equal a reference, every holder must be
 * visited exactly once per frame, and structural churn DURING migration must
 * be handled. Also: enable/disable of systems inside fused groups.
 */
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "../bench/common/scenarios.hpp"
#include "../bench/common/systems.hpp"
#include "../bench/designs/query_partition.hpp"
#include "../bench/designs/sparse_set.hpp"
#include <doctest/doctest.h>

using namespace bench;

namespace
{
    void check(bool ok, const std::string& what) { CHECK_MESSAGE(ok, what); }

    constexpr std::int64_t kN = 20'000;
    constexpr int kFrames = 40;

    /** The paged-in system: row-local, uses the formerly side-stored Frozen. */
    void kernelFrozen(Position& p, Velocity& v, Frozen& f)
    {
        ++f.ticks;
        p.x += 0.5f * static_cast<float>(f.ticks % 3);
        v.dx *= 0.999f;
    }

    /** Structural churn applied identically to both worlds each frame. */
    template <typename W>
    void churn(W& w, std::vector<typename W::Entity>& es, int frame)
    {
        for (std::size_t i = static_cast<std::size_t>(frame % 7); i < es.size(); i += 97)
        {
            if (w.template find<Frozen>(es[i])) w.template remove<Frozen>(es[i]);
            else w.add(es[i], Frozen{ frame });
        }
        for (std::size_t i = static_cast<std::size_t>(frame % 13); i < es.size(); i += 389)
        {
            w.destroy(es[i]);
            es[i] = w.create(Position{ 1.f * frame, 2.f }, Velocity{ 3.f, 4.f }, Frozen{ frame });
        }
        w.commit();
    }

    template <typename W>
    double digest(W& w, const std::vector<typename W::Entity>& es)
    {
        double sum = checksum(w, es);
        for (const auto& e : es)
            if (auto* f = w.template find<Frozen>(e)) sum += f->ticks * 3.0;
        return sum;
    }

    struct Outcome
    {
        double digest = 0.0;
        std::vector<std::size_t> visits;
        int flippedAtFrame = -1;
    };

    Outcome reference()
    {
        auto w = std::make_unique<sparse::World>();
        auto es = populate(*w, kN, Pattern::Fragmented);
        for (std::size_t i = 0; i < es.size(); i += 2) w->add(es[i], Frozen{ 0 });
        Outcome o;
        for (int f = 0; f < kFrames; ++f)
        {
            std::size_t n = 0;
            w->template each<Position, Velocity, Frozen>([&](Position& p, Velocity& v, Frozen& fr) { kernelFrozen(p, v, fr); ++n; });
            o.visits.push_back(n);
            churn(*w, es, f);
        }
        o.digest = digest(*w, es);
        return o;
    }

    Outcome dynamic(std::size_t budget)
    {
        using W = qpart::HintedWorld;   // Frozen is Volatile: side storage
        auto w = std::make_unique<W>();
        auto es = populate(*w, kN, Pattern::Fragmented);
        for (std::size_t i = 0; i < es.size(); i += 2) w->add(es[i], Frozen{ 0 });
        const std::size_t q = w->template addQuery<Position, Velocity, Frozen>();   // paged-in system
        Outcome o;
        for (int f = 0; f < kFrames; ++f)
        {
            std::size_t n = 0;
            w->template eachDyn<Position, Velocity, Frozen>(q, [&](Position& p, Velocity& v, Frozen& fr) { kernelFrozen(p, v, fr); ++n; });
            o.visits.push_back(n);
            churn(*w, es, f);
            if (budget) w->migrateStep(budget);   // bounded restructuring per frame
            if (o.flippedAtFrame < 0 && !w->queryDegraded(q)) o.flippedAtFrame = f;
        }
        o.digest = digest(*w, es);
        return o;
    }
} // namespace

TEST_CASE("H9 paging a system in: every migration budget equals the reference")
{
    const Outcome ref = reference();

    const std::size_t kStall = std::numeric_limits<std::size_t>::max();
    for (std::size_t budget : { std::size_t{ 0 }, std::size_t{ 100 }, std::size_t{ 1000 }, kStall })
    {
        const Outcome o = dynamic(budget);
        const std::string tag = budget == 0 ? "never migrate (degraded)" : budget == kStall ? "stall (migrate all)" : "budget " + std::to_string(budget) + "/frame";
        check(o.digest == ref.digest, tag + ": result equals reference");
        check(o.visits == ref.visits, tag + ": every holder visited exactly once per frame");
        // ~kN/2 holders to migrate: a budget that cannot cover them in kFrames stays degraded.
        const bool canFinish = budget != 0 && budget >= static_cast<std::size_t>(kN) / kFrames;
        if (canFinish) check(o.flippedAtFrame >= 0, tag + ": flips to full path");
        else check(o.flippedAtFrame < 0, tag + ": still degraded (budget too small to finish)");
    }

}

TEST_CASE("H9 enable/disable of systems, in fused groups and as dynamic queries")
{
    // Enable/disable inside a fused group: disabling Forces == running the others.
    {
        auto a = std::make_unique<qpart::HintedWorld>();
        auto b = std::make_unique<qpart::HintedWorld>();
        auto ea = populate(*a, kN, Pattern::Fragmented);
        auto eb = populate(*b, kN, Pattern::Fragmented);
        fusion::Inline exec;
        for (int i = 0; i < 5; ++i)
        {
            a->runFusedOnMasked(exec, 0b101u, Integrate{}, Forces{}, Wrap{});   // Forces disabled
            runSequential(*b, Integrate{}, Wrap{});
        }
        check(checksum(*a, ea) == checksum(*b, eb), "disabled member of a fused group is skipped exactly");

        const std::size_t q = a->template addQuery<Position, Velocity>();
        a->setQueryEnabled(q, false);
        std::size_t n = 0;
        a->template eachDyn<Position, Velocity>(q, [&](Position&, Velocity&) { ++n; });
        check(n == 0, "disabled dynamic system does not run");
        check(!a->queryDegraded(q), "query over already-dense components needs no relayout");
    }
}
