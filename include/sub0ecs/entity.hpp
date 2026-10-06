#pragma once
/** Generational entity handle and allocator.
 *
 * 32-bit handle: low 24 bits = slot index, high 8 bits = version. A destroyed
 * slot bumps its version, so a handle kept past its entity's destruction is
 * detected as stale.
 *
 * Limits:
 *   - at most Entity::kMaxEntities (16,777,215) slots; the allocator returns
 *     kNullEntity beyond that instead of reusing a live index;
 *   - the version is 8 bits: a stale handle compares alive again once its slot
 *     has been reused 256 times. Slots are reused most-recent-first, so a tight
 *     spawn/destroy loop reaches that quickly. Do not keep a handle across
 *     unbounded churn without re-validating it another way.
 */

#include <cstdint>
#include <vector>

namespace sub0ecs
{
    struct Entity
    {
        static constexpr std::uint32_t kIndexBits = 24;
        static constexpr std::uint32_t kIndexMask = (1u << kIndexBits) - 1u;
        /** Slots available; index kIndexMask is kept for the null handle. */
        static constexpr std::uint32_t kMaxEntities = kIndexMask;

        std::uint32_t value = ~0u;

        constexpr std::uint32_t index() const { return value & kIndexMask; }
        constexpr std::uint32_t version() const { return value >> kIndexBits; }
        constexpr bool operator==(const Entity&) const = default;

        static constexpr Entity make(std::uint32_t index, std::uint32_t version)
        {
            return Entity{ (index & kIndexMask) | (version << kIndexBits) };
        }
    };

    inline constexpr Entity kNullEntity{};

    class EntityAllocator
    {
    public:
        void reserve(std::size_t n) { versions_.reserve(n); }

        /** A new handle, or kNullEntity when all Entity::kMaxEntities slots are live. */
        Entity create()
        {
            if (!free_.empty())
            {
                const std::uint32_t index = free_.back();
                free_.pop_back();
                return Entity::make(index, versions_[index]);
            }
            if (versions_.size() >= Entity::kMaxEntities) return kNullEntity;
            const auto index = static_cast<std::uint32_t>(versions_.size());
            versions_.push_back(0);
            return Entity::make(index, 0);
        }

        void release(Entity e)
        {
            versions_[e.index()] = (versions_[e.index()] + 1u) & 0xFFu;
            free_.push_back(e.index());
        }

        bool alive(Entity e) const
        {
            return e.index() < versions_.size() && versions_[e.index()] == e.version();
        }

        std::size_t slots() const { return versions_.size(); }
        std::size_t liveCount() const { return versions_.size() - free_.size(); }

    private:
        std::vector<std::uint32_t> versions_;
        std::vector<std::uint32_t> free_;
    };

} // namespace sub0ecs
