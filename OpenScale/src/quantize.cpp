#include "openscale/openscale.hpp"
#include <cmath>
#include <algorithm>

namespace openscale::math {

void quantize_q8_0(std::span<const float> src, std::span<BlockQ8> dst) {
    size_t num_blocks = src.size() / 32;
    for (size_t i = 0; i < num_blocks; ++i) {
        float max_val = 0.0f;
        for (int j = 0; j < 32; ++j) {
            float v = std::abs(src[i * 32 + j]);
            if (v > max_val) max_val = v;
        }
        
        float d = max_val / 127.0f;
        dst[i].d = d;
        float id = d != 0.0f ? 1.0f / d : 0.0f;
        
        for (int j = 0; j < 32; ++j) {
            float v = src[i * 32 + j] * id;
            dst[i].qs[j] = static_cast<int8_t>(std::round(v));
        }
    }
}

} // namespace openscale::math
