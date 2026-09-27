#include "openscale/av_format.hpp"
#include "openscale/feasibility.hpp"
#include "openscale/kv_cache.hpp"
#include "openscale/modules.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
int main(){using namespace openscale;auto src=std::filesystem::temp_directory_path()/"openscale_fixture.bin";{std::ofstream f(src,std::ios::binary);std::string x(4096,'x');f.write(x.data(),x.size());}auto p=std::filesystem::temp_directory_path()/"openscale.av";ModelInfo i;i.name="test";i.layers=2;i.hidden_size=128;i.parameter_count=1000000;TensorInfo t;t.name="x";t.dtype=DType::I4;t.shape={128,128};t.byte_size=8192;std::string e;assert(AvWriter::write(p,i,{t},src,e));AvReader r;assert(r.open(p,e)&&r.verify(e));assert(r.tensors().size()==1);auto f=analyze(i,1.0,4.0);assert(f.weight_gb>0);KVCache kv;assert(kv.init({2,2,64,8,2}));assert(kv.bytes()>0);assert(!list_modules().empty());std::filesystem::remove(p);std::filesystem::remove(src);std::cout<<"OpenScale core test: PASS\n";}
