#include "entry.hpp"

#include <cstdio>
#include <fstream>

#include "../common/denormals.hpp"
#include "options.hpp"
#include "report.hpp"
#include "runner.hpp"

namespace bench::harness
{
    int benchMain(int argc, char** argv, const Registry& registry, const char* binary)
    {
        bench::flushDenormals();
        const auto options = parseOptions(argc, argv);
        if (!options) return 2;
        if (options->list)
        {
            for (const Case* c : registry.matching(options->filter)) std::printf("%s\n", c->name().c_str());
            return 0;
        }
        const std::vector<Record> records = runAll(registry, *options);
        if (!options->out.empty())
        {
            std::ofstream file(options->out);
            writeJson(file, binary, *options, records);
            if (!file)
            {
                std::fprintf(stderr, "could not write %s\n", options->out.c_str());
                return 1;
            }
        }
        return 0;
    }
} // namespace bench::harness
