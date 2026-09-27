#include "openscale/backend.hpp"
#include "openscale/hardware.hpp"
namespace openscale {
class CpuBackend final: public Backend { BackendCaps c_; public: CpuBackend(){auto h=detect_hardware(); c_.kind=BackendKind::CPU;c_.name="CPU";c_.available=true;c_.score=h.avx512f?90:h.avx2?70:40;c_.fp16=c_.bf16=true;c_.int8=c_.int4=true;} const BackendCaps& caps()const override{return c_;} bool supports(const std::string& op)const override{return op=="matmul"||op=="attention"||op=="rmsnorm"||op=="rope";} };
std::unique_ptr<Backend> make_backend(BackendKind k){ if(k==BackendKind::CPU)return std::make_unique<CpuBackend>(); return {}; }
std::vector<BackendCaps> enumerate_backends(){std::vector<BackendCaps> out;auto b=make_backend(BackendKind::CPU);out.push_back(b->caps());
#ifdef OPENSCALE_ENABLE_CUDA
out.push_back({BackendKind::CUDA,"CUDA",true,100,true,true,true,true});
#else
out.push_back({BackendKind::CUDA,"CUDA",false,0,false,false,false,false});
#endif
#ifdef OPENSCALE_ENABLE_HIP
out.push_back({BackendKind::HIP,"HIP",true,95,true,true,true,true});
#else
out.push_back({BackendKind::HIP,"HIP",false,0,false,false,false,false});
#endif
#ifdef OPENSCALE_ENABLE_METAL
out.push_back({BackendKind::Metal,"Metal",true,95,true,true,true,true});
#else
out.push_back({BackendKind::Metal,"Metal",false,0,false,false,false,false});
#endif
#ifdef OPENSCALE_ENABLE_VULKAN
out.push_back({BackendKind::Vulkan,"Vulkan",true,80,true,true,true,true});
#else
out.push_back({BackendKind::Vulkan,"Vulkan",false,0,false,false,false,false});
#endif
return out;}
}
