/** Micro: cost of iterating one system over K partitions (spans) vs 1 span, same
 * total N. Evidence for docs/research/design-review-pre-h2.md, section 3 (is cross-partition
 * contiguity worth pursuing?). One group; the baseline is 1 span, so each ratio is the
 * cost of splitting the same rows into K spans.
 */
#include <memory>
#include <string>
#include <vector>

#include "common/components.hpp"
#include "harness/harness.hpp"

using namespace bench;

namespace
{
    constexpr std::size_t kRows = 100'000;

    struct Spans
    {
        std::vector<std::vector<Position>> pos;
        std::vector<std::vector<Velocity>> vel;
    };
} // namespace

int main(int argc, char** argv)
{
    harness::Registry registry;
    for (std::size_t k : { 1u, 6u, 64u, 512u, 4096u, 16384u })
    {
        registry.add(harness::Case{
            "Spans", "-", "k" + std::to_string(k), static_cast<std::int64_t>(kRows), static_cast<double>(kRows), "pass", false, 0,
            [k] {
                auto fx = std::make_shared<Spans>();
                fx->pos.resize(k);
                fx->vel.resize(k);
                for (std::size_t i = 0; i < kRows; ++i)
                {
                    fx->pos[i % k].push_back({ float(i % 997), float(i % 13) });
                    fx->vel[i % k].push_back({ 1.f, 2.f });
                }
                harness::Prepared prep;
                prep.op = [fx] {
                    for (std::size_t p = 0; p < fx->pos.size(); ++p)
                    {
                        Position* a = fx->pos[p].data();
                        Velocity* b = fx->vel[p].data();
                        for (std::size_t i = 0, m = fx->pos[p].size(); i < m; ++i) kernel::updatePosition(a[i], b[i], 1.f / 60.f);
                    }
                };
                prep.finish = [k](harness::Record& r) { r.counter("per_span", double(kRows) / double(k)); };
                prep.fixture = fx;
                return prep;
            } });
    }
    return harness::benchMain(argc, argv, registry, "sub0ecs_spans_bench");
}
