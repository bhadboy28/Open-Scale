#pragma once
#include "av_format.hpp"
#include "graph.hpp"
#include "error.hpp"
#include <filesystem>
namespace openscale {
class Model { public: bool load(const std::filesystem::path&,std::string&); const ModelInfo& info() const{return info_;} const std::vector<TensorInfo>& tensors() const{return tensors_;} const Graph& graph() const{return graph_;} bool loaded() const{return loaded_;} private: ModelInfo info_; std::vector<TensorInfo> tensors_; Graph graph_; bool loaded_=false; };
}
