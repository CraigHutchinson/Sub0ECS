#pragma once
/** Registry: the cases a benchmark binary offers, in registration order. */

#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "case.hpp"

namespace bench::harness
{
    class Registry
    {
    public:
        void add(Case c) { cases_.push_back(std::move(c)); }

        /** Cases whose name matches `filter` (ECMAScript regex, searched; empty = all). */
        std::vector<const Case*> matching(const std::string& filter) const
        {
            std::vector<const Case*> out;
            const std::regex rx(filter.empty() ? ".*" : filter);
            for (const Case& c : cases_)
                if (std::regex_search(c.name(), rx)) out.push_back(&c);
            return out;
        }

    private:
        std::vector<Case> cases_;
    };
} // namespace bench::harness
