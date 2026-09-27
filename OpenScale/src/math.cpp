#include "openscale/openscale.hpp"
#include <algorithm>
#include <thread>
#include <immintrin.h>
#include <vector>

namespace openscale::math {

double dot_f32(std::span<const float> a, std::span<const float> b) {
    size_t n = std::min(a.size(), b.size());
    size_t i = 0;
    __m256 sum256 = _mm256_setzero_ps();
    for (; i + 7 < n; i += 8) {
        sum256 = _mm256_fmadd_ps(_mm256_loadu_ps(a.data() + i), _mm256_loadu_ps(b.data() + i), sum256);
    }
    float sum_arr[8];
    _mm256_storeu_ps(sum_arr, sum256);
    double s = 0;
    for (int j = 0; j < 8; ++j) s += sum_arr[j];
    for (; i < n; ++i) s += double(a[i]) * b[i];
    return s;
}

void matmul_f32(std::span<const float> A, std::span<const float> B, std::span<float> C, 
                size_t M, size_t K, size_t N, unsigned threads) {
    if (threads == 0) threads = std::max(1u, std::thread::hardware_concurrency());
    threads = std::min<unsigned>(threads, std::max<size_t>(1, M));
    
    std::fill(C.begin(), C.end(), 0.0f);
    
    auto work = [&](size_t r0, size_t r1) {
        // L1 Cache blocking
        constexpr size_t BK = 256;
        constexpr size_t BN = 192; // Multiple of 24

        for (size_t kk = 0; kk < K; kk += BK) {
            size_t k_end = std::min(K, kk + BK);
            for (size_t jj = 0; jj < N; jj += BN) {
                size_t j_end = std::min(N, jj + BN);
                
                size_t i = r0;
                // 4x24 Micro-Kernel
                for (; i + 3 < r1; i += 4) {
                    size_t j = jj;
                    for (; j + 23 < j_end; j += 24) {
                        __m256 c00 = _mm256_loadu_ps(&C[(i+0)*N + j + 0]);
                        __m256 c01 = _mm256_loadu_ps(&C[(i+0)*N + j + 8]);
                        __m256 c02 = _mm256_loadu_ps(&C[(i+0)*N + j + 16]);
                        
                        __m256 c10 = _mm256_loadu_ps(&C[(i+1)*N + j + 0]);
                        __m256 c11 = _mm256_loadu_ps(&C[(i+1)*N + j + 8]);
                        __m256 c12 = _mm256_loadu_ps(&C[(i+1)*N + j + 16]);
                        
                        __m256 c20 = _mm256_loadu_ps(&C[(i+2)*N + j + 0]);
                        __m256 c21 = _mm256_loadu_ps(&C[(i+2)*N + j + 8]);
                        __m256 c22 = _mm256_loadu_ps(&C[(i+2)*N + j + 16]);
                        
                        __m256 c30 = _mm256_loadu_ps(&C[(i+3)*N + j + 0]);
                        __m256 c31 = _mm256_loadu_ps(&C[(i+3)*N + j + 8]);
                        __m256 c32 = _mm256_loadu_ps(&C[(i+3)*N + j + 16]);

                        for (size_t k = kk; k < k_end; ++k) {
                            __m256 b0 = _mm256_loadu_ps(&B[k*N + j + 0]);
                            __m256 b1 = _mm256_loadu_ps(&B[k*N + j + 8]);
                            __m256 b2 = _mm256_loadu_ps(&B[k*N + j + 16]);

                            __m256 a0 = _mm256_set1_ps(A[(i+0)*K + k]);
                            c00 = _mm256_fmadd_ps(a0, b0, c00);
                            c01 = _mm256_fmadd_ps(a0, b1, c01);
                            c02 = _mm256_fmadd_ps(a0, b2, c02);

                            __m256 a1 = _mm256_set1_ps(A[(i+1)*K + k]);
                            c10 = _mm256_fmadd_ps(a1, b0, c10);
                            c11 = _mm256_fmadd_ps(a1, b1, c11);
                            c12 = _mm256_fmadd_ps(a1, b2, c12);

                            __m256 a2 = _mm256_set1_ps(A[(i+2)*K + k]);
                            c20 = _mm256_fmadd_ps(a2, b0, c20);
                            c21 = _mm256_fmadd_ps(a2, b1, c21);
                            c22 = _mm256_fmadd_ps(a2, b2, c22);

                            __m256 a3 = _mm256_set1_ps(A[(i+3)*K + k]);
                            c30 = _mm256_fmadd_ps(a3, b0, c30);
                            c31 = _mm256_fmadd_ps(a3, b1, c31);
                            c32 = _mm256_fmadd_ps(a3, b2, c32);
                        }
                        
                        _mm256_storeu_ps(&C[(i+0)*N + j + 0], c00);
                        _mm256_storeu_ps(&C[(i+0)*N + j + 8], c01);
                        _mm256_storeu_ps(&C[(i+0)*N + j + 16], c02);
                        
                        _mm256_storeu_ps(&C[(i+1)*N + j + 0], c10);
                        _mm256_storeu_ps(&C[(i+1)*N + j + 8], c11);
                        _mm256_storeu_ps(&C[(i+1)*N + j + 16], c12);
                        
                        _mm256_storeu_ps(&C[(i+2)*N + j + 0], c20);
                        _mm256_storeu_ps(&C[(i+2)*N + j + 8], c21);
                        _mm256_storeu_ps(&C[(i+2)*N + j + 16], c22);
                        
                        _mm256_storeu_ps(&C[(i+3)*N + j + 0], c30);
                        _mm256_storeu_ps(&C[(i+3)*N + j + 8], c31);
                        _mm256_storeu_ps(&C[(i+3)*N + j + 16], c32);
                    }
                    
                    // Tail 1x8
                    for (; j + 7 < j_end; j += 8) {
                        __m256 c0 = _mm256_loadu_ps(&C[(i+0)*N + j]);
                        __m256 c1 = _mm256_loadu_ps(&C[(i+1)*N + j]);
                        __m256 c2 = _mm256_loadu_ps(&C[(i+2)*N + j]);
                        __m256 c3 = _mm256_loadu_ps(&C[(i+3)*N + j]);
                        for (size_t k = kk; k < k_end; ++k) {
                            __m256 b0 = _mm256_loadu_ps(&B[k*N + j]);
                            c0 = _mm256_fmadd_ps(_mm256_set1_ps(A[(i+0)*K+k]), b0, c0);
                            c1 = _mm256_fmadd_ps(_mm256_set1_ps(A[(i+1)*K+k]), b0, c1);
                            c2 = _mm256_fmadd_ps(_mm256_set1_ps(A[(i+2)*K+k]), b0, c2);
                            c3 = _mm256_fmadd_ps(_mm256_set1_ps(A[(i+3)*K+k]), b0, c3);
                        }
                        _mm256_storeu_ps(&C[(i+0)*N + j], c0);
                        _mm256_storeu_ps(&C[(i+1)*N + j], c1);
                        _mm256_storeu_ps(&C[(i+2)*N + j], c2);
                        _mm256_storeu_ps(&C[(i+3)*N + j], c3);
                    }
                    // Tail scalar
                    for (; j < j_end; ++j) {
                        float c0=C[(i+0)*N+j], c1=C[(i+1)*N+j], c2=C[(i+2)*N+j], c3=C[(i+3)*N+j];
                        for (size_t k = kk; k < k_end; ++k) {
                            float b0 = B[k*N+j];
                            c0 += A[(i+0)*K+k] * b0;
                            c1 += A[(i+1)*K+k] * b0;
                            c2 += A[(i+2)*K+k] * b0;
                            c3 += A[(i+3)*K+k] * b0;
                        }
                        C[(i+0)*N+j]=c0; C[(i+1)*N+j]=c1; C[(i+2)*N+j]=c2; C[(i+3)*N+j]=c3;
                    }
                }
                
                // Tail i
                for (; i < r1; ++i) {
                    size_t j = jj;
                    // 1x8 Tail
                    for (; j + 7 < j_end; j += 8) {
                        __m256 c0 = _mm256_loadu_ps(&C[i*N + j]);
                        for (size_t k = kk; k < k_end; ++k) {
                            c0 = _mm256_fmadd_ps(_mm256_set1_ps(A[i*K+k]), _mm256_loadu_ps(&B[k*N+j]), c0);
                        }
                        _mm256_storeu_ps(&C[i*N + j], c0);
                    }
                    // Scalar tail
                    for (; j < j_end; ++j) {
                        float c0 = C[i*N+j];
                        for (size_t k = kk; k < k_end; ++k) c0 += A[i*K+k] * B[k*N+j];
                        C[i*N+j] = c0;
                    }
                }
            }
        }
    };

    if (threads == 1) {
        work(0, M);
    } else {
        std::vector<std::thread> ts;
        for (unsigned t = 0; t < threads; ++t) {
            size_t start = M * t / threads;
            size_t end = M * (t + 1) / threads;
            ts.emplace_back(work, start, end);
        }
        for (auto& t : ts) t.join();
    }
}

void matmul_q8_0(std::span<const BlockQ8> A, std::span<const BlockQ8> B, std::span<float> C, 
                 size_t M, size_t K, size_t N, unsigned threads) {
    if (threads == 0) threads = std::max(1u, std::thread::hardware_concurrency());
    threads = std::min<unsigned>(threads, std::max<size_t>(1, M));
    
    std::fill(C.begin(), C.end(), 0.0f);
    size_t K_blocks = K / 32;

    auto work = [&](size_t r0, size_t r1) {
        for (size_t i = r0; i < r1; ++i) {
            for (size_t j = 0; j < N; ++j) {
                __m256i sum_i32 = _mm256_setzero_si256();
                __m256 sum_f32 = _mm256_setzero_ps();
                
                for (size_t k = 0; k < K_blocks; ++k) {
                    const BlockQ8* a = &A[i * K_blocks + k];
                    const BlockQ8* b = &B[j * K_blocks + k]; // Assuming B is transposed for benchmark simplicity
                    
                    __m256i ax = _mm256_loadu_si256((const __m256i*)a->qs);
                    __m256i bx = _mm256_loadu_si256((const __m256i*)b->qs);
                    
                    __m256i sign = _mm256_sign_epi8(bx, ax);
                    __m256i abs_a = _mm256_abs_epi8(ax);
                    
                    __m256i m16 = _mm256_maddubs_epi16(abs_a, sign);
                    __m256i m32 = _mm256_madd_epi16(m16, _mm256_set1_epi16(1));
                    
                    // Accumulate integer dot product for this block
                    // Wait, we need to multiply by scales a->d * b->d immediately to avoid overflow if we sum many blocks
                    // but for a single block of 32 it's safe.
                    // Actually, we can sum integers over a few blocks, but to be simple we convert to float and multiply block scale
                    
                    int32_t sum_arr[8];
                    _mm256_storeu_si256((__m256i*)sum_arr, m32);
                    int32_t block_sum = 0;
                    for(int x=0; x<8; ++x) block_sum += sum_arr[x];
                    
                    C[i * N + j] += block_sum * (a->d * b->d);
                }
            }
        }
    };

    std::vector<std::thread> ts;
    for (unsigned t = 0; t < threads; ++t) {
        size_t start = M * t / threads;
        size_t end = M * (t + 1) / threads;
        ts.emplace_back(work, start, end);
    }
    for (auto& t : ts) t.join();
}

} // namespace openscale::math
