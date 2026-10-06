#pragma once
/** Column: one type-erased dense component column of a partition. */

#include <cstddef>
#include <cstdint>
#include <cstring>

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

    /** Copy one row of a column (stride bytes; the rows do not overlap).
     *  A stride is only known at run time, so memcpy alone is a library call per
     *  column per moved row. The common component sizes get a fixed-size copy,
     *  which every compiler turns into one or two moves. */
    inline void copyRow(std::byte* dst, const std::byte* src, std::size_t stride)
    {
        switch (stride)
        {
        case 4: std::memcpy(dst, src, 4); return;
        case 8: std::memcpy(dst, src, 8); return;
        case 12: std::memcpy(dst, src, 12); return;
        case 16: std::memcpy(dst, src, 16); return;
        default: std::memcpy(dst, src, stride); return;
        }
    }

} // namespace sub0ecs::store
