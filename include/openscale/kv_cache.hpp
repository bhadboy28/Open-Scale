#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
namespace openscale {
struct KVConfig { uint32_t layers=0, kv_heads=0, head_dim=0, max_tokens=0; size_t bytes_per_element=2; };
class KVCache { public: bool init(const KVConfig&); void reset(); size_t bytes() const{return data_.size();} uint32_t tokens() const{return tokens_;} bool append(const void* data,size_t bytes); private: KVConfig cfg_{}; std::vector<uint8_t> data_; uint32_t tokens_=0; };
}
