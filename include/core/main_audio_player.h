#ifndef CORE_MAIN_AUDIO_PLAYER_H
#define CORE_MAIN_AUDIO_PLAYER_H

#include "core/audio_system.h"
#include <memory>

namespace core {

// 主音频播放器类
class MainAudioPlayer {
public:
    // 构造函数
    MainAudioPlayer();
    
    // 析构函数
    ~MainAudioPlayer();
    
    // 初始化播放器
    bool initialize();
    
    // 关闭播放器
    void shutdown();
    
    // 播放音频文件
    bool play(const std::string& filename);
    
    // 暂停播放
    bool pause();
    
    // 恢复播放
    bool resume();
    
    // 停止播放
    bool stop();
    
    // 设置音量
    bool setVolume(float volume);
    
    // 获取当前音量
    float getVolume() const;
    
    // 获取播放状态
    bool isPlaying() const;
    
    // 获取当前播放位置
    double getCurrentPosition() const;
    
    // 跳转到指定位置
    bool seek(double position);
    
private:
    // 私有成员变量
    bool initialized_;
    std::unique_ptr<AudioSystem> audio_system_;
};

} // namespace core

#endif // CORE_MAIN_AUDIO_PLAYER_H