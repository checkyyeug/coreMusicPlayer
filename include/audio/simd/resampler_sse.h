#ifndef AUDIO_SIMD_RESAMPLER_SSE_H
#define AUDIO_SIMD_RESAMPLER_SSE_H

#include "audio/simd/resampler.h"

namespace audio {
namespace simd {

// SSE优化的重采样器
class ResamplerSSE : public Resampler {
public:
    ResamplerSSE();
    ~ResamplerSSE() override = default;
    
    // 实现重采样接口
    void resample(const float* input, size_t input_frames,
                  float* output, size_t output_frames) override;
    
    std::string getName() const override;
    bool isSupported() const override;

private:
    // SSE优化的重采样实现
    void resampleSSE(const float* input, size_t input_frames,
                    float* output, size_t output_frames);
};

} // namespace simd
} // namespace audio

#endif // AUDIO_SIMD_RESAMPLER_SSE_H