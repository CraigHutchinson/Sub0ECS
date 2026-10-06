#pragma once
/** Executor contract shared by every executor: what the store hands over
 *  (see ../executors.hpp for the full contract) and the helpers to split rows. */

#include <cstddef>
#include <tuple>

#include "sub0ecs/fusion/access.hpp"

namespace sub0ecs::fusion
{
    /** Advances every column pointer of a group by a number of rows.
     *  @param c The column pointers; a null one stays null.
     *  @param b The number of rows to skip.
     *  @return The column pointers of the rows from b on. */
    template <typename... Ts>
    [[nodiscard]] std::tuple<Ts*...> offsetCols(const std::tuple<Ts*...>& c, std::size_t b)
    {
        return { (std::get<Ts*>(c) ? std::get<Ts*>(c) + b : nullptr)... };
    }

    /** Compile-time facts about a fused group, handed to executors.
     *  @tparam Systems The systems fused into the group. */
    template <typename... Systems>
    struct GroupInfo
    {
        /** True when some system of the group writes T. */
        template <typename T>
        static constexpr bool kWritten = (kWrites<Systems, T> || ...);
        /** True when every system of the group may run on a device. */
        static constexpr bool kDeviceSafe = (fusion::kDeviceSafe<Systems> && ...);
    };

} // namespace sub0ecs::fusion
