#pragma once
/** Query<Cs...>: the components a system requires. */
namespace sub0ecs
{
    /** Names the components a system requires. A World is declared with the queries
     *  its systems use, and a system names its own as `using Query = ...`.
     *  @tparam Cs The component types, in the order the system receives them. */
    template <typename... Cs>
    struct Query {};
} // namespace sub0ecs
