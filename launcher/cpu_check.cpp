// Legacy build: check the x86-64-v2 feature set, not the stock v3 set.
#include "launcher.h"

#if defined(__x86_64__) || defined(_M_X64)
#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <cpuid.h>
#endif
#endif

namespace launcher {

#if defined(__x86_64__) || defined(_M_X64)
namespace {
void cpuid(unsigned leaf, unsigned sub, unsigned r[4]) {
#if defined(_MSC_VER)
    int v[4];
    __cpuidex(v, int(leaf), int(sub));
    for (int i = 0; i < 4; ++i) r[i] = unsigned(v[i]);
#else
    __cpuid_count(leaf, sub, r[0], r[1], r[2], r[3]);
#endif
}
}  // namespace

const char* cpu_missing_feature() {
    unsigned r[4];
    cpuid(0, 0, r);
    if (r[0] < 1) return "CPUID leaf 1";
    cpuid(1, 0, r);
    const unsigned ecx = r[2];
    if (!(ecx & (1u << 0))) return "SSE3";
    if (!(ecx & (1u << 9))) return "SSSE3";
    if (!(ecx & (1u << 13))) return "CMPXCHG16B";
    if (!(ecx & (1u << 19))) return "SSE4.1";
    if (!(ecx & (1u << 20))) return "SSE4.2";
    if (!(ecx & (1u << 23))) return "POPCNT";
    cpuid(0x80000000u, 0, r);
    if (r[0] < 0x80000001u) return "LAHF/SAHF";
    cpuid(0x80000001u, 0, r);
    if (!(r[2] & 1u)) return "LAHF/SAHF";
    return nullptr;
}
#else
const char* cpu_missing_feature() { return nullptr; }
#endif

}  // namespace launcher
