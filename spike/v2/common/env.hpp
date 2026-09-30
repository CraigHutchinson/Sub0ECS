#pragma once
/** Benchmark configuration from the environment (set by tools/bench/run.py so
 *  the same binaries scale from a 4-core VM to a many-core reference machine). */

#include <cstdint>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

namespace spike::env
{
    /** Comma-separated integer list from $name, or `fallback` if unset/empty. */
    inline std::vector<std::int64_t> list(const char* name, std::vector<std::int64_t> fallback)
    {
        const char* v = std::getenv(name);
        if (!v || !*v || std::string(v) == "auto") return fallback;
        std::vector<std::int64_t> out;
        std::string s(v);
        for (std::size_t pos = 0; pos <= s.size();)
        {
            const std::size_t comma = s.find(',', pos);
            const std::string item = s.substr(pos, comma == std::string::npos ? std::string::npos : comma - pos);
            if (!item.empty()) out.push_back(std::atoll(item.c_str()));
            if (comma == std::string::npos) break;
            pos = comma + 1;
        }
        return out.empty() ? fallback : out;
    }

    /** 1, 2, 4, ... up to the hardware thread count (always including it). */
    inline std::vector<std::int64_t> threadLadder()
    {
        const std::int64_t hw = std::max(1u, std::thread::hardware_concurrency());
        std::vector<std::int64_t> t;
        for (std::int64_t i = 1; i < hw; i *= 2) t.push_back(i);
        t.push_back(hw);
        return t;
    }
} // namespace spike::env
