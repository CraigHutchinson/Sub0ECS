#pragma once
/** DynamicQuery<Cs...>: the handle of a query added to a World at runtime.
 *
 *  It carries the component list in its type, so iterating it cannot name a
 *  different list from the one it was registered with. Returned by
 *  BasicWorld::addQuery and valid for that world only. */

#include <cstddef>

namespace sub0ecs::store
{
    /** Identifies a query added to a World at runtime.
     *  @tparam Cs The query's components, as given to BasicWorld::addQuery. */
    template <typename... Cs>
    struct DynamicQuery
    {
        std::size_t id = ~std::size_t{ 0 };   ///< Index of the registration in its world.
    };

} // namespace sub0ecs::store
