#include "openscale/device.hpp"
#include <thread>
namespace openscale {
DeviceInfo detect_device(){
 DeviceInfo d; d.cpu_threads=std::thread::hardware_concurrency(); if(!d.cpu_threads)d.cpu_threads=1;
#if defined(__x86_64__) || defined(_M_X64)
 d.architecture="x86_64";
#if defined(__GNUC__) || defined(__clang__)
 d.avx=__builtin_cpu_supports("avx"); d.avx2=__builtin_cpu_supports("avx2"); d.avx512=__builtin_cpu_supports("avx512f");
#endif
#elif defined(__aarch64__) || defined(_M_ARM64)
 d.architecture="ARM64";
#else
 d.architecture="unknown";
#endif
 return d;
}
}
