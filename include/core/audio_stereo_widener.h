#ifndef CORE_AUDIO_STEREO_WIDENER_H
#define CORE_AUDIO_STEREO_WIDENER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频立体声扩展器类
class AudioStereoWidener {
public:
    // 构造函数
    AudioStereoWidener();
    
    // 析构函数
    ~AudioStereoWidener();
    
    // 初始化立体声扩展器
    bool initialize();
    
    // 关闭立体声扩展器
    void shutdown();
    
    // 应用立体声扩展效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置立体声扩展参数
    bool setParameters(float width, float mix);
    
    // 获取立体声扩展参数
    void getParameters(float& width, float& mix) const;
    
    // 重置立体声扩展器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float width_;   // 立体声宽度
    float mix_;     // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_STEREO_WIDENER_H