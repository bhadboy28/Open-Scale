#include "openscale/memory.hpp"
#include <cstdlib>
#include <new>
namespace openscale {
MemoryManager::MemoryManager(size_t limit_bytes):limit_bytes_(limit_bytes){}
MemoryManager::~MemoryManager(){ for(auto &p:allocations_) std::free(p.first); }
void* MemoryManager::allocate(size_t bytes){
    if(bytes==0) return nullptr;
    std::lock_guard<std::mutex> lock(mutex_);
    if(limit_bytes_ && allocated_bytes_+bytes>limit_bytes_) throw std::bad_alloc();
    void* p=std::malloc(bytes); if(!p) throw std::bad_alloc();
    allocations_[p]=bytes; allocated_bytes_+=bytes; return p;
}
void MemoryManager::release(void* ptr){ if(!ptr)return; std::lock_guard<std::mutex> lock(mutex_); auto it=allocations_.find(ptr); if(it!=allocations_.end()){allocated_bytes_-=it->second; allocations_.erase(it); std::free(ptr);} }
size_t MemoryManager::allocated() const {std::lock_guard<std::mutex> lock(mutex_);return allocated_bytes_;}
size_t MemoryManager::limit() const{return limit_bytes_;}
}
