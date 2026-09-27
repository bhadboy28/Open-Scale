#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace openscale {
enum class BackendKind { CPU, CUDA, HIP, Metal, Vulkan, Unknown };
struct BackendCaps { BackendKind kind{BackendKind::Unknown}; std::string name; bool available=false; uint32_t score=0; bool fp16=false,bf16=false,int8=false,int4=false; };
class Backend { public: virtual ~Backend()=default; virtual const BackendCaps& caps() const=0; virtual bool supports(const std::string& op) const=0; };
std::vector<BackendCaps> enumerate_backends();
std::unique_ptr<Backend> make_backend(BackendKind kind);
}
