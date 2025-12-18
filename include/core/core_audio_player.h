#ifndef CORE_CORE_AUDIO_PLAYER_H
#define CORE_CORE_AUDIO_PLAYER_H

#include <string>
#include <memory>

namespace core {

// 核心音频播放器类
class CoreAudioPlayer {
public:
    CoreAudioPlayer();
    ~CoreAudioPlayer();
    
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
    bool playing_;
    float volume_;
};

} // namespace core

#endif // CORE_CORE_AUDIO_PLAYER_H