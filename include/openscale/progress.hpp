#pragma once
#include <chrono>
#include <cstdint>
#include <string>

namespace openscale {
class Progress {
    std::string stage_;
    uint64_t done_{0}, total_{0};
    std::chrono::steady_clock::time_point start_;
public:
    explicit Progress(uint64_t total=0);
    void stage(const std::string&);
    void update(uint64_t done);
    void finish();
};
}
