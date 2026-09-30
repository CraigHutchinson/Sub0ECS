#include "runner.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include <nanobench.h>

namespace bench::harness
{
    namespace
    {
        namespace nb = ankerl::nanobench;

        /** A prepared alternative as nanobench sees it. All alternatives share this one
         *  type, so a runtime-sized group can go through the compile-time compare(): the
         *  cost is one indirect call per operation, equal for every alternative, against
         *  operations that are whole passes over a world. */
        struct Call
        {
            const std::function<void()>* op;
            void operator()() const { (*op)(); }
        };

        constexpr std::size_t kMaxAlternatives = 16;
        using CompareFn = nb::CompareResult (*)(nb::Bench&, const std::vector<std::string>&, const std::vector<Call>&);

        template <std::size_t... I>
        nb::CompareResult compareN(nb::Bench& b, const std::vector<std::string>& names, const std::vector<Call>& calls,
                                   std::index_sequence<I...>)
        {
            // compare() takes `name, op` pairs: flatten (names[I], calls[I])... into one argument list.
            return std::apply([&](const auto&... args) { return b.compare(args...); },
                              std::tuple_cat(std::tuple<const std::string&, const Call&>(names[I], calls[I])...));
        }

        template <std::size_t K>
        nb::CompareResult compareFixed(nb::Bench& b, const std::vector<std::string>& names, const std::vector<Call>& calls)
        {
            if constexpr (K < 2) std::abort();   // a comparison needs two alternatives; callers never ask for fewer
            else return compareN(b, names, calls, std::make_index_sequence<K>{});
        }

        nb::CompareResult compareAll(nb::Bench& b, const std::vector<std::string>& names, const std::vector<Call>& calls)
        {
            static constexpr auto table = []<std::size_t... K>(std::index_sequence<K...>) {
                return std::array<CompareFn, sizeof...(K)>{ &compareFixed<K>... };
            }(std::make_index_sequence<kMaxAlternatives + 1>{});
            return table[calls.size()](b, names, calls);
        }

        nb::Bench configured(const Options& o, const std::string& title, const std::string& unit)
        {
            nb::Bench b;
            b.title(title).unit(unit).epochs(o.epochs).warmup(o.warmup).output(o.quiet ? nullptr : &std::cout);
            if (o.minEpochTime.count() > 0) b.minEpochTime(o.minEpochTime);
            return b;
        }

        Record recordFor(const Case& c)
        {
            Record r;
            r.name = c.name();
            r.scenario = c.scenario;
            r.pattern = c.pattern;
            r.design = c.design;
            r.group = c.group();
            r.unit = c.unit;
            r.n = c.n;
            r.itemsPerOp = c.itemsPerOp;
            return r;
        }

        void fill(Record& r, const nb::Result& result)
        {
            using M = nb::Result::Measure;
            r.medianNs = result.median(M::elapsed) * 1e9;   // nanobench reports seconds per iteration
            r.errPct = result.medianAbsolutePercentError(M::elapsed) * 100.0;
            r.minNs = result.minimum(M::elapsed) * 1e9;
            r.maxNs = result.maximum(M::elapsed) * 1e9;
            r.epochs = result.size();
        }

        /** A case with a setup: one call per epoch, the setup untimed before each. */
        Record runAlone(const Case& c, const Options& o)
        {
            Prepared p = c.prepare();
            nb::Bench b = configured(o, c.group(), c.unit);
            b.epochIterations(1);
            b.setup([&] { p.setup(); }).run(c.name(), [&] { p.op(); });
            Record r = recordFor(c);
            fill(r, b.results().back());
            if (p.finish) p.finish(r);
            return r;
        }

        /** The group's other cases, prepared together and compared against the first. */
        void runPaired(const std::vector<const Case*>& cases, const Options& o, std::vector<Record>& out)
        {
            std::vector<Prepared> prepared;
            prepared.reserve(cases.size());
            for (const Case* c : cases) prepared.push_back(c->prepare());

            nb::Bench b = configured(o, cases.front()->group(), cases.front()->unit);
            if (cases.front()->epochIterations > 0) b.epochIterations(cases.front()->epochIterations);   // no calibration
            if (cases.size() == 1)
            {
                b.run(cases.front()->name(), [&] { prepared.front().op(); });
                Record r = recordFor(*cases.front());
                fill(r, b.results().back());
                if (prepared.front().finish) prepared.front().finish(r);
                out.push_back(std::move(r));
                return;
            }

            // Larger groups than one compare() instantiation holds: compare in chunks, each
            // led by the baseline so every ratio is against the same design.
            for (std::size_t first = 1; first < cases.size(); first += kMaxAlternatives - 1)
            {
                const std::size_t last = std::min(cases.size(), first + kMaxAlternatives - 1);
                std::vector<std::size_t> idx{ 0 };
                for (std::size_t i = first; i < last; ++i) idx.push_back(i);

                std::vector<std::string> names;
                std::vector<Call> calls;
                for (std::size_t i : idx)
                {
                    names.push_back(cases[i]->name());
                    calls.push_back(Call{ &prepared[i].op });
                }
                const nb::CompareResult cmp = compareAll(b, names, calls);

                for (std::size_t k = 0; k < idx.size(); ++k)
                {
                    if (k == 0 && first != 1) continue;   // the baseline is recorded once, from the first chunk
                    const std::size_t i = idx[k];
                    Record r = recordFor(*cases[i]);
                    fill(r, cmp[k].result);
                    r.paired = Paired{ cases.front()->design, cmp[k].relative, cmp[k].relativeLow, cmp[k].relativeHigh,
                                       k != 0 && cmp.isSignificant(k), cmp.rounds() };
                    if (prepared[i].finish) prepared[i].finish(r);
                    out.push_back(std::move(r));
                }
            }
        }
    } // namespace

    std::vector<Record> runAll(const Registry& registry, const Options& options)
    {
        // Groups in order of first appearance; members in registration order (first = baseline).
        std::vector<std::pair<std::string, std::vector<const Case*>>> groups;
        for (const Case* c : registry.matching(options.filter))
        {
            auto it = std::find_if(groups.begin(), groups.end(), [&](const auto& g) { return g.first == c->group(); });
            if (it == groups.end()) groups.push_back({ c->group(), { c } });
            else it->second.push_back(c);
        }

        std::vector<Record> records;
        for (const auto& [key, members] : groups)
        {
            std::vector<const Case*> paired;
            for (const Case* c : members)
            {
                if (c->alone) records.push_back(runAlone(*c, options));   // prepared, measured, released
                else paired.push_back(c);
            }
            if (!paired.empty()) runPaired(paired, options, records);
        }
        return records;
    }
} // namespace bench::harness
