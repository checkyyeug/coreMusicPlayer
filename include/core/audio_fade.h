#ifndef CORE_AUDIO_FADE_H
#define CORE_AUDIO_FADE_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频淡入淡出类
class AudioFade {
public:
    // 构造函数
    AudioFade();
    
    // 析构函数
    ~AudioFade();
    
    // 初始化淡入淡出器
    bool initialize();
    
    // 关闭淡入淡出器
    void shutdown();
    
    // 应用淡入效果
    bool fadeIn(const AudioBuffer& input, AudioBuffer& output, double duration);
    
    // 应用淡出效果
    bool fadeOut(const AudioBuffer& input, AudioBuffer& output, double duration);
    
    // 设置淡入淡出参数
    bool setParameters(double fade_in_time, double fade_out_time);
    
    // 获取淡入淡出参数
    void getParameters(double& fade_in_time, double& fade_out_time) const;
    
private:
    // 私有成员变量
    bool initialized_;
    double fade_in_time_;   // 淡入时间
    double fade_out_time_;  // 淡出时间
};

} // namespace core

#endif // CORE_AUDIO_FADE_H