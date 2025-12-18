#ifndef CORE_AUDIO_WAVEFORM_GENERATOR_H
#define CORE_AUDIO_WAVEFORM_GENERATOR_H

#include "core/audio_buffer.h"
#include <vector>

namespace core {

// 音频波形生成器类
class AudioWaveformGenerator {
public:
    // 构造函数
    AudioWaveformGenerator();
    
    // 析构函数
    ~AudioWaveformGenerator();
    
    // 初始化生成器
    bool initialize();
    
    // 关闭生成器
    void shutdown();
    
    // 生成正弦波
    AudioBuffer generateSine(float frequency, float duration, float amplitude = 1.0f);
    
    // 生成方波
    AudioBuffer generateSquare(float frequency, float duration, float amplitude = 1.0f);
    
    // 生成三角波
    AudioBuffer generateTriangle(float frequency, float duration, float amplitude = 1.0f);
    
    // 生成锯齿波
    AudioBuffer generateSawtooth(float frequency, float duration, float amplitude = 1.0f);
    
    // 生成白噪声
    AudioBuffer generateWhiteNoise(float duration, float amplitude = 1.0f);
    
    // 设置生成参数
    bool setParameters(int sample_rate, int channels);
    
    // 获取生成参数
    void getParameters(int& sample_rate, int& channels) const;
    
private:
    // 私有成员变量
    bool initialized_;
    int sample_rate_;
    int channels_;
};

} // namespace core

#endif // CORE_AUDIO_WAVEFORM_GENERATOR_H