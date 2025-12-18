#ifndef CORE_AUDIO_REVERB_H
#define CORE_AUDIO_REVERB_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频混响器类
class AudioReverb {
public:
    // 构造函数
    AudioReverb();
    
    // 析构函数
    ~AudioReverb();
    
    // 初始化混响器
    bool initialize();
    
    // 关闭混响器
    void shutdown();
    
    // 应用混响效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置混响参数
    bool setParameters(float room_size, float damping, float wet_level, float dry_level);
    
    // 获取混响参数
    void getParameters(float& room_size, float& damping, float& wet_level, float& dry_level) const;
    
    // 重置混响器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float room_size_;     // 房间大小
    float damping_;       // 阻尼
    float wet_level_;     // 湿信号水平
    float dry_level_;     // 干信号水平
};

} // namespace core

#endif // CORE_AUDIO_REVERB_H