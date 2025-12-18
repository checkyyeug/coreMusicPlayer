#ifndef AUDIO_DSP_VOLUME_CONTROL_H
#define AUDIO_DSP_VOLUME_CONTROL_H

#include <memory>

namespace audio {
namespace dsp {

// 音量控制类
class VolumeControl {
public:
    VolumeControl();
    ~VolumeControl() = default;
    
    // 设置音量（0.0到2.0）
    void setVolume(float volume);
    
    // 获取当前音量
    float getVolume() const;
    
    // 应用音量控制到音频数据
    void applyVolume(float* buffer, size_t frames) const;
    
    // 静音/取消静音
    void mute();
    void unmute();
    bool isMuted() const;

private:
    float volume_;
    bool muted_;
};

} // namespace dsp
} // namespace audio

#endif // AUDIO_DSP_VOLUME_CONTROL_H