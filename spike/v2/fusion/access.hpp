#pragma once
/** Fusion extension points — part 0: what a system declares.
 *
 *   struct Integrate {
 *       using Query  = spike::Query<Position, Velocity>;                     // membership
 *       using Access = fusion::Access<fusion::Write<Position>,
 *                                     fusion::Read<Velocity>>;               // optional, default: all Write
 *       static constexpr bool kDeviceSafe = true;                           // optional capability
 *       void operator()(Position&, Velocity&) const;
 *   };
 *
 * Planners use Access to decide what may/should fuse; executors use it to
 * move only the data a group needs (copy back only what it writes); the
 * capability flags gate which executors (devices) may run a system.
 */

#include <type_traits>

#include "../common/query.hpp"

namespace spike::fusion
{
    template <typename T> struct Read {};
    template <typename T> struct Write {};
    template <typename... As> struct Access {};

    namespace detail
    {
        template <typename Q> struct AllWrite;
        template <typename... Cs> struct AllWrite<Query<Cs...>> { using type = Access<Write<Cs>...>; };

        template <typename A, typename T> struct WritesT;
        template <typename... As, typename T>
        struct WritesT<Access<As...>, T> : std::bool_constant<(std::is_same_v<As, Write<T>> || ...)> {};

        template <typename T, typename... Cs>
        constexpr bool contains(Query<Cs...>) { return (std::is_same_v<T, Cs> || ...); }
    } // namespace detail

    /** Declared access of S; systems without an Access declaration are treated as writing everything. */
    template <typename S>
    struct AccessOf { using type = typename detail::AllWrite<typename S::Query>::type; };
    template <typename S>
        requires requires { typename S::Access; }
    struct AccessOf<S> { using type = typename S::Access; };

    template <typename S, typename T>
    inline constexpr bool kUses = detail::contains<T>(typename S::Query{});

    template <typename S, typename T>
    inline constexpr bool kWrites = kUses<S, T> && detail::WritesT<typename AccessOf<S>::type, T>::value;

    namespace detail
    {
        template <typename A, typename... Cs>
        constexpr bool sharesAny(Query<Cs...>) { return (kUses<A, Cs> || ...); }
    }

    /** A and B touch at least one common component (the fusion benefit: shared loads/stores). */
    template <typename A, typename B>
    inline constexpr bool kShares = detail::sharesAny<A>(typename B::Query{});

    /** Capability: the system holds no host pointers / host-only state, so a device may run it. */
    template <typename S>
    inline constexpr bool kDeviceSafe = requires { requires S::kDeviceSafe; };

} // namespace spike::fusion
