#ifndef CORE_AUDIO_VOCODER_H
#define CORE_AUDIO_VOCODER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频声码器类
class AudioVocoder {
public:
    // 构造函数
    AudioVocoder();
    
    // 析构函数
    ~AudioVocoder();
    
    // 初始化声码器
    bool initialize();
    
    // 关闭声码器
    void shutdown();
    
    // 应用声码效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置声码参数
    bool setParameters(float carrier_freq, float modulator_freq, float mix);
    
    // 获取声码参数
    void getParameters(float& carrier_freq, float& modulator_freq, float& mix) const;
    
    // 重置声码器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float carrier_freq_;     // 载波频率
    float modulator_freq_;   // 调制频率
    float mix_;              // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_VOCODER_H