#ifndef CORE_AUDIO_SPECTRAL_DELAY_MODULATED_2_H
#define CORE_AUDIO_SPECTRAL_DELAY_MODULATED_2_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制频谱延迟器类（版本2）
class AudioSpectralDelayModulated2 {
public:
    // 构造函数
    AudioSpectralDelayModulated2();
    
    // 析构函数
    ~AudioSpectralDelayModulated2();
    
    // 初始化调制频谱延迟器（版本2）
    bool initialize();
    
    // 关闭调制频谱延迟器（版本2）
    void shutdown();
    
    // 应用调制频谱延迟效果（版本2）
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制频谱延迟参数（版本2）
    bool setParameters(float delay_time, float feedback, float mix, float filter_freq, float modulation_rate, float modulation_depth, float lfo_waveform);
    
    // 获取调制频谱延迟参数（版本2）
    void getParameters(float& delay_time, float& feedback, float& mix, float& filter_freq, float& modulation_rate, float& modulation_depth, float& lfo_waveform) const;
    
    // 重置调制频谱延迟器（版本2）
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
    float lfo_waveform_;      // LFO波形类型 (0: sine, 1: triangle, 2: square, 3: sawtooth)
};

} // namespace core

#endif // CORE_AUDIO_SPECTRAL_DELAY_MODULATED_2_H