#ifndef CORE_AUDIO_PLAYER_H
#define CORE_AUDIO_PLAYER_H

#include "core/audio_buffer.h"
#include <string>
#include <memory>

namespace core {

// 音频播放器类
class AudioPlayer {
public:
    // 构造函数
    AudioPlayer();
    
    // 析构函数
    ~AudioPlayer();
    
    // 初始化播放器
    bool initialize();
    
    // 关闭播放器
    void shutdown();
    
    // 播放音频文件
    bool play(const std::string& filename);
    
    // 停止播放
    bool stop();
    
    // 暂停播放
    bool pause();
    
    // 恢复播放
    bool resume();
    
    // 设置音量
    bool setVolume(float volume);
    
    // 获取音量
    float getVolume() const;
    
    // 设置播放位置
    bool setPosition(double position);
    
    // 获取播放位置
    double getPosition() const;
    
    // 获取播放状态
    bool isPlaying() const;
    
    // 获取音频信息
    void getAudioInfo(std::string& title, std::string& artist, 
                     double& duration) const;
    
private:
    // 私有成员变量
    bool initialized_;
    bool playing_;
    float volume_;
    double position_;
    double duration_;
    std::string current_file_;
};

} // namespace core

#endif // CORE_AUDIO_PLAYER_H