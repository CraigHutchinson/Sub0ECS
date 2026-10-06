#pragma once
/** Volatile<Cs...>: planning hint for components that are added and removed
 *  often. In carry mode they live in side storage, so churning them never moves
 *  a row between partitions. */

namespace sub0ecs::store
{
    /** Lists the components a World should keep in side storage.
     *  @tparam Cs The churn-heavy component types. A type that is also queried is
     *             a column regardless. */
    template <typename... Cs> struct Volatile {};

} // namespace sub0ecs::store
