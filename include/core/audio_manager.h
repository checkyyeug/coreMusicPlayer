#ifndef CORE_AUDIO_MANAGER_H
#define CORE_AUDIO_MANAGER_H

#include <string>
#include <memory>

namespace core {

// 音频管理器类
class AudioManager {
public:
    virtual ~AudioManager() = default;
    
    // 初始化音频系统
    virtual bool initialize() = 0;
    
    // 关闭音频系统
    virtual void shutdown() = 0;
    
    // 播放音频文件
    virtual bool play(const std::string& filename) = 0;
    
    // 暂停播放
    virtual bool pause() = 0;
    
    // 恢复播放
    virtual bool resume() = 0;
    
    // 停止播放
    virtual bool stop() = 0;
    
    // 设置音量
    virtual bool setVolume(float volume) = 0;
    
    // 获取当前音量
    virtual float getVolume() const = 0;
};

} // namespace core

#endif // CORE_AUDIO_MANAGER_H