#include "openscale/hardware.hpp"
#include <thread>
namespace openscale {
HardwareInfo detect_hardware(){HardwareInfo h;h.threads=std::max(1u,std::thread::hardware_concurrency());
#if defined(_M_X64)||defined(__x86_64__)||defined(__i386__)
h.arch="x86";
#if defined(__GNUC__) && !defined(__clang__)
h.avx2=__builtin_cpu_supports("avx2");h.avx512f=__builtin_cpu_supports("avx512f");h.avx512vnni=__builtin_cpu_supports("avx512vnni");h.fma=__builtin_cpu_supports("fma");
#endif
#elif defined(_M_ARM64)||defined(__aarch64__)
h.arch="arm64";
#else
h.arch="unknown";
#endif
return h;}
std::string hardware_string(const HardwareInfo&h){return h.arch+" threads="+std::to_string(h.threads)+" avx2="+std::to_string(h.avx2)+" avx512f="+std::to_string(h.avx512f)+" vnni="+std::to_string(h.avx512vnni)+" fma="+std::to_string(h.fma);}
}
