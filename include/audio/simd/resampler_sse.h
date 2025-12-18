#ifndef AUDIO_SIMD_RESAMPLER_SSE_H
#define AUDIO_SIMD_RESAMPLER_SSE_H

#include "audio/sample_rate_converter.h"
#include <immintrin.h>

namespace audio {
namespace simd {

// SSE优化的重采样器实现
class ResamplerSSE : public SampleRateConverter {
public:
    // 构造函数
    ResamplerSSE();
    
    // 析构函数
    ~ResamplerSSE() override;
    
    // 重采样音频数据
    std::vector<float> resample(const std::vector<float>& input_data,
                               uint32_t input_sample_rate,
                               uint32_t output_sample_rate,
                               uint16_t channels) override;

private:
    // SSE优化的重采样算法
    void resample_sse(const float* input, float* output,
                     size_t input_size, size_t output_size,
                     uint16_t channels);
};

} // namespace simd
} // namespace audio

#endif // AUDIO_SIMD_RESAMPLER_SSE_H