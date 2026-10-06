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
    /** A handle to an entity: a slot index and the slot's version when the handle was made. */
    struct Entity
    {
        static constexpr std::uint32_t kIndexBits = 24;
        static constexpr std::uint32_t kIndexMask = (1u << kIndexBits) - 1u;
        /** Slots available; index kIndexMask is kept for the null handle. */
        static constexpr std::uint32_t kMaxEntities = kIndexMask;

        std::uint32_t value = ~0u;   ///< Index and version packed; all ones is the null handle.

        /** Extracts the slot index.
         *  @return The low kIndexBits bits. */
        [[nodiscard]] constexpr std::uint32_t index() const { return value & kIndexMask; }

        /** Extracts the version.
         *  @return The slot's version when this handle was made (8 bits). */
        [[nodiscard]] constexpr std::uint32_t version() const { return value >> kIndexBits; }

        constexpr bool operator==(const Entity&) const = default;

        /** Packs an index and a version into a handle.
         *  @param index   The slot index; only its low kIndexBits bits are kept.
         *  @param version The slot's version; only its low 8 bits are kept.
         *  @return The handle. */
        [[nodiscard]] static constexpr Entity make(std::uint32_t index, std::uint32_t version)
        {
            return Entity{ (index & kIndexMask) | (version << kIndexBits) };
        }
    };

    /** The handle that refers to no entity. */
    inline constexpr Entity kNullEntity{};

    /** Hands out entity handles and recycles the slots of released ones.
     *  @note Not thread-safe. */
    class EntityAllocator
    {
    public:
        /** Reserves the slot table.
         *  @param n The number of slots expected. */
        void reserve(std::size_t n) { versions_.reserve(n); }

        /** Allocates a handle, reusing the most recently released slot if there is one.
         *  @return The handle, or kNullEntity when all Entity::kMaxEntities slots are live. */
        [[nodiscard]] Entity create()
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

        /** Releases a live handle's slot for reuse and makes the handle stale.
         *  @param e A handle for which alive(e) is true. */
        void release(Entity e)
        {
            versions_[e.index()] = (versions_[e.index()] + 1u) & 0xFFu;
            free_.push_back(e.index());
        }

        /** Tells whether a handle is current.
         *  @param e The handle.
         *  @return true when e's slot exists and still has e's version. */
        [[nodiscard]] bool alive(Entity e) const
        {
            return e.index() < versions_.size() && versions_[e.index()] == e.version();
        }

        /** Counts the slots ever allocated.
         *  @return Live and free slots together. */
        [[nodiscard]] std::size_t slots() const { return versions_.size(); }

        /** Counts the live handles.
         *  @return Slots minus the free ones. */
        [[nodiscard]] std::size_t liveCount() const { return versions_.size() - free_.size(); }

    private:
        std::vector<std::uint32_t> versions_;
        std::vector<std::uint32_t> free_;
    };

} // namespace sub0ecs
