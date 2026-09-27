#pragma once
#include "av_format.hpp"
namespace openscale {
struct Feasibility {
    long double params{};
    long double weight_gb{};
    long double kv_gb{};
    long double working_gb{};
    long double total_gb{};
    long double available_gb{};
    bool resident{};
    bool streaming{};
    std::string note;
};
Feasibility analyze(const ModelInfo&, double available_ram_gb,
                    double bits_per_weight=4.0, uint64_t context=4096,
                    uint64_t batch=1);
}
