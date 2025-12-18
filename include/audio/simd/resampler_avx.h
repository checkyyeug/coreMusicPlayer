#ifndef AUDIO_SIMD_RESAMPLER_AVX_H
#define AUDIO_SIMD_RESAMPLER_AVX_H

#include "audio/sample_rate_converter.h"
#include <immintrin.h>

namespace audio {
namespace simd {

// AVX优化的重采样器实现
class ResamplerAVX : public SampleRateConverter {
public:
    // 构造函数
    ResamplerAVX();
    
    // 析构函数
    ~ResamplerAVX() override;
    
    // 重采样音频数据
    std::vector<float> resample(const std::vector<float>& input_data,
                               uint32_t input_sample_rate,
                               uint32_t output_sample_rate,
                               uint16_t channels) override;

private:
    // AVX优化的重采样算法
    void resample_avx(const float* input, float* output,
                     size_t input_size, size_t output_size,
                     uint16_t channels);
};

} // namespace simd
} // namespace audio

#endif // AUDIO_SIMD_RESAMPLER_AVX_H