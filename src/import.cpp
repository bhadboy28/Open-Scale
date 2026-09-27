#include "openscale/import.hpp"
#include "openscale/progress.hpp"
#include <fstream>
#include <cstring>
namespace openscale {
static bool u32(std::ifstream&f,uint32_t&x){return bool(f.read((char*)&x,4));}
static bool u64(std::ifstream&f,uint64_t&x){return bool(f.read((char*)&x,8));}
static bool str(std::ifstream&f,std::string&s){uint64_t n{};if(!u64(f,n)||n>64ull*1024*1024)return false;s.resize(n);return bool(f.read(s.data(),n));}
static DType gd(uint32_t x){if(x==0)return DType::F32;if(x==1)return DType::F16;if(x==8||x==9)return DType::I8;return DType::I4;}

static bool skip_val(std::ifstream& f, uint32_t type) {
    if (type == 0 || type == 1 || type == 7) { char b; return bool(f.read(&b, 1)); }
    if (type == 2 || type == 3) { char b[2]; return bool(f.read(b, 2)); }
    if (type == 4 || type == 5 || type == 6) { char b[4]; return bool(f.read(b, 4)); }
    if (type == 10 || type == 11 || type == 12) { char b[8]; return bool(f.read(b, 8)); }
    if (type == 8) { std::string s; return str(f, s); }
    if (type == 9) {
        uint32_t itype{}; uint64_t icount{};
        if (!u32(f, itype) || !u64(f, icount)) return false;
        for (uint64_t j = 0; j < icount; ++j) {
            if (!skip_val(f, itype)) return false;
        }
        return true;
    }
    return false;
}

bool inspect_gguf(const std::filesystem::path&p,ModelInfo&i,std::vector<TensorInfo>&ts,std::string&e){
 std::ifstream f(p,std::ios::binary);if(!f){e="cannot open GGUF";return false;}char m[4]{};if(!f.read(m,4)||memcmp(m,"GGUF",4)){e="not GGUF";return false;}
 uint32_t v{},type{};uint64_t nt{},nk{};if(!u32(f,v)||!u64(f,nt)||!u64(f,nk)||v<1||v>3){e="bad GGUF header";return false;}
 for(uint64_t n=0;n<nk;n++){std::string k;if(!str(f,k)||!u32(f,type)){e="unsupported GGUF metadata";return false;}
   if(type==8){std::string val;if(!str(f,val)){e="bad GGUF string";return false;}if(k=="general.name")i.name=val;else if(k=="general.architecture")i.architecture=val;else i.metadata[k]=val;}
   else if(type==4){uint32_t x;if(!u32(f,x)){e="bad GGUF u32";return false;}if(k.find("context_length")!=std::string::npos)i.context_length=x;else if(k.find("block_count")!=std::string::npos)i.layers=x;else if(k.find("embedding_length")!=std::string::npos)i.hidden_size=x;else if(k.find("head_count_kv")!=std::string::npos)i.kv_heads=x;else if(k.find("head_count")!=std::string::npos)i.attention_heads=x;}
   else if(type==5){int32_t x;if(!f.read((char*)&x,4)){e="bad GGUF i32";return false;}if(k.find("context_length")!=std::string::npos)i.context_length=x;else if(k.find("block_count")!=std::string::npos)i.layers=x;else if(k.find("embedding_length")!=std::string::npos)i.hidden_size=(uint32_t)x;}
   else if(type==10){uint64_t x;if(!u64(f,x)){e="bad GGUF u64";return false;}if(k.find("context_length")!=std::string::npos)i.context_length=x;else if(k.find("block_count")!=std::string::npos)i.layers=x;else if(k.find("embedding_length")!=std::string::npos)i.hidden_size=(uint32_t)x;}
   else if(type==11){int64_t x;if(!f.read((char*)&x,8)){e="bad GGUF i64";return false;}if(k.find("context_length")!=std::string::npos)i.context_length=(uint64_t)x;else if(k.find("block_count")!=std::string::npos)i.layers=(uint32_t)x;else if(k.find("embedding_length")!=std::string::npos)i.hidden_size=(uint32_t)x;}
   else {
       if(!skip_val(f, type)){e="GGUF metadata type skip failed: "+std::to_string(type);return false;}
   }
 }
 ts.reserve((size_t)nt);for(uint64_t n=0;n<nt;n++){TensorInfo t;uint32_t nd{},dt{};uint64_t off{};if(!str(f,t.name)||!u32(f,nd)||nd>64){e="bad GGUF tensor";return false;}t.shape.resize(nd);for(auto&x:t.shape)if(!u64(f,x)){e="bad shape";return false;}if(!u32(f,dt)||!u64(f,off)){e="bad tensor";return false;}t.dtype=gd(dt);t.file_offset=off;ts.push_back(std::move(t));}
 i.metadata["source_format"]="GGUF";return true;
}
bool inspect_safetensors(const std::filesystem::path&p,ModelInfo&i,std::vector<TensorInfo>&ts,std::string&e){
 (void)ts;
 std::ifstream f(p,std::ios::binary);if(!f){e="cannot open safetensors";return false;}uint64_t hs{};if(!u64(f,hs)||hs>256ull*1024*1024){e="bad safetensors header";return false;}std::string j(hs,'\0');if(!f.read(j.data(),hs)){e="truncated header";return false;}
 i.name=p.stem().string();i.architecture="safetensors";i.metadata["source_format"]="safetensors";
 // Full JSON parsing is deliberately deferred to a dedicated parser; never guess tensor byte ranges.
 // This safe inspector still validates the canonical header envelope.
 if(j.empty()||j.front()!='{'){e="invalid safetensors JSON header";return false;}
 e="safetensors header recognized; detailed tensor parsing requires the bundled JSON parser in the next runtime layer";
 return true;
}
bool convert_model_to_av(const std::filesystem::path&in,const std::filesystem::path&out,std::string&e){
 ModelInfo i;std::vector<TensorInfo>ts;Progress p(std::filesystem::file_size(in));p.stage("Inspecting model");
 bool ok=false;if(in.extension()==".gguf")ok=inspect_gguf(in,i,ts,e);else if(in.extension()==".safetensors")ok=inspect_safetensors(in,i,ts,e);else {e="only GGUF/safetensors conversion is enabled";return false;}
 if(!ok) return false;
 p.update(std::filesystem::file_size(in));
 p.stage("Building AV index");
 if(!AvWriter::write(out,i,ts,in,e)) return false;
 p.finish();
 return true;
}
}
