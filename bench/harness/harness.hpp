#pragma once
/** The benchmark harness: nanobench with names, filtering, paired group comparisons
 *  and a JSON report for bench/tools/*.py.
 *
 *   case.hpp      Case, Prepared: what a benchmark is
 *   registry.hpp  Registry: registration and --filter
 *   runner.hpp    runAll: groups measured with paired comparisons
 *   record.hpp    Record, Paired: one measured case
 *   report.hpp    writeJson: the results file
 *   options.hpp   Options: the command line
 *   entry.hpp     benchMain: the shared main()
 */

#include "case.hpp"
#include "entry.hpp"
#include "options.hpp"
#include "record.hpp"
#include "registry.hpp"
#include "report.hpp"
#include "runner.hpp"
