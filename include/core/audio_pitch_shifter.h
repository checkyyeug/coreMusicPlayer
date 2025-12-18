#ifndef CORE_AUDIO_PITCH_SHIFTER_H
#define CORE_AUDIO_PITCH_SHIFTER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频音高变换器类
class AudioPitchShifter {
public:
    // 构造函数
    AudioPitchShifter();
    
    // 析构函数
    ~AudioPitchShifter();
    
    // 初始化音高变换器
    bool initialize();
    
    // 关闭音高变换器
    void shutdown();
    
    // 应用音高变换效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置音高变换参数
    bool setParameters(float pitch_shift, float mix);
    
    // 获取音高变换参数
    void getParameters(float& pitch_shift, float& mix) const;
    
    // 重置音高变换器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float pitch_shift_;   // 音高变换量
    float mix_;           // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_PITCH_SHIFTER_H