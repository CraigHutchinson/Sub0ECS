#pragma once
/** The library vocabulary that benchmark code (namespace bench) uses unqualified:
 *  every comparator design shares the library entity handle and query tag, so the
 *  storage model is the only variable between them. */

#include <cstdint>

#include <sub0ecs/entity.hpp>
#include <sub0ecs/fusion/access.hpp>
#include <sub0ecs/query.hpp>

namespace bench
{
    using sub0ecs::Entity;
    using sub0ecs::EntityAllocator;
    using sub0ecs::kNullEntity;
    using sub0ecs::Query;
    namespace fusion = sub0ecs::fusion;

    /** Process-wide dense component type id for the comparator designs' runtime
     *  registries. (The store numbers types per World type instead.) */
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
} // namespace bench
