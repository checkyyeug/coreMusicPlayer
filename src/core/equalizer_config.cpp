#include "core/equalizer_config.h"
#include <algorithm>

namespace core {

EqualizerConfig::EqualizerConfig() : name_("Default") {
    // 初始化为默认增益值（0dB）
    gains_.resize(NUM_BANDS, 0.0f);
}

float EqualizerConfig::getGain(int band) const {
    if (band >= 0 && band < NUM_BANDS) {
        return gains_[band];
    }
    return 0.0f;
}

void EqualizerConfig::setGain(int band, float gain) {
    if (band >= 0 && band < NUM_BANDS) {
        // 限制增益范围在-12dB到+12dB之间
        gains_[band] = std::clamp(gain, -12.0f, 12.0f);
    }
}

std::vector<float> EqualizerConfig::getAllGains() const {
    return gains_;
}

void EqualizerConfig::setAllGains(const std::vector<float>& gains) {
    if (gains.size() == NUM_BANDS) {
        for (int i = 0; i < NUM_BANDS; ++i) {
            setGain(i, gains[i]);
        }
    }
}

void EqualizerConfig::resetToDefault() {
    // 重置为平坦响应
    std::fill(gains_.begin(), gains_.end(), 0.0f);
    name_ = "Default";
}

std::string EqualizerConfig::getName() const {
    return name_;
}

void EqualizerConfig::setName(const std::string& name) {
    name_ = name;
}

EqualizerConfig EqualizerConfig::getPresets(const std::string& presetName) {
    EqualizerConfig config;
    
    if (presetName == "Rock") {
        // Rock风格均衡器
        config.setName("Rock");
        config.setGain(0, -2.0f);  // 31Hz
        config.setGain(1, -1.0f);  // 62Hz
        config.setGain(2, 1.0f);   // 125Hz
        config.setGain(3, 2.0f);   // 250Hz
        config.setGain(4, 3.0f);   // 500Hz
        config.setGain(5, 2.0f);   // 1kHz
        config.setGain(6, 1.0f);   // 2kHz
        config.setGain(7, -1.0f);  // 4kHz
        config.setGain(8, -2.0f);  // 8kHz
        config.setGain(9, -3.0f);  // 16kHz
    } else if (presetName == "Jazz") {
        // Jazz风格均衡器
        config.setName("Jazz");
        config.setGain(0, -1.0f);  // 31Hz
        config.setGain(1, 0.0f);   // 62Hz
        config.setGain(2, 1.0f);   // 125Hz
        config.setGain(3, 2.0f);   // 250Hz
        config.setGain(4, 1.0f);   // 500Hz
        config.setGain(5, 0.0f);   // 1kHz
        config.setGain(6, -1.0f);  // 2kHz
        config.setGain(7, -2.0f);  // 4kHz
        config.setGain(8, -1.0f);  // 8kHz
        config.setGain(9, 0.0f);   // 16kHz
    } else if (presetName == "Classical") {
        // Classical风格均衡器
        config.setName("Classical");
        config.setGain(0, -2.0f);  // 31Hz
        config.setGain(1, -1.0f);  // 62Hz
        config.setGain(2, 0.0f);   // 125Hz
        config.setGain(3, 1.0f);   // 250Hz
        config.setGain(4, 2.0f);   // 500Hz
        config.setGain(5, 1.0f);   // 1kHz
        config.setGain(6, 0.0f);   // 2kHz
        config.setGain(7, -1.0f);  // 4kHz
        config.setGain(8, -2.0f);  // 8kHz
        config.setGain(9, -3.0f);  // 16kHz
    } else {
        // 默认平坦响应
        config.resetToDefault();
    }
    
    return config;
}

} // namespace core