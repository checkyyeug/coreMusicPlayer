#ifndef CORE_AUDIO_EFFECTS_H
#define CORE_AUDIO_EFFECTS_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频效果基类
class AudioEffect {
public:
    virtual ~AudioEffect() = default;
    
    // 应用音频效果
    virtual bool apply(AudioBuffer& buffer) = 0;
    
    // 获取效果名称
    virtual std::string getName() const = 0;
};

// 音频均衡器类
class AudioEqualizer : public AudioEffect {
public:
    // 构造函数
    AudioEqualizer();
    
    // 应用均衡器效果
    bool apply(AudioBuffer& buffer) override;
    
    // 获取效果名称
    std::string getName() const override;
    
    // 设置频段增益
    void setBandGain(int band, float gain);
    
    // 获取频段增益
    float getBandGain(int band) const;
    
private:
    // 频段增益数组
    std::vector<float> band_gains_;
};

// 音频混响类
class AudioReverb : public AudioEffect {
public:
    // 构造函数
    AudioReverb();
    
    // 应用混响效果
    bool apply(AudioBuffer& buffer) override;
    
    // 获取效果名称
    std::string getName() const override;
    
    // 设置混响参数
    void setParameters(float decay, float mix);
    
private:
    // 混响参数
    float decay_;
    float mix_;
};

// 音频淡入淡出类
class AudioFade : public AudioEffect {
public:
    // 构造函数
    AudioFade();
    
    // 应用淡入淡出效果
    bool apply(AudioBuffer& buffer) override;
    
    // 获取效果名称
    std::string getName() const override;
    
    // 设置淡入淡出参数
    void setParameters(float fade_in_time, float fade_out_time);
    
private:
    // 淡入淡出参数
    float fade_in_time_;
    float fade_out_time_;
};

} // namespace core

#endif // CORE_AUDIO_EFFECTS_H