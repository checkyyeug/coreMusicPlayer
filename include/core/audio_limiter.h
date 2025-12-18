#ifndef CORE_AUDIO_LIMITER_H
#define CORE_AUDIO_LIMITER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频限制器类
class AudioLimiter {
public:
    // 构造函数
    AudioLimiter();
    
    // 析构函数
    ~AudioLimiter();
    
    // 初始化限制器
    bool initialize();
    
    // 关闭限制器
    void shutdown();
    
    // 应用限制效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置限制参数
    bool setParameters(float threshold, float release);
    
    // 获取限制参数
    void getParameters(float& threshold, float& release) const;
    
    // 重置限制器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float threshold_;   // 阈值
    float release_;     // 释放时间
};

} // namespace core

#endif // CORE_AUDIO_LIMITER_H