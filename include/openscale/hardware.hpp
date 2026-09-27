#pragma once
#include <string>
namespace openscale {
struct HardwareInfo {
    unsigned threads{1}; bool avx2{}, avx512f{}, avx512vnni{}, fma{};
    std::string arch;
};
HardwareInfo detect_hardware();
std::string hardware_string(const HardwareInfo&);
}
