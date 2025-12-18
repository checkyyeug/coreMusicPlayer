#ifndef CORE_AUDIO_FILTER_H
#define CORE_AUDIO_FILTER_H

#include "core/audio_buffer.h"
#include <string>

namespace core {

// 音频滤波器基类
class AudioFilter {
public:
    // 构造函数
    AudioFilter();
    
    // 虚析构函数
    virtual ~AudioFilter() = default;
    
    // 应用滤波效果
    virtual bool apply(const AudioBuffer& input, AudioBuffer& output) = 0;
    
    // 设置滤波器参数
    virtual bool setParameters(float param1, float param2, float param3) = 0;
    
    // 获取滤波器参数
    virtual void getParameters(float& param1, float& param2, float& param3) const = 0;
    
    // 获取滤波器名称
    virtual std::string getName() const = 0;
    
    // 初始化滤波器
    virtual bool initialize() = 0;
    
    // 关闭滤波器
    virtual void shutdown() = 0;
};

} // namespace core

#endif // CORE_AUDIO_FILTER_H