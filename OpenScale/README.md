# OpenScale

OpenScale is an experimental hardware-aware AI compute runtime focused on efficient inference on constrained systems.

It is a runtime/library, not a model. The v0.1 implementation provides a native C++ core, tensor abstraction, memory pool, CPU device detection, scheduler, profiler, matrix multiplication, memory-mapped files, and a small Python API.

OpenScale does not magically make a 120B model fit into 4 GB RAM. Its long-term research direction is to reduce active working memory through quantization, streaming, paging, caching, graph optimization, and adaptive scheduling.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Run:

```bash
./build/openscale_demo
./build/openscale-benchmark
```

On Windows, use the corresponding `.exe` files.
