#include <cmath>
#include <cstdio>
#include <ostream>
#include <string>

#include "report.hpp"

namespace bench::harness
{
    namespace
    {
        std::string quoted(const std::string& s)
        {
            std::string out = "\"";
            for (const char c : s)
            {
                switch (c)
                {
                case '"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n"; break;
                case '\t': out += "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20)
                    {
                        char buf[8];
                        std::snprintf(buf, sizeof buf, "\\u%04x", c);
                        out += buf;
                    }
                    else out += c;
                }
            }
            return out + "\"";
        }

        std::string num(double v)
        {
            if (!std::isfinite(v)) return "null";
            char buf[32];
            std::snprintf(buf, sizeof buf, "%.9g", v);
            return buf;
        }
    } // namespace

    void writeJson(std::ostream& out, const std::string& binary, const Options& options, const std::vector<Record>& records)
    {
        out << "{\n  \"schema\": \"sub0ecs-bench-results/1\",\n  \"binary\": " << quoted(binary)
            << ",\n  \"settings\": {\"filter\": " << quoted(options.filter) << ", \"epochs\": " << options.epochs
            << ", \"min_epoch_ms\": " << num(static_cast<double>(options.minEpochTime.count()) / 1e6)
            << ", \"warmup\": " << options.warmup << "},\n  \"results\": [";
        for (std::size_t i = 0; i < records.size(); ++i)
        {
            const Record& r = records[i];
            out << (i ? ",\n" : "\n") << "    {\"name\": " << quoted(r.name) << ", \"scenario\": " << quoted(r.scenario)
                << ", \"pattern\": " << quoted(r.pattern) << ", \"design\": " << quoted(r.design)
                << ", \"group\": " << quoted(r.group) << ", \"n\": " << r.n << ", \"unit\": " << quoted(r.unit)
                << ",\n     \"median_ns\": " << num(r.medianNs) << ", \"err_pct\": " << num(r.errPct)
                << ", \"min_ns\": " << num(r.minNs) << ", \"max_ns\": " << num(r.maxNs) << ", \"epochs\": " << r.epochs
                << ", \"items_per_second\": " << num(r.medianNs > 0 ? r.itemsPerOp * 1e9 / r.medianNs : 0);
            if (r.paired)
            {
                const Paired& p = *r.paired;
                out << ",\n     \"paired\": {\"baseline\": " << quoted(p.baseline) << ", \"ratio\": " << num(p.ratio)
                    << ", \"low\": " << num(p.low) << ", \"high\": " << num(p.high)
                    << ", \"significant\": " << (p.significant ? "true" : "false") << ", \"rounds\": " << p.rounds << "}";
            }
            out << ",\n     \"counters\": {";
            for (std::size_t c = 0; c < r.counters.size(); ++c)
                out << (c ? ", " : "") << quoted(r.counters[c].first) << ": " << num(r.counters[c].second);
            out << "}, \"notes\": {";
            for (std::size_t c = 0; c < r.notes.size(); ++c)
                out << (c ? ", " : "") << quoted(r.notes[c].first) << ": " << quoted(r.notes[c].second);
            out << "}}";
        }
        out << "\n  ]\n}\n";
    }
} // namespace bench::harness
