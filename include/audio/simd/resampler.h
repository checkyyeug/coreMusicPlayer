#ifndef AUDIO_SIMD_RESAMPLER_H
#define AUDIO_SIMD_RESAMPLER_H

#include <string>

namespace audio {
namespace simd {

// SIMD优化的重采样器基类
class Resampler {
public:
    virtual ~Resampler() = default;
    
    // 重采样接口
    virtual void resample(const float* input, size_t input_frames,
                          float* output, size_t output_frames) = 0;
    
    // 获取重采样器名称
    virtual std::string getName() const = 0;
    
    // 检查是否支持该优化
    virtual bool isSupported() const = 0;
};

} // namespace simd
} // namespace audio

#endif // AUDIO_SIMD_RESAMPLER_H