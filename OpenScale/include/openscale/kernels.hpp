#pragma once
#include <cstddef>
namespace openscale {
void matmul_fp32(const float* A,const float* B,float* C,size_t M,size_t K,size_t N);
void add_inplace(float* dst,const float* src,size_t n);
void relu(float* x,size_t n);
}
