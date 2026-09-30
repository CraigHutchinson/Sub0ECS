#pragma once
/** Executor contract shared by every executor: what the store hands over
 *  (see ../executors.hpp for the full contract) and the helpers to split rows. */

#include <cstddef>
#include <tuple>

#include "../access.hpp"

namespace sub0ecs::fusion
{
    template <typename... Ts>
    std::tuple<Ts*...> offsetCols(const std::tuple<Ts*...>& c, std::size_t b)
    {
        return { (std::get<Ts*>(c) ? std::get<Ts*>(c) + b : nullptr)... };
    }

    /** Compile-time facts about a fused group, handed to executors. */
    template <typename... Systems>
    struct GroupInfo
    {
        template <typename T>
        static constexpr bool kWritten = (kWrites<Systems, T> || ...);
        static constexpr bool kDeviceSafe = (fusion::kDeviceSafe<Systems> && ...);
    };

} // namespace sub0ecs::fusion
