#include "openscale/openscale.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
namespace openscale { MemoryMap::~MemoryMap(){close();} Error MemoryMap::open(const std::filesystem::path&p){close();
#ifdef _WIN32
HANDLE f=CreateFileW(p.wstring().c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr);if(f==INVALID_HANDLE_VALUE)return{ErrorCode::Io,"cannot open file"};LARGE_INTEGER s;GetFileSizeEx(f,&s);size_=s.QuadPart;HANDLE m=CreateFileMappingW(f,nullptr,PAGE_READONLY,0,0,nullptr);if(!m){CloseHandle(f);return{ErrorCode::Io,"mapping failed"};}void*d=MapViewOfFile(m,FILE_MAP_READ,0,0,0);if(!d){CloseHandle(m);CloseHandle(f);return{ErrorCode::Io,"map failed"};}data_=static_cast<const std::byte*>(d);handle_=f;mapping_=m;
#else
fd_=::open(p.c_str(),O_RDONLY);if(fd_<0)return{ErrorCode::Io,"cannot open file"};struct stat st{};if(fstat(fd_,&st)!=0){close();return{ErrorCode::Io,"stat failed"};}size_=st.st_size;if(!size_){close();return{ErrorCode::Format,"empty file"};}void*d=mmap(nullptr,size_,PROT_READ,MAP_PRIVATE,fd_,0);if(d==MAP_FAILED){close();return{ErrorCode::Io,"mmap failed"};}data_=static_cast<const std::byte*>(d);
#endif
return{};} void MemoryMap::close(){
#ifdef _WIN32
if(data_)UnmapViewOfFile(data_);if(mapping_)CloseHandle((HANDLE)mapping_);if(handle_)CloseHandle((HANDLE)handle_);data_=nullptr;mapping_=nullptr;handle_=nullptr;
#else
if(data_)munmap((void*)data_,size_);if(fd_>=0)::close(fd_);data_=nullptr;fd_=-1;
#endif
size_=0;}}
