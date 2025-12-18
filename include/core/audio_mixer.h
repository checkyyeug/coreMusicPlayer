#ifndef CORE_AUDIO_MIXER_H
#define CORE_AUDIO_MIXER_H

#include "core/audio_buffer.h"
#include <vector>
#include <memory>

namespace core {

// 音频混音器类
class AudioMixer {
public:
    // 构造函数
    AudioMixer();
    
    // 析构函数
    ~AudioMixer();
    
    // 初始化混音器
    bool initialize();
    
    // 关闭混音器
    void shutdown();
    
    // 添加音频轨道
    bool addTrack(const std::string& name, const AudioBuffer& buffer);
    
    // 移除音频轨道
    bool removeTrack(const std::string& name);
    
    // 设置轨道音量
    bool setTrackVolume(const std::string& name, float volume);
    
    // 获取轨道音量
    float getTrackVolume(const std::string& name) const;
    
    // 设置轨道静音状态
    bool setTrackMute(const std::string& name, bool mute);
    
    // 获取轨道静音状态
    bool getTrackMute(const std::string& name) const;
    
    // 混音所有轨道
    bool mix(AudioBuffer& output);
    
    // 清空所有轨道
    void clear();
    
    // 获取轨道数量
    size_t getTrackCount() const;
    
    // 获取轨道名称列表
    std::vector<std::string> getTrackNames() const;
    
private:
    // 轨道信息结构体
    struct Track {
        std::string name;
        AudioBuffer buffer;
        float volume;
        bool mute;
        
        Track(const std::string& n, const AudioBuffer& b) 
            : name(n), buffer(b), volume(1.0f), mute(false) {}
    };
    
    // 私有成员变量
    bool initialized_;
    std::vector<Track> tracks_;
};

} // namespace core

#endif // CORE_AUDIO_MIXER_H