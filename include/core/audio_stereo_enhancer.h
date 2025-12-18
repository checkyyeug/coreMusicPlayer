#ifndef CORE_AUDIO_STEREO_ENHANCER_H
#define CORE_AUDIO_STEREO_ENHANCER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频立体声增强器类
class AudioStereoEnhancer {
public:
    // 构造函数
    AudioStereoEnhancer();
    
    // 析构函数
    ~AudioStereoEnhancer();
    
    // 初始化立体声增强器
    bool initialize();
    
    // 关闭立体声增强器
    void shutdown();
    
    // 应用立体声增强效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置增强参数
    bool setParameters(float stereo_width, float stereo_imbalance, float mix);
    
    // 获取增强参数
    void getParameters(float& stereo_width, float& stereo_imbalance, float& mix) const;
    
    // 重置立体声增强器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float stereo_width_;       // 立体声宽度
    float stereo_imbalance_;   // 立体声不平衡度
    float mix_;                // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_STEREO_ENHANCER_H