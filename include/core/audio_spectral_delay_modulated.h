#ifndef CORE_AUDIO_SPECTRAL_DELAY_MODULATED_H
#define CORE_AUDIO_SPECTRAL_DELAY_MODULATED_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制频谱延迟器类
class AudioSpectralDelayModulated {
public:
    // 构造函数
    AudioSpectralDelayModulated();
    
    // 析构函数
    ~AudioSpectralDelayModulated();
    
    // 初始化调制频谱延迟器
    bool initialize();
    
    // 关闭调制频谱延迟器
    void shutdown();
    
    // 应用调制频谱延迟效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制频谱延迟参数
    bool setParameters(float delay_time, float feedback, float mix, float filter_freq, float modulation_rate, float modulation_depth);
    
    // 获取调制频谱延迟参数
    void getParameters(float& delay_time, float& feedback, float& mix, float& filter_freq, float& modulation_rate, float& modulation_depth) const;
    
    // 重置调制频谱延迟器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float delay_time_;        // 延迟时间
    float feedback_;          // 反馈量
    float mix_;               // 混合比例
    float filter_freq_;       // 滤波频率
    float modulation_rate_;  // 调制速率
    float modulation_depth_; // 调制深度
};

} // namespace core

#endif // CORE_AUDIO_SPECTRAL_DELAY_MODULATED_H