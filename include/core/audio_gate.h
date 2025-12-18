#ifndef CORE_AUDIO_GATE_H
#define CORE_AUDIO_GATE_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频门控器类
class AudioGate {
public:
    // 构造函数
    AudioGate();
    
    // 析构函数
    ~AudioGate();
    
    // 初始化门控器
    bool initialize();
    
    // 关闭门控器
    void shutdown();
    
    // 应用门控效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置门控参数
    bool setParameters(float threshold, float attack, float release);
    
    // 获取门控参数
    void getParameters(float& threshold, float& attack, float& release) const;
    
    // 重置门控器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float threshold_;   // 阈值
    float attack_;      // 攻击时间
    float release_;     // 释放时间
};

} // namespace core

#endif // CORE_AUDIO_GATE_H