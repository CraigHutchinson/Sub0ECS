#pragma once
/** DeviceAware<Base>: refines any planner so a group is entirely device-safe
 *  (offloadable) or entirely host-only. */

#include <array>
#include <cstddef>

#include "sub0ecs/fusion/access.hpp"

namespace sub0ecs::fusion
{
    /** Capability-aware refinement: split wherever device-safety changes, so a
     *  group is either entirely device-safe (offloadable) or entirely host-only. */
    template <typename Base>
    struct DeviceAware
    {
        static constexpr const char* kName = "DeviceAware";
        template <typename... S>
        static constexpr auto plan()
        {
            auto b = Base::template plan<S...>();
            constexpr std::array<bool, sizeof...(S)> safe{ kDeviceSafe<S>... };
            for (std::size_t i = 1; i < sizeof...(S); ++i)
                if (safe[i] != safe[i - 1]) b[i] = true;
            return b;
        }
    };

} // namespace sub0ecs::fusion
