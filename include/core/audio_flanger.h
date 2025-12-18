#ifndef CORE_AUDIO_FLANGER_H
#define CORE_AUDIO_FLANGER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频镶边器类
class AudioFlanger {
public:
    // 构造函数
    AudioFlanger();
    
    // 析构函数
    ~AudioFlanger();
    
    // 初始化镶边器
    bool initialize();
    
    // 关闭镶边器
    void shutdown();
    
    // 应用镶边效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置镶边参数
    bool setParameters(float rate, float depth, float feedback, float mix);
    
    // 获取镶边参数
    void getParameters(float& rate, float& depth, float& feedback, float& mix) const;
    
    // 重置镶边器
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

#endif // CORE_AUDIO_FLANGER_H