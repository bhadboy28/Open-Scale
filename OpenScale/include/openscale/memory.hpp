#pragma once
#include <cstddef>
#include <unordered_map>
#include <mutex>

namespace openscale {
class MemoryManager {
public:
    explicit MemoryManager(size_t limit_bytes = 0);
    ~MemoryManager();
    void* allocate(size_t bytes);
    void release(void* ptr);
    size_t allocated() const;
    size_t limit() const;
private:
    size_t limit_bytes_;
    size_t allocated_bytes_{0};
    std::unordered_map<void*,size_t> allocations_;
    mutable std::mutex mutex_;
};
}
