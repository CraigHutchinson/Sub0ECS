#pragma once
/** Record: one measured case, as written to the results JSON (report.hpp).
 *  Times are per operation. */

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace bench::harness
{
    /** Paired comparison against the group's baseline: ratio = t_baseline / t_this,
     *  so > 1 means faster than the baseline; [low, high] is a 95% interval for the
     *  whole group (Bonferroni-corrected by nanobench). */
    struct Paired
    {
        std::string baseline;
        double ratio = 1.0;
        double low = 1.0;
        double high = 1.0;
        bool significant = false;
        std::size_t rounds = 0;
    };

    struct Record
    {
        std::string name, scenario, pattern, design, group, unit;
        std::int64_t n = 0;
        double itemsPerOp = 1;
        double medianNs = 0;     // per operation
        double errPct = 0;       // median absolute percent error across epochs
        double minNs = 0, maxNs = 0;
        std::size_t epochs = 0;
        std::optional<Paired> paired;
        std::vector<std::pair<std::string, double>> counters;
        std::vector<std::pair<std::string, std::string>> notes;

        void counter(std::string key, double value) { counters.emplace_back(std::move(key), value); }
        void note(std::string key, std::string value) { notes.emplace_back(std::move(key), std::move(value)); }
    };
} // namespace bench::harness
