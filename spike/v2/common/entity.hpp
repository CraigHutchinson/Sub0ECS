#pragma once
/** Generational entity handle + allocator shared by the v2 candidate designs.
 *
 * 32-bit handle: low 24 bits = slot index (16M live entities), high 8 bits =
 * version. A destroyed slot bumps its version so stale handles are detected.
 * v1 has no equivalent: ids are monotonic and never recycled.
 */

#include <cstdint>
#include <vector>

namespace spike
{
    struct Entity
    {
        static constexpr std::uint32_t kIndexBits = 24;
        static constexpr std::uint32_t kIndexMask = (1u << kIndexBits) - 1u;

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

        Entity create()
        {
            if (!free_.empty())
            {
                const std::uint32_t index = free_.back();
                free_.pop_back();
                return Entity::make(index, versions_[index]);
            }
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

    /** Process-wide dense component type id (runtime registries only). */
    inline std::uint32_t nextTypeId()
    {
        static std::uint32_t next = 0;
        return next++;
    }

    template <typename T>
    std::uint32_t typeId()
    {
        static const std::uint32_t id = nextTypeId();
        return id;
    }

} // namespace spike
