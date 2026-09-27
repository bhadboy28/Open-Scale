# OpenScale architecture

```text
.av / GGUF / SafeTensors
        |
     Loader
        |
   Model + Graph
        |
 Optimization / Planner
        |
   Backend dispatch
   /      |       \
 CPU    CUDA     Metal/HIP/Vulkan
   \      |       /
    Kernel engine
        |
 Memory + KV cache
        |
     Decoder
```

## Design principles
- The core runtime never depends on a single GPU vendor.
- Hardware-specific kernels live behind a backend interface.
- Model data can be memory mapped instead of eagerly copied.
- Large-model execution should minimize resident working sets and support future paging/offload.
- Benchmarks must measure actual work; metadata scans are never reported as token generation.
- Errors are explicit and model files are never executed as pickle payloads.

## `.av` layout
Header -> model manifest -> metadata -> tensor index -> embedded source payload.
Tensor offsets remain source-format offsets and the embedded payload preserves the original model bytes, allowing future loaders to access the original data without requiring the source file to remain beside the `.av` file.
