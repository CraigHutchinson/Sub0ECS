#pragma once
/** Component bitmasks: one bit per component type of a World (see type_index.hpp). */

#include <cstdint>

namespace sub0ecs::store
{
    using Mask = std::uint64_t;

    /** Component types per World type: one bit each in a 64-bit Mask. */
    inline constexpr std::uint32_t kMaxTypes = 64;

    /** Partition::columnOf value for "not a column in this partition". */
    inline constexpr std::int8_t kNoColumn = -1;

} // namespace sub0ecs::store
