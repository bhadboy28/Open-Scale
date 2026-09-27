#pragma once
#include <cstdint>
namespace openscale { struct BenchmarkResult { double seconds=0, bytes_per_sec=0, items_per_sec=0; uint64_t items=0, bytes=0; }; BenchmarkResult benchmark_memory(const uint8_t*,uint64_t,unsigned); }
