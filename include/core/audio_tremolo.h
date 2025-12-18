#ifndef CORE_AUDIO_TREMOLO_H
#define CORE_AUDIO_TREMOLO_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频颤音器类
class AudioTremolo {
public:
    // 构造函数
    AudioTremolo();
    
    // 析构函数
    ~AudioTremolo();
    
    // 初始化颤音器
    bool initialize();
    
    // 关闭颤音器
    void shutdown();
    
    // 应用颤音效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置颤音参数
    bool setParameters(float rate, float depth, float mix);
    
    // 获取颤音参数
    void getParameters(float& rate, float& depth, float& mix) const;
    
    // 重置颤音器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float rate_;     // 调制速率
    float depth_;    // 调制深度
    float mix_;      // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_TREMOLO_H