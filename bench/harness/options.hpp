#pragma once
/** Command-line options shared by every benchmark binary (set by bench/tools/run.py). */

#include <chrono>
#include <cstddef>
#include <optional>
#include <string>

namespace bench::harness
{
    struct Options
    {
        std::string filter;                                // --filter=REGEX   (searched in the case name)
        std::size_t epochs = 11;                           // --epochs=N       (rounds of a paired comparison)
        std::chrono::nanoseconds minEpochTime{ 0 };        // --min-epoch-ms=X (0 = nanobench's default)
        std::size_t warmup = 0;                            // --warmup=N       (untimed calls first)
        std::string out;                                   // --out=FILE       (results JSON)
        bool list = false;                                 // --list           (print case names, run nothing)
        bool quiet = false;                                // --quiet          (no nanobench tables on stdout)
    };

    /** Parses argv; returns nothing (after printing why) on an unknown or malformed argument. */
    std::optional<Options> parseOptions(int argc, char** argv);
} // namespace bench::harness
