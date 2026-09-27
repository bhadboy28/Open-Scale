#include "openscale/openscale.hpp"
#include <thread>
#ifdef _WIN32
#include <windows.h>
#endif
namespace openscale { HardwareInfo detect_hardware(){ HardwareInfo h; h.threads=std::max(1u,std::thread::hardware_concurrency());
#if defined(__AVX2__)
h.avx2=true;
#endif
#if defined(__AVX512F__)
h.avx512=true;
#endif
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
h.neon=true;
#endif
#ifdef _WIN32
MEMORYSTATUSEX s{}; s.dwLength=sizeof(s); if(GlobalMemoryStatusEx(&s)) h.ram_bytes=s.ullTotalPhys;
#endif
return h; }
std::string error_name(ErrorCode c){switch(c){case ErrorCode::Ok:return"OK";case ErrorCode::InvalidArgument:return"INVALID_ARGUMENT";case ErrorCode::Io:return"IO_ERROR";case ErrorCode::Format:return"FORMAT_ERROR";case ErrorCode::Corrupt:return"CORRUPT";case ErrorCode::Unsupported:return"UNSUPPORTED";case ErrorCode::Memory:return"MEMORY";case ErrorCode::Module:return"MODULE";case ErrorCode::Backend:return"BACKEND";default:return"INTERNAL";}}}
