#ifndef CORE_AUDIO_TIME_STRETCH_H
#define CORE_AUDIO_TIME_STRETCH_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频时间拉伸器类
class AudioTimeStretch {
public:
    // 构造函数
    AudioTimeStretch();
    
    // 析构函数
    ~AudioTimeStretch();
    
    // 初始化时间拉伸器
    bool initialize();
    
    // 关闭时间拉伸器
    void shutdown();
    
    // 应用时间拉伸效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置时间拉伸参数
    bool setParameters(float time_stretch, float mix);
    
    // 获取时间拉伸参数
    void getParameters(float& time_stretch, float& mix) const;
    
    // 重置时间拉伸器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float time_stretch_;   // 时间拉伸量
    float mix_;            // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_TIME_STRETCH_H