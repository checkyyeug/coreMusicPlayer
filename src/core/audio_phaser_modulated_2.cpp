#include "core/audio_phaser_modulated_2.h"
#include <cmath>

namespace core {

AudioPhaserModulated2::AudioPhaserModulated2() 
    : initialized_(false),
      rate_(0.0f),
      depth_(0.0f),
      feedback_(0.0f),
      mix_(0.0f),
      modulation_rate_(0.0f),
      modulation_depth_(0.0f),
      lfo_waveform_(0.0f) {
}

AudioPhaserModulated2::~AudioPhaserModulated2() {
    shutdown();
}

bool AudioPhaserModulated2::initialize() {
    // 初始化调制相位器
    initialized_ = true;
    return true;
}

void AudioPhaserModulated2::shutdown() {
    // 关闭调制相位器
    initialized_ = false;
}

bool AudioPhaserModulated2::apply(const AudioBuffer& input, AudioBuffer& output) {
    if (!initialized_) {
        return false;
    }
    
    // 简单的实现：返回输入信号
    output = input;
    return true;
}

bool AudioPhaserModulated2::setParameters(float rate, float depth, float feedback, float mix, 
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

void AudioPhaserModulated2::getParameters(float& rate, float& depth, float& feedback, float& mix, 
                                          float& modulation_rate, float& modulation_depth, float& lfo_waveform) const {
    rate = rate_;
    depth = depth_;
    feedback = feedback_;
    mix = mix_;
    modulation_rate = modulation_rate_;
    modulation_depth = modulation_depth_;
    lfo_waveform = lfo_waveform_;
}

void AudioPhaserModulated2::reset() {
    // 重置调制相位器
    rate_ = 0.0f;
    depth_ = 0.0f;
    feedback_ = 0.0f;
    mix_ = 0.0f;
    modulation_rate_ = 0.0f;
    modulation_depth_ = 0.0f;
    lfo_waveform_ = 0.0f;
}

} // namespace core