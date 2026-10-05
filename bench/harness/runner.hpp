#pragma once
/** Runner: measures the matching cases group by group with nanobench.
 *
 * Per group: cases without a setup are prepared together and measured with one
 * paired comparison (nanobench Bench::compare: one iteration count for all, rounds
 * interleaved in rotating order, a confidence interval on each ratio); cases with
 * a setup are measured on their own, one call per epoch. A group of one is measured
 * with Bench::run.
 *
 * Every alternative of a group is alive at once, so the largest group's memory is
 * the sum of its fixtures.
 */

#include <vector>

#include "options.hpp"
#include "record.hpp"
#include "registry.hpp"

namespace bench::harness
{
    std::vector<Record> runAll(const Registry& registry, const Options& options);
} // namespace bench::harness
