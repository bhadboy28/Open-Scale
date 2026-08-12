#include "openscale/mmap.hpp"
#include <fstream>
#include <vector>
#include <cstring>
namespace openscale {
MappedFile::MappedFile():data_(nullptr),size_(0),fd_(-1){}
MappedFile::~MappedFile(){close();}
bool MappedFile::open(const std::string& path){ close(); std::ifstream f(path,std::ios::binary|std::ios::ate); if(!f)return false; auto n=f.tellg(); if(n<0)return false; size_=static_cast<size_t>(n); if(size_){void*p=std::malloc(size_); if(!p){size_=0;return false;} f.seekg(0); f.read(static_cast<char*>(p),size_); if(!f){std::free(p);data_=nullptr;size_=0;return false;} data_=p;} return true; }
void MappedFile::close(){if(data_)std::free(data_);data_=nullptr;size_=0;}
const void* MappedFile::data()const{return data_;} size_t MappedFile::size()const{return size_;} bool MappedFile::valid()const{return data_||size_==0;}
}
