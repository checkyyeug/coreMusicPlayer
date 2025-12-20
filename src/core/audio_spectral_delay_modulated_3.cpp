#include "core/audio_spectral_delay_modulated_3.h"
#include <cmath>

namespace core {

AudioSpectralDelayModulated3::AudioSpectralDelayModulated3()
    : initialized_(false),
      rate_(0.0f),
      depth_(0.0f),
      feedback_(0.0f),
      mix_(0.0f),
      modulation_rate_(0.0f),
      modulation_depth_(0.0f),
      lfo_waveform_(0.0f) {
}

AudioSpectralDelayModulated3::~AudioSpectralDelayModulated3() {
    shutdown();
}

bool AudioSpectralDelayModulated3::initialize() {
    // 初始化调制频谱延迟
    initialized_ = true;
    return true;
}

void AudioSpectralDelayModulated3::shutdown() {
    // 关闭调制频谱延迟
    initialized_ = false;
}

bool AudioSpectralDelayModulated3::apply(const AudioBuffer& input, AudioBuffer& output) {
    if (!initialized_) {
        return false;
    }

    // 简单的实现：返回输入信号
    output = input;
    return true;
}

bool AudioSpectralDelayModulated3::setParameters(float rate, float depth, float feedback, float mix,
                                                float modulation_rate, float modulation_depth, float lfo_waveform) {
    rate_ = rate;
    depth_ = depth;
    feedback_ = feedback;
    mix_ = mix;
    modulation_rate_ = modulation_rate;
    modulation_depth_ = modulation_depth;
    lfo_waveform_ = lfo_waveform;
    return true;
}

void AudioSpectralDelayModulated3::getParameters(float& rate, float& depth, float& feedback, float& mix,
                                                float& modulation_rate, float& modulation_depth, float& lfo_waveform) const {
    rate = rate_;
    depth = depth_;
    feedback = feedback_;
    mix = mix_;
    modulation_rate = modulation_rate_;
    modulation_depth = modulation_depth_;
    lfo_waveform = lfo_waveform_;
}

void AudioSpectralDelayModulated3::reset() {
    // 重置调制频谱延迟
    rate_ = 0.0f;
    depth_ = 0.0f;
    feedback_ = 0.0f;
    mix_ = 0.0f;
    modulation_rate_ = 0.0f;
    modulation_depth_ = 0.0f;
    lfo_waveform_ = 0.0f;
}

} // namespace core