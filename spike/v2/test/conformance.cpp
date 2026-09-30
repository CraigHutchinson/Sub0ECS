/** Conformance: every design must produce bit-identical state for the same
 * workload, otherwise the benchmark comparison is meaningless.
 */
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "../common/scenarios.hpp"
#include "../designs/archetype.hpp"
#include "../designs/query_partition.hpp"
#include "../designs/sorted_soa.hpp"
#include "../designs/sparse_set.hpp"
#include "../designs/static_bitmask.hpp"
#include "../designs/v1_adapter.hpp"

namespace
{
    int failures = 0;

    void check(bool ok, const std::string& what)
    {
        if (!ok)
        {
            ++failures;
            std::printf("  FAIL: %s\n", what.c_str());
        }
    }

    struct Result
    {
        double core = 0.0;
        double structural = 0.0;
        std::size_t matchedFrame = 0;
        std::size_t matchedSparse = 0;
    };

    constexpr std::int64_t kN = 12'000;

    template <typename W>
    Result run(spike::Pattern pattern)
    {
        using namespace spike;
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
    void compare(const char* name, spike::Pattern pattern, const Result& ref, bool structural)
    {
        const Result r = run<W>(pattern);
        std::printf("  %-14s core=%.6f frame=%zu sparse=%zu structural=%.6f\n", name, r.core, r.matchedFrame,
                    r.matchedSparse, r.structural);
        const std::string tag = std::string(name) + "/" + spike::toString(pattern);
        check(r.core == ref.core, tag + ": core checksum");
        check(r.matchedFrame == ref.matchedFrame, tag + ": Health+Rotation match count");
        check(r.matchedSparse == ref.matchedSparse, tag + ": sparse match count");
        if (structural) check(r.structural == ref.structural, tag + ": structural checksum");
    }
} // namespace

int main()
{
    using namespace spike;
    for (Pattern pattern : { Pattern::Coherent, Pattern::Fragmented })
    {
        std::printf("%s\n", toString(pattern));
        const Result ref = run<sparse::World>(pattern);
        std::printf("  %-14s core=%.6f frame=%zu sparse=%zu structural=%.6f (reference)\n", "SparseSet", ref.core,
                    ref.matchedFrame, ref.matchedSparse, ref.structural);
        check(ref.matchedSparse == static_cast<std::size_t>((kN + 99) / 100), "reference sparse match count");

        compare<v1::World>("V1", pattern, ref, false);
        compare<sorted::World>("SortedSoA", pattern, ref, true);
        compare<archetype::World>("Archetype", pattern, ref, true);
        compare<fixed::World<16384>>("StaticBitmask", pattern, ref, true);
        compare<qpart::World>("QueryPart", pattern, ref, true);
        compare<qpart::HintedWorld>("QPartHinted", pattern, ref, true);
    }
    std::printf(failures ? "\n%d FAILURE(S)\n" : "\nALL DESIGNS CONFORM\n", failures);
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
