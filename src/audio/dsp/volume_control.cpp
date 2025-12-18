#include "audio/dsp/volume_control.h"
#include <algorithm>

namespace audio {
namespace dsp {

VolumeControl::VolumeControl() : volume_(1.0f), muted_(false) {}

void VolumeControl::setVolume(float volume) {
    // 限制音量范围在0.0到2.0之间
    volume_ = std::clamp(volume, 0.0f, 2.0f);
}

float VolumeControl::getVolume() const {
    return volume_;
}

void VolumeControl::applyVolume(float* buffer, size_t frames) const {
    if (muted_) {
        // 如果静音，将所有样本设为0
        for (size_t i = 0; i < frames; ++i) {
            buffer[i] = 0.0f;
        }
    } else {
        // 应用音量控制
        for (size_t i = 0; i < frames; ++i) {
            buffer[i] *= volume_;
        }
    }
}

void VolumeControl::mute() {
    muted_ = true;
}

void VolumeControl::unmute() {
    muted_ = false;
}

bool VolumeControl::isMuted() const {
    return muted_;
}

} // namespace dsp
} // namespace audio