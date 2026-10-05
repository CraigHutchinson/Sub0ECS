#pragma once
/** Flush denormals to zero on the calling thread.
 *
 * The physics kernels damp velocity every step, so long benchmark runs drift into
 * denormal floats and end up timing the FPU's microcode path (10-30x slower,
 * varying with iteration count). The benchmarks set it explicitly and build
 * without -ffast-math, so conformance stays bit-exact.
 */

#include <cstdint>

#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#    include <xmmintrin.h>
#    define BENCH_X86_MXCSR 1
#endif

namespace bench
{
    inline void flushDenormals()
    {
#if defined(BENCH_X86_MXCSR)
        _mm_setcsr(_mm_getcsr() | 0x8040u);   // FTZ (bit 15) + DAZ (bit 6)
#elif defined(__aarch64__)
        std::uint64_t fpcr = 0;
        __asm__ __volatile__("mrs %0, fpcr" : "=r"(fpcr));
        __asm__ __volatile__("msr fpcr, %0" : : "r"(fpcr | (std::uint64_t{ 1 } << 24)));   // FZ
#endif
    }
} // namespace bench
