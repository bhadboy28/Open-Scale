# Runtime research

OpenScale's architecture is informed by public documentation from established runtimes.

ONNX Runtime uses execution providers with capability discovery and graph partitioning, allowing different graph regions to run on different hardware. It also supports offline graph optimization and kernel fusion. llama.cpp documents GGUF quantization, importance matrices, and the accuracy/speed tradeoff of reduced precision.

OpenScale uses these as design references while keeping its own implementation and APIs.

References:
- https://onnxruntime.ai/docs/execution-providers/
- https://onnxruntime.ai/docs/reference/high-level-design.html
- https://onnxruntime.ai/docs/performance/model-optimizations/graph-optimizations.html
- https://github.com/ggml-org/llama.cpp/blob/master/tools/quantize/README.md
