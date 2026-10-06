#pragma once
/** CpuTopology: which logical CPUs this process may use, and which of them are
 *  the capable ones.
 *
 *  Hybrid CPUs mix performance and efficiency cores. A compute pool that starts
 *  one thread per logical CPU puts most of its threads on the slow ones, and a
 *  process restricted to a few CPUs (affinity, a container) has fewer than
 *  hardware_concurrency() to use at all. This reports both, once.
 *
 *  On Windows this header includes <windows.h> (with NOMINMAX and
 *  WIN32_LEAN_AND_MEAN unless already defined).
 *
 *  Everything here is a hint. Where the platform gives no answer, every CPU is
 *  reported as one class; callers must work the same. */

#include <algorithm>
#include <cstdint>
#include <thread>
#include <vector>

#if defined(_WIN32)
#    if !defined(NOMINMAX)
#        define NOMINMAX   // keep std::min/std::max usable after <windows.h>
#    endif
#    if !defined(WIN32_LEAN_AND_MEAN)
#        define WIN32_LEAN_AND_MEAN
#    endif
#    include <windows.h>
#elif defined(__linux__)
#    include <cstdio>
#    include <sched.h>
#elif defined(__APPLE__)
#    include <sys/sysctl.h>
#    include <sys/types.h>
#endif

namespace sub0ecs::fusion
{
    struct CpuTopology
    {
        /** Logical CPUs this process may run on, most capable class first. */
        std::vector<unsigned> cpus;
        /** How many of `cpus` are in the most capable class (all of them on a uniform CPU). */
        unsigned performance = 1;

        /** True when the process may use CPUs of more than one class. */
        bool hybrid() const { return performance < cpus.size(); }
    };

    namespace detail
    {
        struct RankedCpu
        {
            unsigned id;
            long long capability;   // higher = more capable; equal for all when unknown
        };

        /** Builds the topology from the CPUs the process may use and a capability per CPU. */
        inline CpuTopology rank(std::vector<RankedCpu> allowed)
        {
            CpuTopology t;
            if (allowed.empty())
            {
                const unsigned n = std::max(1u, std::thread::hardware_concurrency());
                for (unsigned i = 0; i < n; ++i) t.cpus.push_back(i);
                t.performance = n;
                return t;
            }
            std::stable_sort(allowed.begin(), allowed.end(), [](const RankedCpu& a, const RankedCpu& b) { return a.capability > b.capability; });
            const long long top = allowed.front().capability;
            t.performance = 0;
            for (const RankedCpu& c : allowed)
            {
                t.cpus.push_back(c.id);
                if (c.capability == top) ++t.performance;
            }
            return t;
        }

        inline CpuTopology detectTopology()
        {
            std::vector<RankedCpu> allowed;
#if defined(_WIN32)
            // Single processor group only (up to 64 logical CPUs); otherwise report one class.
            DWORD_PTR process = 0, system = 0;
            DWORD length = 0;
            GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);
            std::vector<unsigned char> buffer(length);
            if (length != 0 && GetProcessAffinityMask(GetCurrentProcess(), &process, &system) &&
                GetLogicalProcessorInformationEx(RelationProcessorCore, reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>(buffer.data()), &length))
            {
                for (DWORD offset = 0; offset < length;)
                {
                    const auto* info = reinterpret_cast<const SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>(buffer.data() + offset);
                    if (info->Relationship == RelationProcessorCore)
                    {
                        if (info->Processor.GroupCount != 1 || info->Processor.GroupMask[0].Group != 0) return rank({});
                        const KAFFINITY mask = info->Processor.GroupMask[0].Mask & process;
                        for (unsigned bit = 0; bit < 64; ++bit)
                            if (mask >> bit & 1u) allowed.push_back({ bit, info->Processor.EfficiencyClass });
                    }
                    offset += info->Size;
                }
            }
#elif defined(__linux__)
            cpu_set_t set;
            CPU_ZERO(&set);
            if (sched_getaffinity(0, sizeof(set), &set) == 0)
            {
                // Capacity where the kernel reports it (Arm), else the maximum frequency, which
                // separates performance from efficiency cores on hybrid x86.
                const auto read = [](unsigned cpu, const char* leaf) -> long long {
                    char path[96];
                    std::snprintf(path, sizeof(path), "/sys/devices/system/cpu/cpu%u/%s", cpu, leaf);
                    long long value = 0;
                    if (std::FILE* f = std::fopen(path, "r"))
                    {
                        if (std::fscanf(f, "%lld", &value) != 1) value = 0;
                        std::fclose(f);
                    }
                    return value;
                };
                for (unsigned cpu = 0; cpu < static_cast<unsigned>(CPU_SETSIZE); ++cpu)
                {
                    if (!CPU_ISSET(cpu, &set)) continue;
                    long long capability = read(cpu, "cpu_capacity");
                    if (capability == 0) capability = read(cpu, "cpufreq/cpuinfo_max_freq");
                    allowed.push_back({ cpu, capability });
                }
            }
#elif defined(__APPLE__)
            // No affinity API: every CPU is usable; report the performance-level count.
            int performance = 0;
            std::size_t size = sizeof(performance);
            const unsigned n = std::max(1u, std::thread::hardware_concurrency());
            if (sysctlbyname("hw.perflevel0.logicalcpu", &performance, &size, nullptr, 0) != 0 || performance <= 0) performance = static_cast<int>(n);
            CpuTopology t;
            for (unsigned i = 0; i < n; ++i) t.cpus.push_back(i);
            t.performance = std::min(n, static_cast<unsigned>(performance));
            return t;
#endif
            return rank(std::move(allowed));
        }
    } // namespace detail

    /** The topology, detected on first use. It does not follow later affinity changes. */
    inline const CpuTopology& cpuTopology()
    {
        static const CpuTopology topology = detail::detectTopology();
        return topology;
    }

    /** Restricts the calling thread to the performance cores the process may use, leaving
     *  the choice among them to the OS. Returns false and changes nothing when the CPU is
     *  not hybrid, a CPU cannot be addressed by a 64-bit mask, or the platform has no
     *  thread affinity. */
    inline bool keepThisThreadOnPerformanceCores()
    {
        const CpuTopology& topology = cpuTopology();
        if (!topology.hybrid()) return false;
        std::uint64_t mask = 0;
        for (unsigned i = 0; i < topology.performance; ++i)
        {
            if (topology.cpus[i] >= 64) return false;
            mask |= std::uint64_t{ 1 } << topology.cpus[i];
        }
#if defined(_WIN32)
        return SetThreadAffinityMask(GetCurrentThread(), static_cast<DWORD_PTR>(mask)) != 0;
#elif defined(__linux__)
        cpu_set_t set;
        CPU_ZERO(&set);
        for (unsigned cpu = 0; cpu < 64; ++cpu)
            if (mask >> cpu & 1u) CPU_SET(cpu, &set);
        return sched_setaffinity(0, sizeof(set), &set) == 0;
#else
        return false;
#endif
    }

} // namespace sub0ecs::fusion
