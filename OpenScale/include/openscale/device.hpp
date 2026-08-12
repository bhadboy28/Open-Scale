#pragma once
#include <string>
namespace openscale {
struct DeviceInfo { std::string architecture; unsigned int cpu_threads; bool avx=false; bool avx2=false; bool avx512=false; };
DeviceInfo detect_device();
}
