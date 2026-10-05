#pragma once
/** The results JSON, schema "sub0ecs-bench-results/1", read by the scripts in bench/tools:
 *  {"schema", "binary", "settings": {...}, "results": [Record...]}. */

#include <iosfwd>
#include <string>
#include <vector>

#include "options.hpp"
#include "record.hpp"

namespace bench::harness
{
    void writeJson(std::ostream& out, const std::string& binary, const Options& options, const std::vector<Record>& records);
} // namespace bench::harness
