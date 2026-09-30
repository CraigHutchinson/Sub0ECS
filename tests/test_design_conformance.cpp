/** Conformance: every design must produce bit-identical state for the same
 * workload, otherwise the benchmark comparison is meaningless.
 */
#include <memory>
#include <string>

#include "doctest.h"

#include "../bench/common/scenarios.hpp"
#include "../bench/common/systems.hpp"
#include "../bench/designs/archetype.hpp"
#include "../bench/designs/query_partition.hpp"
#include "../bench/designs/sorted_soa.hpp"
#include "../bench/designs/sparse_set.hpp"
#include "../bench/designs/static_bitmask.hpp"
#include "../bench/designs/v1_adapter.hpp"

namespace
{
    void check(bool ok, const std::string& what) { CHECK_MESSAGE(ok, what); }

    struct Result
    {
        double core = 0.0;
        double structural = 0.0;
        std::size_t matchedFrame = 0;
        std::size_t matchedSparse = 0;
    };

    constexpr std::int64_t kN = 12'000;

    template <typename W>
    Result run(bench::Pattern pattern)
    {
        using namespace bench;
        Result r;
        auto w = std::make_unique<W>();
        auto es = populate(*w, kN, pattern);

        for (int i = 0; i < 3; ++i) systemPhysics(*w);
        systemFrame3(*w);
        systemIter1(*w);
        tagEveryHundredth(*w, es);
        systemSparse(*w);
        systemSparse(*w);

        w->template each<Health, Rotation>([&](Health&, Rotation&) { ++r.matchedFrame; });
        w->template each<Position, Velocity, Tag>([&](Position&, Velocity&, Tag&) { ++r.matchedSparse; });
        r.core = checksum(*w, es);

        if constexpr (W::kSupportsRemove && W::kSupportsDestroy)
        {
            // Structural edits that change which systems match (move data in
            // partitioned designs) and that promote/demote side storage:
            //  - untag every 200th tagged entity (queried component removed)
            //  - give Medium entities Color => they start matching Pulse, so
            //    Scale is promoted from side storage to a column (QueryPart)
            //  - strip Rotation from some Large entities => RotHealth no longer
            //    matches, so Health is demoted to side storage (QueryPart)
            for (std::size_t i = 0; i < es.size(); i += 200) w->template remove<Tag>(es[i]);
            for (std::size_t i = 0; i < es.size(); i += 5)
            {
                if (w->template find<Scale>(es[i]) && !w->template find<Color>(es[i]))
                    w->add(es[i], Color{ 0.25f, 0.5f, 0.75f, 1.0f });
                else if (i % 7 == 0 && w->template find<Rotation>(es[i]) && w->template find<Color>(es[i]))
                    w->template remove<Rotation>(es[i]);
            }
            w->commit();
            systemFrame3(*w);
            systemSparse(*w);

            churnAddRemove(*w, es);
            check(w->template find<Frozen>(es[0]) == nullptr, std::string(W::kName) + ": Frozen removed");

            Rng rng(7);
            const auto stale = es[0];
            for (std::size_t round = 0; round < 3; ++round) churnDestroyCreate(*w, es, round, rng);
            check(w->template find<Position>(stale) == nullptr, std::string(W::kName) + ": stale handle rejected");

            for (int i = 0; i < 2; ++i) systemFrame3(*w);
            systemSparse(*w);
            r.structural = checksum(*w, es);

            std::size_t live = 0;
            w->template each<Position>([&](Position&) { ++live; });
            check(live == static_cast<std::size_t>(kN), std::string(W::kName) + ": live count after churn");
        }
        return r;
    }

    template <typename W>
    void compare(const char* name, bench::Pattern pattern, const Result& ref, bool structural)
    {
        const Result r = run<W>(pattern);
        const std::string tag = std::string(name) + "/" + bench::toString(pattern);
        check(r.core == ref.core, tag + ": core checksum");
        check(r.matchedFrame == ref.matchedFrame, tag + ": Health+Rotation match count");
        check(r.matchedSparse == ref.matchedSparse, tag + ": sparse match count");
        if (structural) check(r.structural == ref.structural, tag + ": structural checksum");
    }

    // ---- H7 fusion: fused passes must equal sequential passes bit-for-bit ----

    enum class Exec { Sequential, Fused, Grouped, Update2Kernel };

    template <typename W>
    double fusionRun(bench::Pattern pattern, Exec exec)
    {
        using namespace bench;
        auto w = std::make_unique<W>();
        auto es = populate(*w, kN, pattern);
        tagEveryHundredth(*w, es);
        auto frame = [&] {
            if (exec == Exec::Sequential) { systemFusionFrame(*w); systemFrame3Seq(*w); }
            else if (exec == Exec::Update2Kernel)
            {
                // FusionFrame's split kernels applied in order == kernel::updatePosition
                systemPhysics(*w);
                runSequential(*w, RotHealthSys{});
                systemFrame3Seq(*w);
            }
            else if (exec == Exec::Grouped)
            {
                if constexpr (requires { w->runFused(Integrate{}); }) { systemFusionFrameGrouped(*w); systemFrame3Seq(*w); }
            }
            else
            {
                if constexpr (requires { w->runFused(Integrate{}); }) { systemFusionFrameFused(*w); systemFrame3Fused(*w); }
            }
        };
        for (int i = 0; i < 3; ++i) frame();
        // Structural change between frames: partitions' system subsets change
        // (Medium entities gain Color -> start matching Pulse).
        for (std::size_t i = 0; i < es.size(); i += 4)
        {
            if (w->template find<Scale>(es[i]) && !w->template find<Color>(es[i])) w->add(es[i], Color{});
        }
        w->commit();
        for (int i = 0; i < 2; ++i) frame();
        return checksum(*w, es);
    }
} // namespace

TEST_CASE("every design reproduces the reference state bit-for-bit")
{
    using namespace bench;
    for (Pattern pattern : { Pattern::Coherent, Pattern::Fragmented })
    {
        CAPTURE(toString(pattern));
        const Result ref = run<sparse::World>(pattern);
        check(ref.matchedSparse == static_cast<std::size_t>((kN + 99) / 100), "reference sparse match count");

        compare<v1::World>("V1", pattern, ref, false);
        compare<sorted::World>("SortedSoA", pattern, ref, true);
        compare<archetype::World>("Archetype", pattern, ref, true);
        compare<fixed::World<16384>>("StaticBitmask", pattern, ref, true);
        compare<qpart::World>("QueryPart", pattern, ref, true);
        compare<qpart::HintedWorld>("QPartHinted", pattern, ref, true);
    }
}

TEST_CASE("H7 fused passes equal sequential passes bit-for-bit")
{
    using namespace bench;
    for (Pattern pattern : { Pattern::Coherent, Pattern::Fragmented })
    {
        const std::string ft = std::string("fusion/") + toString(pattern);
        const double fRef = fusionRun<sparse::World>(pattern, Exec::Sequential);
        check(fusionRun<sparse::World>(pattern, Exec::Update2Kernel) == fRef, ft + ": split kernels == Update2 kernel");
        check(fusionRun<qpart::HintedWorld>(pattern, Exec::Grouped) == fRef, ft + ": grouped fusion == sequential");
        check(fusionRun<qpart::HintedWorld>(pattern, Exec::Sequential) == fRef, ft + ": sequential QPartHinted == SparseSet");
        check(fusionRun<qpart::HintedWorld>(pattern, Exec::Fused) == fRef, ft + ": fused QPartHinted == sequential");
        check(fusionRun<qpart::World>(pattern, Exec::Fused) == fRef, ft + ": fused QueryPart == sequential");
    }
}
