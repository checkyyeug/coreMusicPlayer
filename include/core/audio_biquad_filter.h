#ifndef CORE_AUDIO_BIQUAD_FILTER_H
#define CORE_AUDIO_BIQUAD_FILTER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频双二阶滤波器类
class AudioBiquadFilter {
public:
    // 构造函数
    AudioBiquadFilter();
    
    // 析构函数
    ~AudioBiquadFilter();
    
    // 初始化滤波器
    bool initialize();
    
    // 关闭滤波器
    void shutdown();
    
    // 应用滤波效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置滤波参数
    bool setParameters(float frequency, float q_factor, int filter_type);
    
    // 获取滤波参数
    void getParameters(float& frequency, float& q_factor, int& filter_type) const;
    
    // 重置滤波器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float frequency_;     // 截止频率
    float q_factor_;      // Q因子
    int filter_type_;     // 滤波类型 (0: lowpass, 1: highpass, 2: bandpass)
};

} // namespace core

#endif // CORE_AUDIO_BIQUAD_FILTER_H