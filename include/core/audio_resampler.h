#ifndef CORE_AUDIO_RESAMPLER_H
#define CORE_AUDIO_RESAMPLER_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频重采样器基类
class AudioResampler {
public:
    virtual ~AudioResampler() = default;
    
    // 重采样音频数据
    virtual bool resample(const AudioBuffer& input, AudioBuffer& output,
                         double ratio) = 0;
    
    // 获取重采样器名称
    virtual std::string getName() const = 0;
};

} // namespace core

#endif // CORE_AUDIO_RESAMPLER_H