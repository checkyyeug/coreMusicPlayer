#ifndef CORE_AUDIO_VOCODER_MODULATED_3_H
#define CORE_AUDIO_VOCODER_MODULATED_3_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制声码器类（版本3）
class AudioVocoderModulated3 {
public:
    // 构造函数
    AudioVocoderModulated3();
    
    // 析构函数
    ~AudioVocoderModulated3();
    
    // 初始化调制声码器（版本3）
    bool initialize();
    
    // 关闭调制声码器（版本3）
    void shutdown();
    
    // 应用调制声码效果（版本3）
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制声码参数（版本3）
    bool setParameters(float carrier_freq, float modulator_freq, float mix, float modulation_rate, float modulation_depth, float lfo_waveform, float stereo_width);
    
    // 获取调制声码参数（版本3）
    void getParameters(float& carrier_freq, float& modulator_freq, float& mix, float& modulation_rate, float& modulation_depth, float& lfo_waveform, float& stereo_width) const;
    
    // 重置调制声码器（版本3）
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float carrier_freq_;      // 载波频率
    float modulator_freq_;   // 调制频率
    float mix_;              // 混合比例
    float modulation_rate_;  // 调制速率
    float modulation_depth_; // 调制深度
    float lfo_waveform_;      // LFO波形类型 (0: sine, 1: triangle, 2: square, 3: sawtooth)
    float stereo_width_;       // 立体声宽度
};

} // namespace core

#endif // CORE_AUDIO_VOCODER_MODULATED_3_H