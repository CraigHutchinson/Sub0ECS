#pragma once
#include <algorithm>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>
namespace bench::stack
{
/** Synthetic fixed-identity mobile state; unsigned arithmetic is exactly reproducible. */
struct Row
{
    std::uint64_t id{}, position{}, velocity{};
    bool operator==(const Row&) const = default;
};
struct Command { std::uint64_t impulse{}; };
/** Coordinator-owned admission copies borrowed input before the publishing call returns. */
class Ingress
{
public:
    explicit Ingress(std::size_t capacity) : storage_(capacity) {}
    bool admit(std::span<const Command> commands) noexcept
    {
        if (commands.size() > storage_.size() - size_) return false;
        std::copy(commands.begin(), commands.end(), storage_.begin() + size_);
        size_ += commands.size();
        return true;
    }
    std::uint64_t drain() noexcept
    {
        std::uint64_t impulse{};
        for (std::size_t i = 0; i < size_; ++i) impulse += storage_[i].impulse;
        size_ = 0;
        return impulse;
    }
private:
    std::vector<Command> storage_;
    std::size_t size_{};
};
inline Row initial(std::size_t id) { return {id, id * 17u, id % 7u}; }
inline Row propose(Row row, std::uint64_t impulse)
{
    row.velocity += impulse;
    row.position += row.velocity;
    return row;
}
/** Independent contiguous end-to-end floor, including owned command copy and publication. */
class Plain
{
public:
    explicit Plain(std::size_t n) : ingress_(8), rows_(n), published_(n)
    {
        for (std::size_t i = 0; i < n; ++i) rows_[i] = initial(i);
        published_ = rows_;
    }
    bool admit(std::span<const Command> commands) { return ingress_.admit(commands); }
    void tick()
    {
        const auto impulse = ingress_.drain();
        for (auto& row : rows_) row = propose(row, impulse);
        published_ = rows_;
        ++ticks_;
    }
    const auto& state() const { return published_; }
    auto ticks() const { return ticks_; }
private:
    Ingress ingress_;
    std::vector<Row> rows_, published_;
    std::size_t ticks_{};
};
}
