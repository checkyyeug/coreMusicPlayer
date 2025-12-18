#ifndef CORE_AUDIO_WAVESHAPER_H
#define CORE_AUDIO_WAVESHAPER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频波形整形器类
class AudioWaveshaper {
public:
    // 构造函数
    AudioWaveshaper();
    
    // 析构函数
    ~AudioWaveshaper();
    
    // 初始化波形整形器
    bool initialize();
    
    // 关闭波形整形器
    void shutdown();
    
    // 应用波形整形效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置波形整形参数
    bool setParameters(float drive, float shape, float mix);
    
    // 获取波形整形参数
    void getParameters(float& drive, float& shape, float& mix) const;
    
    // 重置波形整形器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float drive_;   // 驱动强度
    float shape_;   // 形状参数
    float mix_;     // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_WAVESHAPER_H