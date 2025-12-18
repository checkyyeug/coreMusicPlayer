#include "platform/linux/alsa_output.h"
#include <iostream>

#ifdef __linux__
    #include <alsa/asoundlib.h>
#endif

namespace platform {
namespace linux {

AlsaOutput::AlsaOutput()
    : initialized_(false),
      playing_(false),
      volume_(0.5f) {
}

AlsaOutput::~AlsaOutput() {
    cleanup();
}

bool AlsaOutput::initialize(const std::string& device_name) {
#ifdef __linux__
    // 简化实现 - 实际项目中需要初始化ALSA
    device_name_ = device_name;
    initialized_ = true;
    std::cout << "ALSA output initialized\n";
    return true;
#else
    return false;
#endif
}

void AlsaOutput::cleanup() {
    playing_ = false;
    initialized_ = false;
    std::cout << "ALSA output cleaned up\n";
}

bool AlsaOutput::play_audio(const void* data, size_t size) {
#ifdef __linux__
    if (!initialized_) {
        return false;
    }
    
    // 简化实现 - 实际项目中需要将音频数据传递给ALSA
    std::cout << "Playing audio via ALSA: " << size << " bytes\n";
    playing_ = true;
    return true;
#else
    return false;
#endif
}

bool AlsaOutput::stop() {
#ifdef __linux__
    playing_ = false;
    std::cout << "ALSA playback stopped\n";
    return true;
#else
    return false;
#endif
}

bool AlsaOutput::set_volume(float volume) {
#ifdef __linux__
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    volume_ = volume;
    std::cout << "ALSA volume set to: " << volume_ << "\n";
    return true;
#else
    return false;
#endif
}

std::string AlsaOutput::get_status() const {
    if (!initialized_) {
        return "Not initialized";
    }
    if (playing_) {
        return "Playing";
    }
    return "Stopped";
}

bool AlsaOutput::is_playing() const {
    return playing_;
}

} // namespace linux
} // namespace platform