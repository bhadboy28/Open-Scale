#pragma once
#include <cstddef>
#include <string>
namespace openscale {
class MappedFile { public: MappedFile(); ~MappedFile(); bool open(const std::string& path); void close(); const void* data()const; size_t size()const; bool valid()const; private: void* data_; size_t size_; int fd_; };
}
