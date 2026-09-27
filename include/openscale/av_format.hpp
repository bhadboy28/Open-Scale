#pragma once
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace openscale {
enum class DType : uint8_t { F32, F16, BF16, I8, I4, I2, UNKNOWN=255 };

struct TensorInfo {
    std::string name;
    DType dtype{DType::UNKNOWN};
    std::vector<uint64_t> shape;
    uint64_t file_offset{};
    uint64_t byte_size{};
};

struct ModelInfo {
    std::string name, architecture;
    uint64_t parameter_count{}, context_length{}, vocab_size{};
    uint32_t layers{}, hidden_size{}, attention_heads{}, kv_heads{};
    std::string preferred_dtype{"auto"};
    std::string preferred_kernel{"auto"};
    std::string cache_policy{"hot-kv-stream-weights"};
    std::unordered_map<std::string,std::string> metadata;
};

class AvWriter {
public:
    static bool write(const std::filesystem::path&, const ModelInfo&,
                      const std::vector<TensorInfo>&,
                      const std::filesystem::path&, std::string&);
};

class AvReader {
    std::filesystem::path path_;
    ModelInfo info_;
    std::vector<TensorInfo> tensors_;
public:
    bool open(const std::filesystem::path&, std::string&);
    bool verify(std::string&) const;
    const ModelInfo& info() const { return info_; }
    const std::vector<TensorInfo>& tensors() const { return tensors_; }
};
std::string dtype_name(DType);
}
