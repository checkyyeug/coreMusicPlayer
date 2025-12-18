#ifndef CORE_AUDIO_PHASER_H
#define CORE_AUDIO_PHASER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频相位器类
class AudioPhaser {
public:
    // 构造函数
    AudioPhaser();
    
    // 析构函数
    ~AudioPhaser();
    
    // 初始化相位器
    bool initialize();
    
    // 关闭相位器
    void shutdown();
    
    // 应用相位效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置相位参数
    bool setParameters(float rate, float depth, float feedback, float mix);
    
    // 获取相位参数
    void getParameters(float& rate, float& depth, float& feedback, float& mix) const;
    
    // 重置相位器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float rate_;       // 调制速率
    float depth_;      // 调制深度
    float feedback_;   // 反馈量
    float mix_;        // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_PHASER_H