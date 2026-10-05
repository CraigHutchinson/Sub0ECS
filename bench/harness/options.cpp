#include "options.hpp"

#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>

namespace bench::harness
{
    namespace
    {
        template <typename T>
        bool number(std::string_view text, T& out)
        {
            const auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), out);
            return ec == std::errc{} && end == text.data() + text.size();
        }
    } // namespace

    std::optional<Options> parseOptions(int argc, char** argv)
    {
        Options o;
        for (int i = 1; i < argc; ++i)
        {
            const std::string_view arg = argv[i];
            std::string_view v;   // the value after a matched "--key="
            const auto key = [&](std::string_view k) {
                if (arg.substr(0, k.size()) != k) return false;
                v = arg.substr(k.size());
                return true;
            };
            bool ok = true;
            if (key("--filter=")) o.filter = std::string(v);
            else if (key("--epochs=")) ok = number(v, o.epochs) && o.epochs > 0;
            else if (key("--warmup=")) ok = number(v, o.warmup);
            else if (key("--out=")) o.out = std::string(v);
            else if (key("--min-epoch-ms="))
            {
                // strtod, not from_chars: floating-point from_chars is missing from some libc++ versions
                const std::string text(v);
                char* end = nullptr;
                const double ms = std::strtod(text.c_str(), &end);
                ok = !text.empty() && end == text.c_str() + text.size() && ms >= 0;
                o.minEpochTime = std::chrono::nanoseconds(static_cast<long long>(ms * 1e6));
            }
            else if (arg == "--list") o.list = true;
            else if (arg == "--quiet") o.quiet = true;
            else ok = false;
            if (!ok)
            {
                std::fprintf(stderr,
                             "unrecognised argument '%s'\n"
                             "options: --filter=REGEX --epochs=N --min-epoch-ms=X --warmup=N --out=FILE --list --quiet\n",
                             argv[i]);
                return std::nullopt;
            }
        }
        return o;
    }
} // namespace bench::harness
