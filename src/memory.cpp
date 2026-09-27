#include "openscale/memory.hpp"
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#include <fstream>
namespace openscale {
MappedFile::~MappedFile(){close();}
bool MappedFile::open(const std::filesystem::path&p,std::string&e){close();
#ifdef _WIN32
HANDLE file=CreateFileW(p.wstring().c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);if(file==INVALID_HANDLE_VALUE){e="cannot open file";return false;}LARGE_INTEGER n;if(!GetFileSizeEx(file,&n)){CloseHandle(file);e="cannot stat file";return false;}HANDLE map=CreateFileMappingW(file,nullptr,PAGE_READONLY,0,0,nullptr);if(!map){CloseHandle(file);e="cannot map file";return false;}auto* d=(uint8_t*)MapViewOfFile(map,FILE_MAP_READ,0,0,0);if(!d){CloseHandle(map);CloseHandle(file);e="cannot map view";return false;}handle_=map;data_=d;size_=(uint64_t)n.QuadPart;CloseHandle(file);return true;
#else
int fd=::open(p.c_str(),O_RDONLY);if(fd<0){e="cannot open file";return false;}struct stat st{};if(fstat(fd,&st)!=0){::close(fd);e="cannot stat file";return false;}if(st.st_size==0){::close(fd);e="empty file";return false;}void*d=mmap(nullptr,st.st_size,PROT_READ,MAP_PRIVATE,fd,0);::close(fd);if(d==MAP_FAILED){e="mmap failed";return false;}data_=(uint8_t*)d;size_=st.st_size;return true;
#endif
}
void MappedFile::close(){
#ifdef _WIN32
if(data_)UnmapViewOfFile(data_);if(handle_)CloseHandle((HANDLE)handle_);
#else
if(data_)munmap(data_,size_);
#endif
data_=nullptr;size_=0;handle_=nullptr;}
}
