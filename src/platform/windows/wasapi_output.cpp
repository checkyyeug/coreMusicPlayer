#include "platform/windows/wasapi_output.h"
#include <iostream>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <mmdeviceapi.h>
    #include <endpointvolume.h>
    #include <audioclient.h>
#endif

namespace platform {
namespace windows {

WasapiOutput::WasapiOutput()
    : initialized_(false),
      playing_(false),
      volume_(0.5f) {
}

WasapiOutput::~WasapiOutput() {
    cleanup();
}

bool WasapiOutput::initialize(const std::string& device_name) {
#ifdef _WIN32
    // 简化实现 - 实际项目中需要初始化WASAPI
    device_name_ = device_name;
    initialized_ = true;
    std::cout << "WASAPI output initialized\n";
    return true;
#else
    return false;
#endif
}

void WasapiOutput::cleanup() {
    playing_ = false;
    initialized_ = false;
    std::cout << "WASAPI output cleaned up\n";
}

bool WasapiOutput::play_audio(const void* data, size_t size) {
#ifdef _WIN32
    if (!initialized_) {
        return false;
    }
    
    // 简化实现 - 实际项目中需要将音频数据传递给WASAPI
    std::cout << "Playing audio via WASAPI: " << size << " bytes\n";
    playing_ = true;
    return true;
#else
    return false;
#endif
}

bool WasapiOutput::stop() {
#ifdef _WIN32
    playing_ = false;
    std::cout << "WASAPI playback stopped\n";
    return true;
#else
    return false;
#endif
}

bool WasapiOutput::set_volume(float volume) {
#ifdef _WIN32
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    volume_ = volume;
    std::cout << "WASAPI volume set to: " << volume_ << "\n";
    return true;
#else
    return false;
#endif
}

std::string WasapiOutput::get_status() const {
    if (!initialized_) {
        return "Not initialized";
    }
    if (playing_) {
        return "Playing";
    }
    return "Stopped";
}

bool WasapiOutput::is_playing() const {
    return playing_;
}

} // namespace windows
} // namespace platform