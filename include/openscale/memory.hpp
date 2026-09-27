#pragma once
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
namespace openscale {
class MappedFile { public: MappedFile()=default; ~MappedFile(); MappedFile(const MappedFile&)=delete; MappedFile& operator=(const MappedFile&)=delete; bool open(const std::filesystem::path&,std::string&); void close(); const uint8_t* data() const{return data_;} uint64_t size() const{return size_;} bool valid() const{return data_!=nullptr;} private: uint8_t* data_=nullptr; uint64_t size_=0; void* handle_=nullptr; };
struct MemoryStats { uint64_t resident=0, mapped=0, peak=0; };
}
