#pragma once
/** benchMain: the shared main() of every benchmark binary. Parses the options,
 *  runs the registry's matching cases and writes the results JSON. */

#include "registry.hpp"

namespace bench::harness
{
    int benchMain(int argc, char** argv, const Registry& registry, const char* binary);
} // namespace bench::harness
