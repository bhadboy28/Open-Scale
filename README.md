# OpenScale

OpenScale is a cross-platform, speed-first AI runtime foundation. It is designed around a hardware-neutral execution planner, memory-mapped model storage, quantized kernels, cache-aware execution, and optional accelerator backends.

## What is implemented here
- OpenScale `.av` indexed container with embedded source payload and tensor index.
- Safe model inspection for GGUF and safetensors headers.
- Portable memory mapping (Windows and POSIX).
- Backend/module registry with CPU detection and compile-time CUDA/HIP/Metal/Vulkan capability slots.
- CPU hardware detection.
- Graph, model, planner, KV-cache, tokenizer and error abstractions.
- Progress reporting with percentage, throughput, elapsed time and ETA.
- Feasibility analysis for large models.
- Truthful container/memory benchmarks; no fake token/s claims.
- CLI for inspect, convert, verify, benchmark, analyze, modules, plan and run.

## Build on Windows + MSYS2 UCRT64
```powershell
$env:Path += ";C:\Program Files\CMake\bin;C:\msys64\ucrt64\bin"
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

## CLI
```powershell
.\build\openscale.exe hardware
.\build\openscale.exe modules
.\build\openscale.exe inspect model.gguf
.\build\openscale.exe convert model.gguf model.av
.\build\openscale.exe verify model.av
.\build\openscale.exe plan model.av
.\build\openscale.exe benchmark model.av 100
.\build\openscale.exe run model.av --prompt "hello"
```

## Backend status
The backend interface is ready for CPU/CUDA/HIP/Metal/Vulkan implementations. Only the CPU backend is implemented in this package. CUDA/Metal/HIP/Vulkan are **not falsely represented as working inference engines** until their kernels and device memory paths are implemented.

## Performance target
OpenScale is designed to reduce bytes moved, resident memory, and scheduling overhead. A runtime cannot make a trillion dense parameters physically occupy 1 GB or guarantee 100 tok/s on arbitrary hardware. At 4-bit, one trillion parameters require roughly 500 GB of raw weights before metadata and caches. OpenScale therefore targets mmap/offload/quantization/hybrid execution rather than impossible guarantees.

## Next execution milestones
1. GGUF tensor-data importer with complete metadata types.
2. Real transformer graph construction.
3. Fused RMSNorm/RoPE/GQA attention/MLP kernels.
4. INT4/INT8/FP16/BF16 CPU kernels and runtime dispatch.
5. KV-cache paging and prefix caching.
6. CUDA backend, then Metal/HIP/Vulkan.
7. Speculative decoding and measured prefill/decode benchmarks.

OpenScale's `.av` container is a storage/execution format; it is not a magic compression layer. Performance must always be measured on real hardware.
