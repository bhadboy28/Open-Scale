#pragma once
#include "device.hpp"
#include "memory.hpp"
#include "scheduler.hpp"
namespace openscale {
class Runtime { public: struct Config{size_t memory_limit_bytes=0;size_t workers=0;}; explicit Runtime(const Config& c=Config{}); const DeviceInfo& device()const; MemoryManager& memory(); Scheduler& scheduler(); private: DeviceInfo device_; MemoryManager memory_; Scheduler scheduler_; };
}
