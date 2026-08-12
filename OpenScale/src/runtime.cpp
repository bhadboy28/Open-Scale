#include "openscale/runtime.hpp"
namespace openscale { Runtime::Runtime(const Config&c):device_(detect_device()),memory_(c.memory_limit_bytes),scheduler_(c.workers){} const DeviceInfo&Runtime::device()const{return device_;} MemoryManager&Runtime::memory(){return memory_;} Scheduler&Runtime::scheduler(){return scheduler_;} }
