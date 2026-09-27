#pragma once
#include "av_format.hpp"
namespace openscale {
bool inspect_gguf(const std::filesystem::path&, ModelInfo&, std::vector<TensorInfo>&, std::string&);
bool inspect_safetensors(const std::filesystem::path&, ModelInfo&, std::vector<TensorInfo>&, std::string&);
bool convert_model_to_av(const std::filesystem::path&, const std::filesystem::path&, std::string&);
}
