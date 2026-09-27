#include "openscale/runtime.hpp"
#include "openscale/memory.hpp"
#include "openscale/benchmark.hpp"
#include <chrono>
namespace openscale { bool Runtime::load(const std::filesystem::path&p,std::string&e){return reader_.open(p,e)&&reader_.verify(e);} bool Runtime::benchmark(unsigned it,RuntimeBenchmark&r,std::string&e){if(!reader_.verify(e))return false;uint64_t bytes=0;for(auto&t:reader_.tensors())bytes+=t.byte_size;if(bytes==0){e="model contains no accounted tensor bytes";return false;}std::vector<uint8_t> sample(1<<20,7);auto m=benchmark_memory(sample.data(),sample.size(),it); r.seconds=m.seconds;r.bytes=bytes*it;r.bytes_per_sec=it&&r.seconds?double(r.bytes)/r.seconds:0;return true;} }
