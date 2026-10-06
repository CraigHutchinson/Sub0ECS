#pragma once
/** Fusion extension points — part 0: what a system declares.
 *
 *   struct Integrate {
 *       using Query  = sub0ecs::Query<Position, Velocity>;                     // membership
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

#include "sub0ecs/query.hpp"

namespace sub0ecs::fusion
{
    /** Declares that a system only reads a component.
     *  @tparam T The component type. */
    template <typename T> struct Read {};

    /** Declares that a system writes a component.
     *  @tparam T The component type. */
    template <typename T> struct Write {};

    /** A system's declared access, as `using Access = Access<Write<A>, Read<B>>`.
     *  @tparam As One Read<T> or Write<T> per component of the system's query.
     *  @note The declaration is trusted, not checked: a system that writes a component
     *        it declared Read loses that write on an executor that copies data back. */
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

    /** Yields a system's declared access; a system without an Access declaration is
     *  treated as writing everything.
     *  @tparam S The system type. */
    template <typename S>
    struct AccessOf { using type = typename detail::AllWrite<typename S::Query>::type; };
    template <typename S>
        requires requires { typename S::Access; }
    struct AccessOf<S> { using type = typename S::Access; };

    /** True when a system's query names a component.
     *  @tparam S The system type.
     *  @tparam T The component type. */
    template <typename S, typename T>
    inline constexpr bool kUses = detail::contains<T>(typename S::Query{});

    /** True when a system uses a component and declares that it writes it.
     *  @tparam S The system type.
     *  @tparam T The component type. */
    template <typename S, typename T>
    inline constexpr bool kWrites = kUses<S, T> && detail::WritesT<typename AccessOf<S>::type, T>::value;

    namespace detail
    {
        template <typename A, typename... Cs>
        constexpr bool sharesAny(Query<Cs...>) { return (kUses<A, Cs> || ...); }
    }

    /** True when two systems touch at least one common component (the fusion benefit:
     *  shared loads and stores).
     *  @tparam A A system type.
     *  @tparam B Another system type. */
    template <typename A, typename B>
    inline constexpr bool kShares = detail::sharesAny<A>(typename B::Query{});

    /** True when a system declares `static constexpr bool kDeviceSafe = true`: it holds
     *  no host pointers or host-only state, so a device may run it.
     *  @tparam S The system type. */
    template <typename S>
    inline constexpr bool kDeviceSafe = requires { requires S::kDeviceSafe; };

} // namespace sub0ecs::fusion
