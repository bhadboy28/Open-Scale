#include <cassert>
#include "openscale/runtime.hpp"
#include "openscale/tensor.hpp"
#include "openscale/kernels.hpp"
int main(){openscale::Runtime r;assert(r.device().cpu_threads>0);openscale::Tensor a({2,3});assert(a.numel()==6);a.fill(2);assert(a.data()[0]==2);openscale::Tensor A({2,2}),B({2,2}),C({2,2});A.fill(1);B.fill(2);C.fill(0);openscale::matmul_fp32(A.data(),B.data(),C.data(),2,2,2);assert(C.data()[0]==4);return 0;}
