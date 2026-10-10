#pragma once
#include <cstddef>
namespace sub0ecs::store
{
/** Explicit row chunk size for expensive per-row work; does not change pool width.
 *  @tparam Rows Positive rows per chunk. The existing four-chunk inline threshold applies.
 *  @note Small grains increase scratch and scheduling overhead; measure whole calls.
 */
template <std::size_t Rows>
struct RowGrain
{
    static_assert(Rows > 0, "RowGrain requires positive rows");
};
}
