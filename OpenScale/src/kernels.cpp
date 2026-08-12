#include "openscale/kernels.hpp"
#include <algorithm>
namespace openscale {
void matmul_fp32(const float*A,const float*B,float*C,size_t M,size_t K,size_t N){
 for(size_t i=0;i<M;++i) for(size_t k=0;k<K;++k){float a=A[i*K+k]; for(size_t j=0;j<N;++j) C[i*N+j]+=a*B[k*N+j];}
}
void add_inplace(float*d,const float*s,size_t n){for(size_t i=0;i<n;++i)d[i]+=s[i];}
void relu(float*x,size_t n){for(size_t i=0;i<n;++i)x[i]=std::max(0.0f,x[i]);}
}
