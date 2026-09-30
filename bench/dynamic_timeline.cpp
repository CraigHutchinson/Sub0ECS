/** H9 timeline: cost of paging a system in at runtime.
 *
 *   spike_dynamic_timeline [N=1000000] [frames=60] [out.json]
 *
 * World: QPartHinted, fragmented, Frozen (Volatile → side storage) on 50% of
 * entities. At frame 0 a system over <Position, Velocity, Frozen> is added,
 * which forces Frozen to be promoted to dense columns. Each frame runs the
 * system (degraded or full path) then migrateStep(budget). Per budget we
 * report: degraded vs full per-frame system cost, worst frame (system +
 * migration), frames until the full path, total migration time.
 * Median of 3 trials per metric.
 */
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#if defined(__SSE__) || defined(_M_X64)
#    include <xmmintrin.h>
#endif

#include "common/scenarios.hpp"
#include "designs/query_partition.hpp"

using namespace bench;
using Clock = std::chrono::steady_clock;

namespace
{
    struct Trial
    {
        double degradedUs = 0, fullUs = 0, worstFrameUs = 0, migrationTotalUs = 0;
        int flipFrame = -1;
    };

    double us(Clock::time_point a, Clock::time_point b) { return std::chrono::duration<double, std::micro>(b - a).count(); }

    Trial run(std::int64_t n, int frames, std::size_t budget)
    {
        auto w = std::make_unique<qpart::HintedWorld>();
        auto es = populate(*w, n, Pattern::Fragmented);
        for (std::size_t i = 0; i < es.size(); i += 2) w->add(es[i], Frozen{ 0 });
        const std::size_t q = w->template addQuery<Position, Velocity, Frozen>();
        Trial t;
        std::vector<double> degraded, full;
        for (int f = 0; f < frames; ++f)
        {
            const bool wasDegraded = w->queryDegraded(q);
            const auto t0 = Clock::now();
            w->template eachDyn<Position, Velocity, Frozen>(q, [](Position& p, Velocity& v, Frozen& fr) {
                ++fr.ticks;
                p.x += v.dx * 0.016f;
            });
            const auto t1 = Clock::now();
            if (budget) w->migrateStep(budget);
            const auto t2 = Clock::now();
            (wasDegraded ? degraded : full).push_back(us(t0, t1));
            t.migrationTotalUs += us(t1, t2);
            t.worstFrameUs = std::max(t.worstFrameUs, us(t0, t2));
            if (t.flipFrame < 0 && !w->queryDegraded(q)) t.flipFrame = f;
        }
        auto median = [](std::vector<double> v) {
            if (v.empty()) return 0.0;
            std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
            return v[v.size() / 2];
        };
        t.degradedUs = median(degraded);
        t.fullUs = median(full);
        return t;
    }
} // namespace

int main(int argc, char** argv)
{
#if defined(__SSE__) || defined(_M_X64)
    _mm_setcsr(_mm_getcsr() | 0x8040u);
#endif
    const std::int64_t n = argc > 1 ? std::atoll(argv[1]) : 1'000'000;
    const int frames = argc > 2 ? std::atoi(argv[2]) : 60;
    const char* out = argc > 3 ? argv[3] : nullptr;
    const std::size_t kStall = std::numeric_limits<std::size_t>::max();
    const std::size_t budgets[] = { kStall, 65536, 16384, 4096, 0 };

    std::printf("H9 dynamic system timeline: N=%lld, %lld holders promoted, %d frames, median of 3\n\n",
                static_cast<long long>(n), static_cast<long long>(n / 2), frames);
    std::printf("| Budget (entities/frame) | Degraded system µs/frame | Full-path system µs/frame | Worst frame µs | Frames to full path | Total migration ms |\n");
    std::printf("|---|---:|---:|---:|---:|---:|\n");
    std::string json = "{\"benchmark\":\"dynamic_timeline\",\"n\":" + std::to_string(n) + ",\"frames\":" + std::to_string(frames) + ",\"rows\":[";
    bool first = true;
    for (std::size_t budget : budgets)
    {
        Trial tr[3];
        for (auto& t : tr) t = run(n, frames, budget);
        auto med = [&](auto get) {
            double v[3] = { get(tr[0]), get(tr[1]), get(tr[2]) };
            std::sort(v, v + 3);
            return v[1];
        };
        const double deg = med([](const Trial& t) { return t.degradedUs; });
        const double full = med([](const Trial& t) { return t.fullUs; });
        const double worst = med([](const Trial& t) { return t.worstFrameUs; });
        const double mig = med([](const Trial& t) { return t.migrationTotalUs; }) / 1000.0;
        const int flip = static_cast<int>(med([](const Trial& t) { return static_cast<double>(t.flipFrame); }));
        const std::string name = budget == kStall ? "stall (all at once)" : budget == 0 ? "never (degraded only)" : std::to_string(budget);
        std::printf("| %s | %s | %s | %.0f | %s | %.1f |\n", name.c_str(), deg > 0 ? std::to_string(static_cast<long long>(deg)).c_str() : "—",
                    full > 0 ? std::to_string(static_cast<long long>(full)).c_str() : "—", worst,
                    flip >= 0 ? std::to_string(flip).c_str() : "never", mig);
        json += std::string(first ? "" : ",") + "{\"budget\":\"" + name + "\",\"degraded_us\":" + std::to_string(deg) +
                ",\"full_us\":" + std::to_string(full) + ",\"worst_frame_us\":" + std::to_string(worst) +
                ",\"flip_frame\":" + std::to_string(flip) + ",\"migration_ms\":" + std::to_string(mig) + "}";
        first = false;
    }
    json += "]}";
    if (out)
    {
        if (FILE* f = std::fopen(out, "w")) { std::fputs(json.c_str(), f); std::fclose(f); }
    }
    return 0;
}
