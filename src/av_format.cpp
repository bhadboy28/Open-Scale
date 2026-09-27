#include "openscale/av_format.hpp"
#include <cstring>
#include <fstream>
#include <limits>
namespace openscale {
static constexpr char MAGIC[8]={'O','P','E','N','A','V','4','\0'};
template<class T> static void put(std::ofstream&f,T v){f.write(reinterpret_cast<const char*>(&v),sizeof(v));}
template<class T> static bool get(std::ifstream&f,T&v){return bool(f.read(reinterpret_cast<char*>(&v),sizeof(v)));}
static void ps(std::ofstream&f,const std::string&s){uint64_t n=s.size();put(f,n);f.write(s.data(),std::streamsize(n));}
static bool gs(std::ifstream&f,std::string&s){uint64_t n{};if(!get(f,n)||n>64ull*1024*1024)return false;s.resize((size_t)n);return bool(f.read(s.data(),std::streamsize(n)));}
std::string dtype_name(DType d){switch(d){case DType::F32:return"F32";case DType::F16:return"F16";case DType::BF16:return"BF16";case DType::I8:return"I8";case DType::I4:return"I4";case DType::I2:return"I2";default:return"UNKNOWN";}}
bool AvWriter::write(const std::filesystem::path&p,const ModelInfo&i,const std::vector<TensorInfo>&ts,const std::filesystem::path&src,std::string&e){
 std::ifstream in(src,std::ios::binary);if(!in){e="cannot open source model";return false;}in.seekg(0,std::ios::end);uint64_t source_size=(uint64_t)in.tellg();in.seekg(0);std::ofstream f(p,std::ios::binary|std::ios::trunc);if(!f){e="cannot create AV";return false;}
 f.write(MAGIC,8);uint32_t ver=4;put(f,ver);uint64_t index=0,payload=0;put(f,index);put(f,payload);ps(f,i.name);ps(f,i.architecture);put(f,i.parameter_count);put(f,i.context_length);put(f,i.vocab_size);put(f,i.layers);put(f,i.hidden_size);put(f,i.attention_heads);put(f,i.kv_heads);ps(f,i.preferred_dtype);ps(f,i.preferred_kernel);ps(f,i.cache_policy);uint64_t mc=i.metadata.size();put(f,mc);for(auto&[k,v]:i.metadata){ps(f,k);ps(f,v);}ps(f,src.filename().string());
 index=(uint64_t)f.tellp();uint64_t n=ts.size();put(f,n);for(auto&t:ts){ps(f,t.name);uint8_t d=(uint8_t)t.dtype;put(f,d);uint64_t nd=t.shape.size();put(f,nd);for(auto x:t.shape)put(f,x);put(f,t.file_offset);put(f,t.byte_size);}
 payload=(uint64_t)f.tellp();put(f,source_size);std::vector<char>buf(1<<20);uint64_t left=source_size;while(left){uint64_t nread=std::min<uint64_t>(left,buf.size());if(!in.read(buf.data(),std::streamsize(nread))){e="source read failed";return false;}f.write(buf.data(),std::streamsize(nread));left-=nread;}
 f.seekp(12);put(f,index);put(f,payload);return bool(f);
}
bool AvReader::open(const std::filesystem::path&p,std::string&e){std::ifstream f(p,std::ios::binary);if(!f){e="cannot open AV";return false;}char m[8]{};if(!f.read(m,8)||memcmp(m,MAGIC,8)){e="invalid OpenScale AV container";return false;}uint32_t v{};uint64_t index{},payload{};if(!get(f,v)||v!=4||!get(f,index)||!get(f,payload)){e="bad AV header";return false;}if(index>=std::filesystem::file_size(p)||payload>=std::filesystem::file_size(p)){e="invalid AV offsets";return false;}
 if(!gs(f,info_.name)||!gs(f,info_.architecture)||!get(f,info_.parameter_count)||!get(f,info_.context_length)||!get(f,info_.vocab_size)||!get(f,info_.layers)||!get(f,info_.hidden_size)||!get(f,info_.attention_heads)||!get(f,info_.kv_heads)||!gs(f,info_.preferred_dtype)||!gs(f,info_.preferred_kernel)||!gs(f,info_.cache_policy)){e="truncated metadata";return false;}uint64_t mc{};if(!get(f,mc)||mc>100000){e="bad metadata";return false;}for(uint64_t n=0;n<mc;n++){std::string k,vv;if(!gs(f,k)||!gs(f,vv)){e="bad metadata";return false;}info_.metadata[k]=vv;}std::string source;if(!gs(f,source)){e="bad source";return false;}uint64_t nt{};if(!get(f,nt)||nt>10000000){e="bad tensor count";return false;}tensors_.clear();tensors_.reserve((size_t)nt);for(uint64_t n=0;n<nt;n++){TensorInfo t;uint8_t d{};uint64_t nd{};if(!gs(f,t.name)||!get(f,d)||d>5||!get(f,nd)||nd>64){e="bad tensor";return false;}t.dtype=(DType)d;t.shape.resize((size_t)nd);for(auto&x:t.shape)if(!get(f,x)){e="bad shape";return false;}if(!get(f,t.file_offset)||!get(f,t.byte_size)){e="bad tensor offsets";return false;}tensors_.push_back(std::move(t));}return true;}
bool AvReader::verify(std::string&e)const{if(tensors_.empty()){e="AV has no tensors";return false;}return true;}
}
