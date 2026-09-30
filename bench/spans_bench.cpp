// Micro: cost of iterating one system over K partitions (spans) vs 1 span, same total N.
// Evidence for design-review-pre-h2.md §3 (is cross-partition contiguity worth H2?).
#include <benchmark/benchmark.h>
#include <vector>
#include "common/denormals.hpp"
#include "common/components.hpp"
using namespace bench;
static void BM_Spans(benchmark::State& st) {
    const std::size_t n = 100000, k = static_cast<std::size_t>(st.range(0));
    std::vector<std::vector<Position>> pos(k); std::vector<std::vector<Velocity>> vel(k);
    for (std::size_t i = 0; i < n; ++i) { pos[i % k].push_back({float(i % 997), float(i % 13)}); vel[i % k].push_back({1.f, 2.f}); }
    for (auto _ : st) {
        for (std::size_t p = 0; p < k; ++p) {
            Position* a = pos[p].data(); Velocity* b = vel[p].data();
            for (std::size_t i = 0, m = pos[p].size(); i < m; ++i) kernel::updatePosition(a[i], b[i], 1.f / 60.f);
        }
        benchmark::ClobberMemory();
    }
    st.SetItemsProcessed(st.iterations() * n);
    st.counters["per_span"] = double(n) / double(k);
}
BENCHMARK(BM_Spans)->Arg(1)->Arg(6)->Arg(64)->Arg(512)->Arg(4096)->Arg(16384)->Unit(benchmark::kMicrosecond);
int main(int argc, char** argv) { bench::flushDenormals(); benchmark::Initialize(&argc, argv); benchmark::RunSpecifiedBenchmarks(); }
