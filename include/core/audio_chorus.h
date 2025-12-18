#ifndef CORE_AUDIO_CHORUS_H
#define CORE_AUDIO_CHORUS_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频合唱器类
class AudioChorus {
public:
    // 构造函数
    AudioChorus();
    
    // 析构函数
    ~AudioChorus();
    
    // 初始化合唱器
    bool initialize();
    
    // 关闭合唱器
    void shutdown();
    
    // 应用合唱效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置合唱参数
    bool setParameters(float rate, float depth, float feedback, float mix);
    
    // 获取合唱参数
    void getParameters(float& rate, float& depth, float& feedback, float& mix) const;
    
    // 重置合唱器
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

#endif // CORE_AUDIO_CHORUS_H