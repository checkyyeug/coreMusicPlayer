#ifndef PLATFORM_MACOS_MACOS_AUDIO_MANAGER_H
#define PLATFORM_MACOS_MACOS_AUDIO_MANAGER_H

#include "platform/audio_manager.h"

namespace platform {
namespace macos {

// macOS平台音频管理器
class MacosAudioManager : public AudioManager {
public:
    MacosAudioManager();
    ~MacosAudioManager() override = default;
    
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
    // macOS音频相关成员变量
    bool initialized_;
    float volume_;
};

} // namespace macos
} // namespace platform

#endif // PLATFORM_MACOS_MACOS_AUDIO_MANAGER_H