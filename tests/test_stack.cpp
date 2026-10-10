#include <array>
#include <doctest/doctest.h>
#include "stack/simulation.hpp"
namespace
{
template <typename Route, unsigned Layout>
void checkStack()
{
    for (const auto size : {0u, 1u, 17u, 1023u, 1024u, 1025u})
    for (const auto workers : {1u, 2u, 4u})
    {
        bench::stack::Plain reference(size);
        bench::stack::Simulation<Route, Layout> candidate(size, workers);
        for (unsigned tick = 0; tick < 7; ++tick)
        {
            std::array<bench::stack::Command, 8> commands{};
            for (auto& command : commands) command.impulse = tick + 1;
            CHECK(candidate.admit(commands));
            CHECK(reference.admit(commands));
            CHECK_FALSE(candidate.admit({commands.data(), 1}));
            // Admission owns its copy. Mutating the source cannot affect deferred work.
            commands.fill({999});
            CHECK(candidate.ticks() == tick);
            CHECK(candidate.state() == reference.state());
            candidate.tick(); reference.tick();
            CHECK(candidate.state() == reference.state());
            CHECK(candidate.ticks() == reference.ticks());
        }
        candidate.tick(); reference.tick(); // Empty admission boundary remains meaningful.
        CHECK(candidate.state() == reference.state());
    }
}
}
TEST_CASE("full stack routes preserve owned admission staged rows and publication")
{
    checkStack<bench::stack::DomainRoute, false>();
    checkStack<bench::stack::WiredRoute, false>();
    checkStack<bench::stack::DomainRoute, true>();
    checkStack<bench::stack::WiredRoute, true>();
    checkStack<bench::stack::DomainRoute, 2>();
    checkStack<bench::stack::WiredRoute, 2>();
}
TEST_CASE("scoped stack sessions isolate receipts and teardown")
{
    bench::stack::Simulation<bench::stack::DomainRoute, true> first(17, 2);
    const bench::stack::Command command{3};
    {
        bench::stack::Simulation<bench::stack::DomainRoute, true> second(17, 1);
        CHECK(first.admit({&command, 1}));
        second.tick();
        CHECK(first.ticks() == 0);
        CHECK(second.state()[0].velocity == 0);
    }
    first.tick();
    CHECK(first.state()[0].velocity == 3);
    CHECK(first.state()[0].position == 3);
    CHECK(first.admit({&command, 1}));
    first.tick();
    CHECK(first.state()[0].position == 9);
}
