#include "openscale/tensor.hpp"
#include <numeric>
#include <functional>

namespace openscale {
size_t dtype_size(DType dtype) { return dtype == DType::FP32 ? sizeof(float) : sizeof(int8_t); }
Tensor::Tensor() = default;
Tensor::Tensor(std::vector<size_t> shape, DType dtype): shape_(std::move(shape)), dtype_(dtype) {
    if (dtype_ != DType::FP32) throw std::runtime_error("v0.1 Tensor storage currently supports FP32 only");
    fp32_.resize(numel());
}
size_t Tensor::numel() const { if(shape_.empty()) return 0; return std::accumulate(shape_.begin(),shape_.end(),size_t{1},std::multiplies<size_t>()); }
size_t Tensor::bytes() const { return fp32_.size()*sizeof(float); }
const std::vector<size_t>& Tensor::shape() const { return shape_; }
DType Tensor::dtype() const { return dtype_; }
float* Tensor::data() { return fp32_.data(); }
const float* Tensor::data() const { return fp32_.data(); }
void Tensor::fill(float value) { std::fill(fp32_.begin(),fp32_.end(),value); }
}
