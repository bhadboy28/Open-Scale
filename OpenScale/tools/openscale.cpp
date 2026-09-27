#include "openscale/openscale.hpp"
#include <iostream>
#include <vector>
#include <chrono>
using namespace openscale;
int main(int argc,char**argv){
    std::cout<<"OpenScale\n";
    if(argc<2){std::cout<<"doctor | benchmark | info <file.av> | validate <file.av>\n";return 0;}
    std::string c=argv[1];
    if(c=="doctor"){
        auto h=detect_hardware();
        std::cout<<"threads: "<<h.threads<<"\nRAM: "<<h.ram_bytes/(1024*1024)<<" MiB\nAVX2: "<<h.avx2<<" AVX512: "<<h.avx512<<" NEON: "<<h.neon<<"\n";
        return 0;
    }
    if(c=="benchmark"){
        size_t M_burst = 64, K_burst = 64, N_burst = 64;
        std::vector<float> A_burst(M_burst*K_burst,.01f), B_burst(K_burst*N_burst,.02f), C_burst(M_burst*N_burst);
        std::cout << "Testing FP32 L1 Cache Burst (Physical Hardware Limit)...\n";
        auto t_burst=std::chrono::steady_clock::now();
        // Loop 10,000 times, run single-threaded to avoid thread spawn overhead
        for(int i = 0; i < 10000; ++i) {
            math::matmul_f32(A_burst, B_burst, C_burst, M_burst, K_burst, N_burst, 1);
        }
        double s_burst=std::chrono::duration<double>(std::chrono::steady_clock::now()-t_burst).count();
        double total_flops = 2.0 * M_burst * K_burst * N_burst * 10000;
        std::cout<<"FP32 L1 Burst GFLOP/s (Single Core): "<<(total_flops/s_burst/1e9)<<"\n";
        
        // Multi-core burst estimation (Assuming linear scaling across all physical cores)
        auto h = detect_hardware();
        std::cout<<"FP32 Estimated Multi-Core Peak GFLOP/s: "<<(total_flops/s_burst/1e9 * h.threads)<<"\n\n";

        size_t M=1,K=4096,N=4096;
        std::vector<float> A(M*K,.01f), B(K*N,.02f), C(M*N);
        std::cout << "Testing FP32 Engine...\n";
        auto t=std::chrono::steady_clock::now();
        math::matmul_f32(A,B,C,M,K,N);
        double s=std::chrono::duration<double>(std::chrono::steady_clock::now()-t).count();
        std::cout<<"FP32 GFLOP/s: "<<(2.0*M*K*N/s/1e9)<<"\n";

        std::cout << "Testing Q8_0 Engine...\n";
        // B is transposed for Q8_0
        std::vector<float> BT(N*K, .02f);
        std::vector<math::BlockQ8> QA(M * K / 32), QB(N * K / 32);
        math::quantize_q8_0(A, QA);
        math::quantize_q8_0(BT, QB);
        
        t=std::chrono::steady_clock::now();
        math::matmul_q8_0(QA, QB, C, M, K, N);
        double s_q8=std::chrono::duration<double>(std::chrono::steady_clock::now()-t).count();
        std::cout<<"Q8_0 GFLOP/s: "<<(2.0*M*K*N/s_q8/1e9)<<"\n";
        std::cout<<"Speedup: "<<(s / s_q8)<<"x\n";
        
        return 0;
    }
    if(argc<3)return 2;
    Error e;
    if(c=="validate"){e=AvFile::validate(argv[2]);}
    else if(c=="info"){
        auto m=AvFile::inspect(argv[2],e);
        if(m)std::cout<<"name: "<<m->name<<"\narchitecture: "<<m->architecture<<"\nparameters: "<<m->parameter_count<<"\ntensors: "<<m->tensors.size()<<"\nbytes: "<<m->tensor_bytes<<"\n";
    }else return 2;
    if(e){std::cerr<<error_name(e.code)<<": "<<e.message<<"\n";return 1;}
    if(c=="validate")std::cout<<"valid AV file\n";
    return 0;
}
