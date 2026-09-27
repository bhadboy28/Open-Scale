#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace openscale {
class Tokenizer { public: bool load(const std::string& path,std::string&); std::vector<uint32_t> encode(const std::string&) const; std::string decode(const std::vector<uint32_t>&) const; };
}
