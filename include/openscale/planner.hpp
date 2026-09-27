#pragma once
#include "backend.hpp"
#include "model.hpp"
namespace openscale { struct ExecutionPlan { BackendKind backend=BackendKind::CPU; uint32_t threads=1; bool mmap_weights=true, paged_kv=true; std::string summary; }; ExecutionPlan plan(const Model&, const std::vector<BackendCaps>&); }
