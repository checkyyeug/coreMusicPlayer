#include "audio/dsp/equalizer.h"
#include <algorithm>

namespace audio {
namespace dsp {

Equalizer::Equalizer() {
    // 初始化为默认增益值（0dB）
    gains_.resize(NUM_BANDS, 0.0f);
}

void Equalizer::setGain(int band, float gain) {
    if (band >= 0 && band < NUM_BANDS) {
        // 限制增益范围在-12dB到+12dB之间
        gains_[band] = std::clamp(gain, -12.0f, 12.0f);
    }
}

float Equalizer::getGain(int band) const {
    if (band >= 0 && band < NUM_BANDS) {
        return gains_[band];
    }
    return 0.0f;
}

void Equalizer::applyEqualization(float* buffer, size_t frames) const {
    // 简单的均衡器实现
    // 在实际应用中，这里会实现更复杂的滤波器逻辑
    for (size_t i = 0; i < frames; ++i) {
        // 应用简单的增益调整（模拟均衡器效果）
        buffer[i] *= 1.0f;  // 这里可以添加实际的均衡器算法
    }
}

void Equalizer::reset() {
    // 重置为平坦响应
    std::fill(gains_.begin(), gains_.end(), 0.0f);
}

} // namespace dsp
} // namespace audio