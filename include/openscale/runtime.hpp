#pragma once
#include "av_format.hpp"
#include <filesystem>
#include <string>
namespace openscale { struct RuntimeBenchmark { double seconds=0,bytes_per_sec=0; uint64_t bytes=0; }; class Runtime { AvReader reader_; public: bool load(const std::filesystem::path&,std::string&); bool benchmark(unsigned,RuntimeBenchmark&,std::string&); const ModelInfo& info()const{return reader_.info();} const std::vector<TensorInfo>& tensors()const{return reader_.tensors();} }; }
