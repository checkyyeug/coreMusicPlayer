#include "volume_control.h"
#include <algorithm>
#include <cmath>

namespace audio {
namespace dsp {

VolumeControl::VolumeControl() 
    : linear_volume_(1.0f),
      logarithmic_volume_(0.0f),
      no_distortion_enabled_(false) {
}

VolumeControl::~VolumeControl() = default;

void VolumeControl::setLinearVolume(float volume) {
    // 限制音量范围
    linear_volume_ = std::max(0.0f, std::min(1.0f, volume));
    
    // 更新对数音量值
    logarithmic_volume_ = linearToDb(linear_volume_);
}

void VolumeControl::setLogarithmicVolume(float dB) {
    // 限制分贝范围
    dB = std::max(DB_MIN, std::min(DB_MAX, dB));
    
    // 转换为线性音量值
    linear_volume_ = dbToLinear(dB);
    
    // 更新对数音量值
    logarithmic_volume_ = dB;
}

float VolumeControl::getLinearVolume() const {
    return linear_volume_;
}

float VolumeControl::getLogarithmicVolume() const {
    return logarithmic_volume_;
}

void VolumeControl::applyVolume(float* data, size_t size, int channels) {
    if (!data || size == 0 || channels <= 0) {
        return;
    }
    
    // 如果启用了无失真控制，需要检查是否超出范围
    if (no_distortion_enabled_) {
        for (size_t i = 0; i < size * channels; ++i) {
            data[i] = std::max(-MAX_LINEAR_VOLUME, std::min(MAX_LINEAR_VOLUME, data[i] * linear_volume_));
        }
    } else {
        // 直接应用音量
        for (size_t i = 0; i < size * channels; ++i) {
            data[i] *= linear_volume_;
        }
    }
}

void VolumeControl::applyVolume(int16_t* data, size_t size, int channels) {
    if (!data || size == 0 || channels <= 0) {
        return;
    }
    
    // 如果启用了无失真控制，需要检查是否超出范围
    if (no_distortion_enabled_) {
        const float max_value = static_cast<float>(32767);
        for (size_t i = 0; i < size * channels; ++i) {
            float sample = static_cast<float>(data[i]) / max_value;
            sample *= linear_volume_;
            sample = std::max(-MAX_LINEAR_VOLUME, std::min(MAX_LINEAR_VOLUME, sample));
            data[i] = static_cast<int16_t>(sample * max_value);
        }
    } else {
        // 直接应用音量
        const float max_value = static_cast<float>(32767);
        for (size_t i = 0; i < size * channels; ++i) {
            float sample = static_cast<float>(data[i]) / max_value;
            sample *= linear_volume_;
            data[i] = static_cast<int16_t>(sample * max_value);
        }
    }
}

void VolumeControl::setNoDistortion(bool enabled) {
    no_distortion_enabled_ = enabled;
}

bool VolumeControl::isNoDistortionEnabled() const {
    return no_distortion_enabled_;
}

float VolumeControl::dbToLinear(float dB) const {
    // 将分贝转换为线性值
    return std::pow(10.0f, dB / 20.0f);
}

float VolumeControl::linearToDb(float linear) const {
    // 将线性值转换为分贝
    if (linear <= 0.0f) {
        return DB_MIN;
    }
    return 20.0f * std::log10(linear);
}

} // namespace dsp
} // namespace audio