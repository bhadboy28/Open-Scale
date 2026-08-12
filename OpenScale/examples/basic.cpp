#include <iostream>
#include "openscale/runtime.hpp"
#include "openscale/tensor.hpp"
#include "openscale/kernels.hpp"
#include "openscale/profiler.hpp"
int main(){
 openscale::Runtime::Config c; c.memory_limit_bytes=512ull*1024*1024; c.workers=4; openscale::Runtime r(c);
 auto d=r.device(); std::cout<<"OpenScale v0.1\nArchitecture: "<<d.architecture<<"\nThreads: "<<d.cpu_threads<<"\nAVX2: "<<d.avx2<<"\nWorkers: "<<r.scheduler().workers()<<"\n";
 openscale::Tensor A({128,128}),B({128,128}),C({128,128}); A.fill(1);B.fill(0.5f);C.fill(0);
 openscale::Profiler p;p.start();openscale::matmul_fp32(A.data(),B.data(),C.data(),128,128,128);double t=p.stop_seconds();
 std::cout<<"Matmul seconds: "<<t<<"\nC[0]: "<<C.data()[0]<<"\n";
}
