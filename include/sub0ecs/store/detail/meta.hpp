#pragma once
/** Compile-time helpers for the store: type-list union and index lookup. */

#include <cstddef>
#include <type_traits>

#include "../../query.hpp"

namespace sub0ecs::store::detail
{
    template <typename T, typename... Ts>
    constexpr std::size_t indexOf()
    {
        std::size_t i = 0;
        const bool found = ((std::is_same_v<T, Ts> ? true : (++i, false)) || ...);
        return found ? i : ~std::size_t{ 0 };
    }

    // Union of all component types used by the fused systems (deduplicated).
    template <typename... Ts> struct TypeList {};
    template <typename L, typename T> struct Append;
    template <typename... Ts, typename T>
    struct Append<TypeList<Ts...>, T>
    {
        using type = std::conditional_t<(std::is_same_v<T, Ts> || ...), TypeList<Ts...>, TypeList<Ts..., T>>;
    };
    template <typename L, typename Q> struct AppendQuery { using type = L; };
    template <typename L, typename C, typename... Cs>
    struct AppendQuery<L, Query<C, Cs...>>
    {
        using type = typename AppendQuery<typename Append<L, C>::type, Query<Cs...>>::type;
    };
    /** Number of types in a TypeList. */
    template <typename L> struct Size;
    template <typename... Ts>
    struct Size<TypeList<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    /** Position of T in a TypeList, or ~0 when it is not in it. */
    template <typename T, typename L> struct IndexIn;
    template <typename T, typename... Ts>
    struct IndexIn<T, TypeList<Ts...>> : std::integral_constant<std::size_t, indexOf<T, Ts...>()> {};

    template <typename L, typename... Qs2> struct UnionOf { using type = L; };
    template <typename L, typename Q, typename... Rest>
    struct UnionOf<L, Q, Rest...>
    {
        using type = typename UnionOf<typename AppendQuery<L, Q>::type, Rest...>::type;
    };

    /** True when no type appears twice in Ts. */
    template <typename... Ts>
    inline constexpr bool kDistinct = Size<typename UnionOf<TypeList<>, Query<Ts...>>::type>::value == sizeof...(Ts);

} // namespace sub0ecs::store::detail
