#ifndef CORE_AUDIO_SPECTRAL_DELAY_H
#define CORE_AUDIO_SPECTRAL_DELAY_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频频谱延迟器类
class AudioSpectralDelay {
public:
    // 构造函数
    AudioSpectralDelay();
    
    // 析构函数
    ~AudioSpectralDelay();
    
    // 初始化频谱延迟器
    bool initialize();
    
    // 关闭频谱延迟器
    void shutdown();
    
    // 应用频谱延迟效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置频谱延迟参数
    bool setParameters(float delay_time, float feedback, float mix, float filter_freq);
    
    // 获取频谱延迟参数
    void getParameters(float& delay_time, float& feedback, float& mix, float& filter_freq) const;
    
    // 重置频谱延迟器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float delay_time_;     // 延迟时间
    float feedback_;       // 反馈量
    float mix_;            // 混合比例
    float filter_freq_;    // 滤波频率
};

} // namespace core

#endif // CORE_AUDIO_SPECTRAL_DELAY_H