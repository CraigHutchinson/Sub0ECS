#pragma once
/** Column: one type-erased dense component column of a partition. */

#include <cstddef>
#include <cstdint>

namespace sub0ecs::store
{
    /** Type-erased dense column (one per hot component in a partition).
     *  Storage is owned by the Partition (raw, 64-byte aligned, grown
     *  geometrically, never zero-filled): pushRow/swapRemove touch no bytes
     *  beyond the row itself. */
    struct Column
    {
        std::uint32_t type = 0;
        std::size_t stride = 0;
        std::byte* data = nullptr;

        std::byte* at(std::size_t row) const { return data + row * stride; }
    };

} // namespace sub0ecs::store
