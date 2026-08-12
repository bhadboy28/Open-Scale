#include <iostream>
#include "openscale/tensor.hpp"
#include "openscale/kernels.hpp"
#include "openscale/profiler.hpp"
int main(){const size_t N=512;openscale::Tensor A({N,N}),B({N,N}),C({N,N});A.fill(0.01f);B.fill(0.02f);C.fill(0);openscale::Profiler p;p.start();openscale::matmul_fp32(A.data(),B.data(),C.data(),N,N,N);double s=p.stop_seconds();double ops=2.0*N*N*N;std::cout<<"OpenScale benchmark\nMatrix: "<<N<<"x"<<N<<"\nTime: "<<s<<" s\nGFLOP/s: "<<ops/s/1e9<<"\nResult: "<<C.data()[0]<<"\n";}
