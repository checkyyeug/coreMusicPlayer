#ifndef PLATFORM_WIN32_WIN32_AUDIO_MANAGER_H
#define PLATFORM_WIN32_WIN32_AUDIO_MANAGER_H

#include "platform/audio_manager.h"
#include <windows.h>

namespace platform {
namespace win32 {

// Windows平台音频管理器
class Win32AudioManager : public AudioManager {
public:
    Win32AudioManager();
    ~Win32AudioManager() override = default;
    
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
    // Windows音频相关成员变量
    HWAVEOUT hWaveOut_;
    bool initialized_;
    float volume_;
};

} // namespace win32
} // namespace platform

#endif // PLATFORM_WIN32_WIN32_AUDIO_MANAGER_H