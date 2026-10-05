#pragma once
/** TypeIndices<Owner>: runtime component numbering per owner (a World type).
 *
 * Numbers the component types an owner meets, densely from 0 and without limit, on
 * first use. One function-local static per (Owner, T): a lookup costs one load.
 * The World puts its compile-time-known types (queried and Volatile) in front of
 * these; see "Type indices" in world.hpp. */

#include <atomic>
#include <cstdint>

namespace sub0ecs::store
{
    template <typename Owner>
    struct TypeIndices
    {
        /** Index of T among this owner's runtime-numbered types, assigned on first use. */
        template <typename T>
        static std::uint32_t of()
        {
            static const std::uint32_t index = next();
            return index;
        }

    private:
        static std::uint32_t next()
        {
            static std::atomic<std::uint32_t> counter{ 0 };   // first uses may race across threads
            return counter.fetch_add(1, std::memory_order_relaxed);
        }
    };

} // namespace sub0ecs::store
