# Architecture

Container -> memory map/pager -> execution planner -> backend capability selection -> kernels -> telemetry.

The long-term backend interface is designed to support CPU SIMD, CUDA, HIP, Vulkan and Metal without coupling the core runtime to one vendor. Large models can use out-of-core storage and a working-set cache; the runtime should measure whether storage or compute is the bottleneck.
