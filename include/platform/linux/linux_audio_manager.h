#ifndef PLATFORM_LINUX_LINUX_AUDIO_MANAGER_H
#define PLATFORM_LINUX_LINUX_AUDIO_MANAGER_H

#include "platform/audio_manager.h"

namespace platform {
namespace linux {

// Linux平台音频管理器
class LinuxAudioManager : public AudioManager {
public:
    LinuxAudioManager();
    ~LinuxAudioManager() override = default;
    
    // 实现音频管理接口
    bool initialize() override;
    void shutdown() override;
    bool play(const std::string& filename) override;
    bool pause() override;
    bool resume() override;
    bool stop() override;
    bool setVolume(float volume) override;
    float getVolume() const override;
    
private:
    // Linux音频相关成员变量
    bool initialized_;
    float volume_;
};

} // namespace linux
} // namespace platform

#endif // PLATFORM_LINUX_LINUX_AUDIO_MANAGER_H