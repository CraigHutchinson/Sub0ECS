/** Full-stack synthetic boundary: typed delivery, owned admission, joined proposals,
 * ECS commit and snapshot publication. Compare fixed-resource alternatives with the
 * independent handwritten complete-tick floor; this is not Crucible's spatial solver.
 */
#include <memory>
#include "common/env.hpp"
#include "harness/harness.hpp"
#include "stack/simulation.hpp"
namespace
{
struct DirectRoute
{
    bench::stack::Receiver receiver;
    explicit DirectRoute(bench::stack::Ingress& ingress) : receiver{ingress, {}} {}
    auto send(const bench::stack::Batch& batch) { receiver.receive(batch); return receiver.receipt; }
};
template <typename Route>
bench::harness::Prepared prepareDelivery(std::size_t count)
{
    struct Fixture
    {
        bench::stack::Ingress ingress{8};
        Route route{ingress};
        std::vector<bench::stack::Command> commands;
        std::uint64_t request{}, sum{};
        explicit Fixture(std::size_t n) : commands(n, {1}) {}
    };
    auto fixture = std::make_shared<Fixture>(count);
    bench::harness::Prepared result;
    result.fixture = fixture;
    result.op = [fixture] {
        const auto receipt = fixture->route.send({++fixture->request, fixture->commands});
        if (!receipt.accepted || receipt.request != fixture->request)
            throw std::runtime_error("delivery receipt mismatch");
        fixture->sum += fixture->ingress.drain();
    };
    result.finish = [fixture](bench::harness::Record& record) {
        if (fixture->sum != fixture->request * fixture->commands.size())
            throw std::runtime_error("delivery lost command");
        record.counter("commands_consumed", double(fixture->sum));
    };
    return result;
}
template <typename Simulation>
bench::harness::Prepared prepare(std::shared_ptr<Simulation> simulation)
{
    bench::stack::Plain reference(simulation->state().size());
    const bench::stack::Command command{1};
    for (unsigned i = 0; i < 3; ++i)
    {
        if (!reference.admit({&command, 1}) || !simulation->admit({&command, 1}))
            throw std::runtime_error("stack warmup admission");
        reference.tick(); simulation->tick();
        if (reference.state() != simulation->state()) throw std::runtime_error("stack conformance");
    }
    bench::harness::Prepared result;
    result.fixture = simulation;
    result.op = [simulation] {
        const bench::stack::Command command{1};
        if (!simulation->admit({&command, 1})) throw std::runtime_error("stack admission");
        simulation->tick();
    };
    result.finish = [simulation](bench::harness::Record& record) {
        std::uint64_t checksum{};
        for (const auto& row : simulation->state()) checksum += row.position + row.velocity;
        record.counter("state_checksum", double(checksum));
        record.counter("completed_ticks", double(simulation->ticks()));
    };
    return result;
}
template <typename Route, unsigned Layout>
void add(bench::harness::Registry& registry, const std::string& pattern, const char* name, int n, unsigned workers)
{
    registry.add({"Stack", pattern, name, n, 1, "boundary", false, 1,
        [=] { return prepare(std::make_shared<bench::stack::Simulation<Route, Layout>>(n, workers)); }});
}
}
int main(int argc, char** argv)
{
    bench::harness::Registry registry;
    for (const auto count : {1, 8})
    {
        registry.add({"Stack", "Admission", "HandWritten", count, 1, "batch", false, 0,
            [=] { return prepareDelivery<DirectRoute>(count); }});
        registry.add({"Stack", "Admission", "Domain", count, 1, "batch", false, 0,
            [=] { return prepareDelivery<bench::stack::DomainRoute>(count); }});
        registry.add({"Stack", "Admission", "Wiring", count, 1, "batch", false, 0,
            [=] { return prepareDelivery<bench::stack::WiredRoute>(count); }});
    }
    for (const auto n : bench::env::list("STACK_SIZES", {64, 4096, 65536}))
    for (const auto workers : bench::env::list("STACK_THREADS", {1, 2, 4}))
    {
        if (n < 1 || n > 1000000 || workers < 1 || workers > 64)
            throw std::invalid_argument("STACK_SIZES 1..1000000; STACK_THREADS 1..64");
        const auto pattern = "Boundary" + std::to_string(workers);
        registry.add({"Stack", pattern, "HandWritten", n, 1, "boundary", false, 1,
            [=] { return prepare(std::make_shared<bench::stack::Plain>(n)); }});
        add<bench::stack::DomainRoute, false>(registry, pattern, "DomainSearch", n, workers);
        add<bench::stack::WiredRoute, false>(registry, pattern, "WiringSearch", n, workers);
        add<bench::stack::DomainRoute, true>(registry, pattern, "DomainDense", n, workers);
        add<bench::stack::WiredRoute, true>(registry, pattern, "WiringDense", n, workers);
        add<bench::stack::DomainRoute, 2>(registry, pattern, "DomainIndexed", n, workers);
        add<bench::stack::WiredRoute, 2>(registry, pattern, "WiringIndexed", n, workers);
    }
    return bench::harness::benchMain(argc, argv, registry, "sub0ecs_stack_bench");
}
