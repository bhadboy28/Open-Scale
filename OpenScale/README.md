# OpenScale

OpenScale is a performance-first, cross-platform runtime foundation for efficient model storage and execution. It combines memory mapping, hardware discovery, cache-aware CPU math, a checksummed `.av` container, and a modular backend architecture.

## Current implemented core
- C++20/CMake project
- `.av` header/metadata/payload format with checksum validation
- memory-mapped loading on Windows/POSIX
- hardware discovery
- cache-aware tiled, multithreaded FP32 matrix multiplication
- structured error codes
- CLI: `doctor`, `benchmark`, `info`, `validate`
- unit test

## Build on Windows/MSYS2
```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/msys64/ucrt64/bin/mingw32-make.exe"
cmake --build build
ctest --test-dir build --output-on-failure
.\build\openscale.exe doctor
.\build\openscale.exe benchmark
```

## Architecture direction
The design separates the container, memory manager, planner, math kernels, and hardware backends. This follows the useful execution-provider/capability pattern documented by ONNX Runtime and the quantization/performance lessons documented by llama.cpp/GGUF. See `docs/research.md`.

OpenScale does not claim that software can make a trillion-parameter model physically fit in 1 GB or guarantee a fixed tokens/sec rate. Out-of-core streaming can reduce resident memory, but storage bandwidth and compute remain physical limits.
