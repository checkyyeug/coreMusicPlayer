#include "audio/audio_engine.h"
#include "audio/device_manager.h"
#include "core/equalizer_config.h"
#include <iostream>
#include <memory>

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
    // 初始化音频设备和驱动
    std::cout << "Initializing audio engine..." << std::endl;

    // 这里应该调用平台特定的初始化代码
    // 例如：WASAPI, ALSA, CoreAudio等

    // 检查是否已设置音频设备
    if (device_manager_ == nullptr) {
        // 创建默认设备管理器（实际项目中可能需要更复杂的逻辑）
        device_manager_ = std::make_shared<DeviceManager>();
    }

    // 初始化设备管理器
    if (!device_manager_->initialize()) {
        std::cerr << "Failed to initialize device manager" << std::endl;
        return false;
    }

    std::cout << "Audio engine initialized successfully" << std::endl;
    return true;
}

void AudioEngine::cleanup() {
    // 清理资源
    state_ = EngineState::STOPPED;
    if (device_manager_) {
        device_manager_->cleanup();
    }
    std::cout << "Audio engine cleaned up" << std::endl;
}

bool AudioEngine::play_audio(const AudioBuffer& buffer, const AudioFormat& format) {
    if (buffer.size() == 0) {
        return false;
    }

    // 应用均衡器处理
    if (equalizer_config_) {
        auto params = equalizer_config_->getAllGains();
        // 创建并应用均衡器（实际项目中需要更复杂的实现）
        std::cout << "Applying equalizer with " << params.size() << " bands" << std::endl;
    }

    // 实际播放音频数据
    // 这里应该调用平台特定的音频输出API
    std::cout << "Playing audio: " << buffer.size()
              << " samples, format: " << static_cast<int>(format.format)
              << ", sample rate: " << format.sample_rate << std::endl;

    state_ = EngineState::PLAYING;
    return true;
}

bool AudioEngine::stop_playback() {
    state_ = EngineState::STOPPED;
    std::cout << "Playback stopped" << std::endl;
    return true;
}

bool AudioEngine::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << std::endl;
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

// 获取设备管理器
std::shared_ptr<DeviceManager> AudioEngine::get_device_manager() const {
    return device_manager_;
}

// 设置设备管理器
void AudioEngine::set_device_manager(std::shared_ptr<DeviceManager> manager) {
    device_manager_ = manager;
}

} // namespace audio