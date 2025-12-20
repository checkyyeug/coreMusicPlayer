#ifndef CORE_AUDIO_SPECTRAL_DELAY_MODULATED_3_H
#define CORE_AUDIO_SPECTRAL_DELAY_MODULATED_3_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制频谱延迟类（版本3）
class AudioSpectralDelayModulated3 {
public:
    // 构造函数
    AudioSpectralDelayModulated3();

    // 析构函数
    ~AudioSpectralDelayModulated3();

    // 初始化调制频谱延迟（版本3）
    bool initialize();

    // 关闭调制频谱延迟（版本3）
    void shutdown();

    // 应用调制频谱延迟效果（版本3）
    bool apply(const AudioBuffer& input, AudioBuffer& output);

    // 设置调制频谱延迟参数（版本3）
    bool setParameters(float rate, float depth, float feedback, float mix, float modulation_rate, float modulation_depth, float lfo_waveform);

    // 获取调制频谱延迟参数（版本3）
    void getParameters(float& rate, float& depth, float& feedback, float& mix, float& modulation_rate, float& modulation_depth, float& lfo_waveform) const;

    // 重置调制频谱延迟（版本3）
    void reset();

private:
    // 私有成员变量
    bool initialized_;
    float rate_;              // 基础调制速率
    float depth_;             // 基础调制深度
    float feedback_;          // 反馈量
    float mix_;               // 混合比例
    float modulation_rate_;   // 调制速率
    float modulation_depth_;  // 调制深度
    float lfo_waveform_;      // LFO波形类型 (0: sine, 1: triangle, 2: square, 3: sawtooth)
};

} // namespace core

#endif // CORE_AUDIO_SPECTRAL_DELAY_MODULATED_3_H