#pragma once
/** Case: one benchmark, and what preparing it produces.
 *
 * A case is named <Scenario>/<Pattern>/<Design>/<N>. Cases that share
 * scenario, pattern and N form a *group*: the runner measures a group's
 * alternatives against each other with a paired, interleaved comparison
 * (nanobench's Bench::compare), so machine drift cancels out of the ratios.
 * The first case registered in a group is its baseline.
 */

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "record.hpp"

namespace bench::harness
{
    /** What a case's prepare() returns: the timed operation and the state it runs on. */
    struct Prepared
    {
        /** One timed unit of work, e.g. one pass of a system over the world. Called
         *  repeatedly; it must leave the fixture able to run again (a steady state). */
        std::function<void()> op;

        /** Untimed reset before every timed call (e.g. a fresh world for Create).
         *  Required exactly when the case is declared `alone`. */
        std::function<void()> setup;

        /** Optional: add counters and notes to the record once measuring is done. */
        std::function<void(Record&)> finish;

        /** Keeps the fixture (world, entities, thread pool) alive while it is measured. */
        std::shared_ptr<void> fixture;
    };

    struct Case
    {
        std::string scenario;   // e.g. "Update2"
        std::string pattern;    // e.g. "Fragmented"; "-" when not applicable
        std::string design;     // e.g. "QPartHinted"
        std::int64_t n = 0;     // problem size: entities, units per team, ...
        double itemsPerOp = 1;  // for items_per_second (entities per pass, ticks, ...)
        std::string unit = "op";
        /** Measured on its own, one call per epoch with Prepared::setup before each,
         *  instead of in the group's paired comparison (the op cannot run twice as-is). */
        bool alone = false;
        /** Fixed operations per epoch (0 = let nanobench calibrate). Set it when every
         *  alternative must perform exactly the same operations: a calibrated count is
         *  shared, but calibration itself runs each alternative a different number of
         *  times first, so an evolving fixture (a game) would diverge between designs.
         *  A group uses its first (baseline) case's value. */
        std::uint64_t epochIterations = 0;
        std::function<Prepared()> prepare;

        std::string name() const { return scenario + "/" + pattern + "/" + design + "/" + std::to_string(n); }
        std::string group() const { return scenario + "/" + pattern + "/" + std::to_string(n); }
    };
} // namespace bench::harness
