#ifndef CORE_AUDIO_PEAK_LIMITER_H
#define CORE_AUDIO_PEAK_LIMITER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频峰值限制器类
class AudioPeakLimiter {
public:
    // 构造函数
    AudioPeakLimiter();
    
    // 析构函数
    ~AudioPeakLimiter();
    
    // 初始化峰值限制器
    bool initialize();
    
    // 关闭峰值限制器
    void shutdown();
    
    // 应用峰值限制效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置峰值限制参数
    bool setParameters(float threshold, float release);
    
    // 获取峰值限制参数
    void getParameters(float& threshold, float& release) const;
    
    // 重置峰值限制器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float threshold_;   // 阈值
    float release_;     // 释放时间
};

} // namespace core

#endif // CORE_AUDIO_PEAK_LIMITER_H