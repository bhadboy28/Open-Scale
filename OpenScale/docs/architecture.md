# OpenScale Architecture

OpenScale is a runtime layer between models and hardware.

```text
Model -> Graph -> Planner -> Scheduler -> Compute -> CPU/GPU/RAM/Storage
                         |-> Memory Engine
                         |-> Streaming Engine
                         |-> Profiler
                         |-> Resource Governor
```

v0.1 focuses on the native foundation. Future releases add real model graph execution, quantization, asynchronous weight streaming, KV-cache management, GPU backends, and adaptive resource control.
