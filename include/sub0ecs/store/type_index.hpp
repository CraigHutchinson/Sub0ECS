#pragma once
/** TypeIndices<Owner>: dense component numbering per owner (a World type).
 *
 * Each owner numbers the component types it meets from 0, on first use, so the
 * 64-type limit is per World type rather than per process. One function-local
 * static per (Owner, T): a lookup costs one load, like a global type id. */

#include <atomic>
#include <cstdint>
#include <cstdlib>

#include "mask.hpp"

namespace sub0ecs::store
{
    template <typename Owner>
    struct TypeIndices
    {
        /** Index of T for this owner, assigned on first use; > 64 types terminates (64-bit masks). */
        template <typename T>
        static std::uint32_t of()
        {
            static const std::uint32_t index = [] {
                const std::uint32_t i = next();
                if (i >= kMaxTypes) std::abort();
                return i;
            }();
            return index;
        }

        template <typename T>
        static Mask bit() { return Mask{ 1 } << of<T>(); }

    private:
        static std::uint32_t next()
        {
            static std::atomic<std::uint32_t> counter{ 0 };   // first uses may race across threads
            return counter.fetch_add(1, std::memory_order_relaxed);
        }
    };

} // namespace sub0ecs::store
