#ifndef AUDIO_SIMD_RESAMPLER_AVX_H
#define AUDIO_SIMD_RESAMPLER_AVX_H

#include "audio/simd/resampler.h"

namespace audio {
namespace simd {

// AVX优化的重采样器
class ResamplerAVX : public Resampler {
public:
    ResamplerAVX();
    ~ResamplerAVX() override = default;
    
    // 实现重采样接口
    void resample(const float* input, size_t input_frames,
                  float* output, size_t output_frames) override;
    
    std::string getName() const override;
    bool isSupported() const override;

private:
    // AVX优化的重采样实现
    void resampleAVX(const float* input, size_t input_frames,
                     float* output, size_t output_frames);
};

} // namespace simd
} // namespace audio

#endif // AUDIO_SIMD_RESAMPLER_AVX_H