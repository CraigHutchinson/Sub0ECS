/** Executor contract, checked directly: every row exactly once whatever the
 * split, only declared-written columns come back from a device, and the
 * AutoTuner settles on one of its candidates. */
#include <algorithm>
#include <atomic>
#include <cstddef>
#include <string_view>
#include <tuple>
#include <vector>

#include <sub0ecs/fusion/executors.hpp>
#include <sub0ecs/fusion/planner.hpp>

#include "../bench/common/components.hpp"
#include <doctest/doctest.h>

namespace fz = sub0ecs::fusion;
using bench::Position;
using bench::Velocity;

namespace
{
    struct Integrate   // writes Position, reads Velocity
    {
        using Query = sub0ecs::Query<Position, Velocity>;
        using Access = fz::Access<fz::Write<Position>, fz::Read<Velocity>>;
        static constexpr bool kDeviceSafe = true;
        void operator()(Position& p, Velocity& v) const { p.x += v.dx; }
    };
    using Info = fz::GroupInfo<Integrate>;
    using Cols = std::tuple<Position*, Velocity*>;

    struct Rows
    {
        std::vector<Position> p;
        std::vector<Velocity> v;
        explicit Rows(std::size_t n) : p(n), v(n)
        {
            for (std::size_t i = 0; i < n; ++i) v[i].dx = 1.0f;
        }
        Cols cols() { return { p.data(), v.data() }; }
        /** Every row visited exactly once <=> every Position.x == 1. */
        bool exactlyOnce() const
        {
            for (const Position& q : p)
                if (q.x != 1.0f) return false;
            return true;
        }
    };

    auto integrateKernel()
    {
        return [](const Cols& c, std::size_t count) {
            for (std::size_t i = 0; i < count; ++i) Integrate{}(std::get<Position*>(c)[i], std::get<Velocity*>(c)[i]);
        };
    }
} // namespace

TEST_CASE("executors: GroupInfo derives written columns from Access")
{
    static_assert(Info::kWritten<Position> && !Info::kWritten<Velocity>);
    static_assert(Info::kDeviceSafe);
    static_assert(fz::kWrites<Integrate, Position> && !fz::kWrites<Integrate, Velocity>);
}

TEST_CASE("executors: offsetCols shifts every pointer and keeps missing columns null")
{
    Position p[4];
    const std::tuple<Position*, Velocity*> c{ p, nullptr };
    const auto shifted = fz::offsetCols(c, 3);
    CHECK(std::get<Position*>(shifted) == p + 3);
    CHECK(std::get<Velocity*>(shifted) == nullptr);
}

TEST_CASE("executors: every executor processes each row exactly once")
{
    for (std::size_t n : { std::size_t{ 0 }, std::size_t{ 1 }, std::size_t{ 999 }, std::size_t{ 1000 }, std::size_t{ 1001 },
                           std::size_t{ 4500 }, std::size_t{ 40'000 } })
    {
        CAPTURE(n);
        auto kernel = integrateKernel();
        {
            Rows r(n);
            fz::Inline exec;
            exec.run<Info>(n, r.cols(), kernel);
            CHECK(r.exactlyOnce());
        }
        {
            Rows r(n);
            fz::Tiled<1000> exec;   // n around tile multiples: partial last tile
            exec.run<Info>(n, r.cols(), kernel);
            CHECK(r.exactlyOnce());
        }
        {
            Rows r(n);
            fz::Parallel exec(4);   // small n runs inline, large n splits into chunks
            exec.run<Info>(n, r.cols(), kernel);
            CHECK(r.exactlyOnce());
        }
        {
            Rows r(n);
            fz::Offload<fz::EmulatedDevice, 1024> exec;
            exec.run<Info>(n, r.cols(), kernel);
            CHECK(r.exactlyOnce());
            CHECK(exec.bytesIn() == n * (sizeof(Position) + sizeof(Velocity)));
            CHECK(exec.bytesOut() == n * sizeof(Position));
        }
    }
}

TEST_CASE("executors: InlineRange runs exactly its sub-range")
{
    Rows r(100);
    auto kernel = integrateKernel();
    fz::InlineRange exec{ 10, 30 };
    exec.run<Info>(100, r.cols(), kernel);
    for (std::size_t i = 0; i < 100; ++i) CHECK(r.p[i].x == ((i >= 10 && i < 30) ? 1.0f : 0.0f));
}

TEST_CASE("executors: Offload discards device writes to columns declared read-only")
{
    Rows r(3000);
    // A kernel that (illegally) also writes Velocity: Access says Velocity is
    // read-only, so the device copy of it must never reach the store.
    auto kernel = [](const Cols& c, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i)
        {
            std::get<Position*>(c)[i].x += 1.0f;
            std::get<Velocity*>(c)[i].dx = 99.0f;
        }
    };
    fz::Offload<fz::EmulatedDevice, 1024> exec;
    exec.run<Info>(3000, r.cols(), kernel);
    CHECK(r.exactlyOnce());
    for (const Velocity& v : r.v) CHECK(v.dx == 1.0f);
}

TEST_CASE("executors: Parallel::parallelFor claims every item exactly once")
{
    for (bool affinity : { false, true })
    {
        CAPTURE(affinity);
        fz::Parallel pool(4, affinity);
        CHECK(pool.concurrency() == 4);
        for (std::size_t items : { std::size_t{ 0 }, std::size_t{ 1 }, std::size_t{ 3 }, std::size_t{ 17 }, std::size_t{ 10'000 } })
        {
            CAPTURE(items);
            std::vector<std::atomic<int>> hits(items);
            std::atomic<bool> badWorker{ false };
            auto body = [&](std::size_t i, unsigned worker) {
                hits[i].fetch_add(1, std::memory_order_relaxed);
                if (worker >= 4) badWorker = true;
            };
            pool.parallelFor(items, body);
            bool once = true;
            for (auto& h : hits) once = once && h.load() == 1;
            CHECK(once);
            CHECK_FALSE(badWorker.load());
        }
    }
}

TEST_CASE("executors: a pool reused across many back-to-back dispatches loses no work")
{
    fz::Parallel pool(4);
    std::atomic<std::size_t> total{ 0 };
    auto body = [&](std::size_t, unsigned) { total.fetch_add(1, std::memory_order_relaxed); };
    for (int i = 0; i < 2000; ++i) pool.parallelFor(8, body);   // exercises spin-then-park wake-ups
    CHECK(total.load() == 2000u * 8u);
}

namespace
{
    /** Minimal world for the AutoTuner: counts runPlanned dispatches per group. */
    struct CountingWorld
    {
        int groups = 0;
        template <typename Exec, typename... Systems>
        void runFusedOn(Exec&, const Systems&...) { ++groups; }
    };
    struct A { using Query = sub0ecs::Query<Position>; };
    struct B { using Query = sub0ecs::Query<Position, Velocity>; };
} // namespace

TEST_CASE("planners: a plan is a compile-time value")
{
    static_assert(fz::Plan<fz::NeverFuse, A, B>::kGroups == 2);
    static_assert(fz::Plan<fz::AlwaysFuse, A, B>::kGroups == 1);
    static_assert(fz::Plan<fz::ShareColumns, A, B>::kGroups == 1);   // share Position
    static_assert(fz::Plan<fz::DeviceAware<fz::AlwaysFuse>, A, Integrate>::kGroups == 2,
                  "DeviceAware splits where device-safety changes");
}

TEST_CASE("planners: runPlanned dispatches one fused run per group")
{
    CountingWorld w;
    fz::Inline exec;
    fz::runPlanned<fz::NeverFuse>(w, exec, exec, A{}, B{});
    CHECK(w.groups == 2);
    w.groups = 0;
    fz::runPlanned<fz::AlwaysFuse>(w, exec, exec, A{}, B{});
    CHECK(w.groups == 1);
}

TEST_CASE("planners: AutoTuner trials every candidate, then keeps one")
{
    using Tuner = fz::AutoTuner<fz::NeverFuse, fz::AlwaysFuse>;
    Tuner tuner;
    CountingWorld w;
    fz::Inline exec;
    constexpr int kTrials = static_cast<int>(Tuner::kCandidates) * Tuner::kTrialsPerCandidate;
    for (int i = 0; i < kTrials; ++i)
    {
        CHECK_FALSE(tuner.decided());
        tuner.run(w, exec, exec, A{}, B{});
    }
    CHECK(tuner.decided());
    CHECK(tuner.chosen() < Tuner::kCandidates);
    CHECK(Tuner::names()[0] == std::string_view("NeverFuse"));

    const int before = w.groups;
    tuner.run(w, exec, exec, A{}, B{});
    CHECK(w.groups - before == (tuner.chosen() == 0 ? 2 : 1));   // runs the chosen plan from now on
}

TEST_CASE("executors: Parallel creates threads only as work needs them")
{
    fz::Parallel pool(8);
    CHECK(pool.concurrency() == 8);
    CHECK(pool.threadsStarted() == 0);   // nothing until a dispatch asks

    std::atomic<int> ran{ 0 };
    std::atomic<unsigned> widest{ 0 };
    auto body = [&](std::size_t, unsigned worker) {
        ran.fetch_add(1, std::memory_order_relaxed);
        for (unsigned seen = widest.load(); worker > seen && !widest.compare_exchange_weak(seen, worker);) {}
    };

    pool.parallelFor(1, body);           // one item runs on the caller
    CHECK(pool.threadsStarted() == 0);

    pool.parallelFor(4, body);           // four ways parallel: the caller plus three
    CHECK(pool.threadsStarted() == 3);
    CHECK(widest.load() <= 3);

    pool.parallelFor(2, body);           // narrower work starts nothing new
    CHECK(pool.threadsStarted() == 3);

    pool.parallelFor(100, body);         // wide work grows to the limit, not beyond
    CHECK(pool.threadsStarted() == 7);
    CHECK(widest.load() <= 7);
    CHECK(ran.load() == 1 + 4 + 2 + 100);
}

TEST_CASE("executors: a narrow dispatch on a grown pool uses only the workers it needs")
{
    for (bool affinity : { false, true })
    {
        CAPTURE(affinity);
        fz::Parallel pool(8, affinity);
        std::atomic<int> warm{ 0 };
        auto warmUp = [&](std::size_t, unsigned) { warm.fetch_add(1, std::memory_order_relaxed); };
        pool.parallelFor(64, warmUp);    // all seven workers exist now
        CHECK(pool.threadsStarted() == 7);

        for (int round = 0; round < 200; ++round)
        {
            std::atomic<int> hits[3] = {};
            std::atomic<bool> outsider{ false };
            auto body = [&](std::size_t i, unsigned worker) {
                hits[i].fetch_add(1, std::memory_order_relaxed);
                if (worker >= 3) outsider = true;   // only the caller and two workers may take part
            };
            pool.parallelFor(3, body);
            CHECK(hits[0].load() == 1);
            CHECK(hits[1].load() == 1);
            CHECK(hits[2].load() == 1);
            CHECK_FALSE(outsider.load());
        }
    }
}

TEST_CASE("executors: the default Parallel is sized by the CPU topology")
{
    const fz::CpuTopology& topology = fz::cpuTopology();
    CHECK(!topology.cpus.empty());
    CHECK(topology.performance >= 1);
    CHECK(topology.performance <= topology.cpus.size());
    std::vector<unsigned> sorted = topology.cpus;
    std::sort(sorted.begin(), sorted.end());
    CHECK(std::adjacent_find(sorted.begin(), sorted.end()) == sorted.end());   // no CPU listed twice
    CHECK(topology.hybrid() == (topology.performance < topology.cpus.size()));

    fz::Parallel pool;   // one thread per performance core the process may use
    CHECK(pool.concurrency() == topology.performance);
    CHECK(pool.threadsStarted() == 0);
}

TEST_CASE("executors: pinning workers to the performance cores is an option that changes no result")
{
    // Whether a thread can be restricted depends on the machine; the work must not.
    fz::Parallel pool(fz::Parallel::Options{ .threads = 4, .ownerComputes = false, .pinToPerformanceCores = true });
    CHECK(pool.concurrency() == 4);
    std::vector<std::atomic<int>> hits(5000);
    auto body = [&](std::size_t i, unsigned) { hits[i].fetch_add(1, std::memory_order_relaxed); };
    for (int round = 0; round < 3; ++round) pool.parallelFor(hits.size(), body);
    bool thrice = true;
    for (auto& h : hits) thrice = thrice && h.load() == 3;
    CHECK(thrice);
    CHECK(pool.threadsStarted() == 3);
}
