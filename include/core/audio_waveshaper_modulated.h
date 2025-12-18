#ifndef CORE_AUDIO_WAVESHAPER_MODULATED_H
#define CORE_AUDIO_WAVESHAPER_MODULATED_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制波形整形器类
class AudioWaveshaperModulated {
public:
    // 构造函数
    AudioWaveshaperModulated();
    
    // 析构函数
    ~AudioWaveshaperModulated();
    
    // 初始化调制波形整形器
    bool initialize();
    
    // 关闭调制波形整形器
    void shutdown();
    
    // 应用调制波形整形效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制波形整形参数
    bool setParameters(float drive, float shape, float mix, float modulation_rate, float modulation_depth);
    
    // 获取调制波形整形参数
    void getParameters(float& drive, float& shape, float& mix, float& modulation_rate, float& modulation_depth) const;
    
    // 重置调制波形整形器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float drive_;             // 驱动强度
    float shape_;             // 形状参数
    float mix_;               // 混合比例
    float modulation_rate_;  // 调制速率
    float modulation_depth_; // 调制深度
};

} // namespace core

#endif // CORE_AUDIO_WAVESHAPER_MODULATED_H