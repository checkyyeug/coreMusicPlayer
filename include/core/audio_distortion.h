#ifndef CORE_AUDIO_DISTORTION_H
#define CORE_AUDIO_DISTORTION_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频失真器类
class AudioDistortion {
public:
    // 构造函数
    AudioDistortion();
    
    // 析构函数
    ~AudioDistortion();
    
    // 初始化失真器
    bool initialize();
    
    // 关闭失真器
    void shutdown();
    
    // 应用失真效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置失真参数
    bool setParameters(float drive, float tone, float mix);
    
    // 获取失真参数
    void getParameters(float& drive, float& tone, float& mix) const;
    
    // 重置失真器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float drive_;   // 驱动强度
    float tone_;    // 音调
    float mix_;     // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_DISTORTION_H