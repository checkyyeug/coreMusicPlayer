#ifndef CORE_AUDIO_SPECTRAL_FILTER_MODULATED_H
#define CORE_AUDIO_SPECTRAL_FILTER_MODULATED_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制频谱滤波器类
class AudioSpectralFilterModulated {
public:
    // 构造函数
    AudioSpectralFilterModulated();
    
    // 析构函数
    ~AudioSpectralFilterModulated();
    
    // 初始化调制频谱滤波器
    bool initialize();
    
    // 关闭调制频谱滤波器
    void shutdown();
    
    // 应用调制频谱滤波效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制频谱滤波参数
    bool setParameters(float frequency, float q_factor, int filter_type, float modulation_rate, float modulation_depth);
    
    // 获取调制频谱滤波参数
    void getParameters(float& frequency, float& q_factor, int& filter_type, float& modulation_rate, float& modulation_depth) const;
    
    // 重置调制频谱滤波器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float frequency_;        // 截止频率
    float q_factor_;         // Q因子
    int filter_type_;        // 滤波类型 (0: lowpass, 1: highpass, 2: bandpass)
    float modulation_rate_;   // 调制速率
    float modulation_depth_; // 调制深度
};

} // namespace core

#endif // CORE_AUDIO_SPECTRAL_FILTER_MODULATED_H