#include "openscale/benchmark.hpp"
#include <chrono>
namespace openscale { BenchmarkResult benchmark_memory(const uint8_t*d,uint64_t n,unsigned it){BenchmarkResult r;r.bytes=n*uint64_t(it);r.items=it;volatile uint64_t sink=0;auto s=std::chrono::steady_clock::now();for(unsigned k=0;k<it;k++){uint64_t x=0;for(uint64_t i=0;i<n;i+=64)x^=d[i];sink^=x;}auto e=std::chrono::duration<double>(std::chrono::steady_clock::now()-s).count();r.seconds=e;r.bytes_per_sec=e?double(r.bytes)/e:0;r.items_per_sec=e?double(it)/e:0;(void)sink;return r;} }
