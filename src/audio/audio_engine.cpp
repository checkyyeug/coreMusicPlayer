#include "audio/audio_engine.h"
#include "core/equalizer_config.h"
#include <iostream>

namespace audio {

std::shared_ptr<AudioEngine> AudioEngine::instance() {
    static std::shared_ptr<AudioEngine> engine =
        std::make_shared<AudioEngine>();
    return engine;
}

AudioEngine::AudioEngine()
    : state_(EngineState::STOPPED),
      volume_(0.5f) {
}

bool AudioEngine::initialize() {
    // 简化实现 - 实际项目中需要初始化音频设备、驱动等
    std::cout << "Audio engine initialized\n";
    return true;
}

void AudioEngine::cleanup() {
    // 清理资源
    state_ = EngineState::STOPPED;
    std::cout << "Audio engine cleaned up\n";
}

bool AudioEngine::play_audio(const AudioBuffer& buffer, const AudioFormat& format) {
    if (buffer.size() == 0) {
        return false;
    }

    // 应用均衡器处理
    if (equalizer_config_ && equalizer_config_->is_enabled()) {
        auto params = equalizer_config_->get_params();
        // 创建并应用均衡器（简化实现）
        std::cout << "Applying equalizer with " << params.size() << " bands\n";
    }

    // 简化实现 - 实际项目中需要将音频数据传递给音频设备
    std::cout << "Playing audio: " << buffer.size()
              << " samples, format: " << static_cast<int>(format.format)
              << ", sample rate: " << format.sample_rate << "\n";

    state_ = EngineState::PLAYING;
    return true;
}

bool AudioEngine::stop_playback() {
    state_ = EngineState::STOPPED;
    std::cout << "Playback stopped\n";
    return true;
}

bool AudioEngine::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << "\n";
    return true;
}

std::string AudioEngine::get_status() const {
    switch (state_) {
        case EngineState::STOPPED:
            return "Stopped";
        case EngineState::PLAYING:
            return "Playing";
        case EngineState::PAUSED:
            return "Paused";
        default:
            return "Unknown";
    }
}

bool AudioEngine::is_playing() const {
    return state_ == EngineState::PLAYING;
}

// 设置均衡器配置
void AudioEngine::set_equalizer_config(std::shared_ptr<core::EqualizerConfig> config) {
    equalizer_config_ = config;
}

} // namespace audio