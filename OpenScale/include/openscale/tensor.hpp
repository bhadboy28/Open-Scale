#pragma once
#include <cstddef>
#include <vector>
#include <stdexcept>

namespace openscale {

enum class DType { FP32, INT8 };

size_t dtype_size(DType dtype);

class Tensor {
public:
    Tensor();
    explicit Tensor(std::vector<size_t> shape, DType dtype = DType::FP32);
    size_t numel() const;
    size_t bytes() const;
    const std::vector<size_t>& shape() const;
    DType dtype() const;
    float* data();
    const float* data() const;
    void fill(float value);
private:
    std::vector<size_t> shape_;
    DType dtype_{DType::FP32};
    std::vector<float> fp32_;
};

}
