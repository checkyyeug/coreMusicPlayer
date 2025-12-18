#ifndef AUDIO_DSP_EQUALIZER_H
#define AUDIO_DSP_EQUALIZER_H

#include <vector>
#include <memory>

namespace audio {
namespace dsp {

// 均衡器类
class Equalizer {
public:
    Equalizer();
    ~Equalizer() = default;
    
    // 设置频段增益
    void setGain(int band, float gain);
    float getGain(int band) const;
    
    // 应用均衡器到音频数据
    void applyEqualization(float* buffer, size_t frames) const;
    
    // 重置为默认设置
    void reset();
    
    // 获取频段数量
    static constexpr int NUM_BANDS = 10;

private:
    std::vector<float> gains_;
};

} // namespace dsp
} // namespace audio

#endif // AUDIO_DSP_EQUALIZER_H