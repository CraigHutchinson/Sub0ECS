#pragma once
#include <mutex>
#include "stack/model.hpp"
#include "sub0pub/broker.hpp"
#include "sub0pub/wiring.hpp"
namespace bench::stack
{
struct BrokerLock
{
    std::mutex mutex;
    void lock() { mutex.lock(); }
    void unlock() noexcept { mutex.unlock(); }
};
/** Mirrors Crucible's synchronous scoped, locked, one-endpoint delivery contract. */
struct Batch
{
    using sub0_config = sub0::config<sub0::Scoped, sub0::Capacity<1>, sub0::NoFilter,
                                   sub0::LockWith<BrokerLock>>;
    std::uint64_t request{};
    std::span<const Command> commands;
};
struct Receipt { std::uint64_t request{}; bool accepted{}; };
struct Receiver
{
    Ingress& ingress;
    Receipt receipt;
    void receive(const Batch& batch) noexcept { receipt = {batch.request, ingress.admit(batch.commands)}; }
};
class DomainRoute
{
    struct Sink final : sub0::Subscribe<Batch>
    {
        Receiver receiver;
        Sink(sub0::Domain<Batch>& domain, Ingress& ingress)
            : sub0::Subscribe<Batch>(domain), receiver{ingress, {}}
        {
            if (this->trySubscribe() != sub0::SubscribeResult::Subscribed)
                throw std::runtime_error("stack subscription failed");
        }
        ~Sink() { this->unsubscribe(); }
        void receive(const Batch& batch) noexcept override { receiver.receive(batch); }
    };
    struct Source final : sub0::Publish<Batch>
    {
        explicit Source(sub0::Domain<Batch>& domain) : sub0::Publish<Batch>(domain) {}
        void send(const Batch& batch) { sub0::publish(*this, batch); }
    };
public:
    explicit DomainRoute(Ingress& ingress) : sink_(domain_, ingress), source_(domain_) {}
    Receipt send(const Batch& batch)
    {
        source_.send(batch);
        return sink_.receiver.receipt;
    }
private:
    sub0::Domain<Batch> domain_;
    Sink sink_;
    Source source_;
};
/** Fixed composition only: does not replace Domain's dynamic/concurrent lifetime features. */
class WiredRoute
{
public:
    explicit WiredRoute(Ingress& ingress) : receiver_{ingress, {}} {}
    Receipt send(const Batch& batch)
    {
        sub0::wire(receiver_).publish(batch);
        return receiver_.receipt;
    }
private:
    Receiver receiver_;
};
}
