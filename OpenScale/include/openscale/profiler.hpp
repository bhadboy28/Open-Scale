#pragma once
#include <chrono>
namespace openscale {
class Profiler { public: void start(); double stop_seconds(); private: std::chrono::steady_clock::time_point start_; };
}
