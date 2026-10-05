#pragma once
/** Component bitmasks: one bit per layout-capable component type of a World
 *  (see "Type indices" in world.hpp). */

#include <cstdint>

namespace sub0ecs::store
{
    using Mask = std::uint64_t;

    /** Layout bits per World type: how many component types can be dense columns.
     *  Types beyond it still work; they are kept in side storage. */
    inline constexpr std::uint32_t kMaxTypes = 64;

    /** Partition::columnOf value for "not a column in this partition". */
    inline constexpr std::int8_t kNoColumn = -1;

} // namespace sub0ecs::store
