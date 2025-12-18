#ifndef CORE_AUDIO_DELAY_H
#define CORE_AUDIO_DELAY_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频延迟器类
class AudioDelay {
public:
    // 构造函数
    AudioDelay();
    
    // 析构函数
    ~AudioDelay();
    
    // 初始化延迟器
    bool initialize();
    
    // 关闭延迟器
    void shutdown();
    
    // 应用延迟效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置延迟参数
    bool setParameters(float delay_time, float feedback, float mix);
    
    // 获取延迟参数
    void getParameters(float& delay_time, float& feedback, float& mix) const;
    
    // 重置延迟器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float delay_time_;   // 延迟时间
    float feedback_;     // 反馈量
    float mix_;          // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_DELAY_H