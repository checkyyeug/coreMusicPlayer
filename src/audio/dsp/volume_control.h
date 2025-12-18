#ifndef AUDIO_DSP_VOLUME_CONTROL_H
#define AUDIO_DSP_VOLUME_CONTROL_H

#include <cstdint>

namespace audio {
namespace dsp {

/**
 * @brief 音量控制类，用于数字音频的音量调节
 */
class VolumeControl {
public:
    /**
     * @brief 构造函数
     */
    VolumeControl();

    /**
     * @brief 析构函数
     */
    ~VolumeControl();

    /**
     * @brief 设置线性音量（0.0 - 1.0）
     * @param volume 音量值，范围[0.0, 1.0]
     */
    void setLinearVolume(float volume);

    /**
     * @brief 设置对数音量（dB）
     * @param dB 音量值，单位为分贝
     */
    void setLogarithmicVolume(float dB);

    /**
     * @brief 获取当前线性音量
     * @return 当前音量值
     */
    float getLinearVolume() const;

    /**
     * @brief 获取当前对数音量
     * @return 当前音量值（dB）
     */
    float getLogarithmicVolume() const;

    /**
     * @brief 应用音量到音频数据
     * @param data 音频数据指针
     * @param size 数据大小（采样数）
     * @param channels 通道数
     */
    void applyVolume(float* data, size_t size, int channels);

    /**
     * @brief 应用音量到16位整型音频数据
     * @param data 音频数据指针
     * @param size 数据大小（采样数）
     * @param channels 通道数
     */
    void applyVolume(int16_t* data, size_t size, int channels);

    /**
     * @brief 设置是否启用无失真控制
     * @param enabled 是否启用
     */
    void setNoDistortion(bool enabled);

    /**
     * @brief 检查是否启用无失真控制
     * @return 如果启用返回true，否则返回false
     */
    bool isNoDistortionEnabled() const;

private:
    /// 当前线性音量值
    float linear_volume_;
    
    /// 当前对数音量值（dB）
    float logarithmic_volume_;
    
    /// 是否启用无失真控制
    bool no_distortion_enabled_;
    
    /// 最大音量限制（用于无失真控制）
    static constexpr float MAX_LINEAR_VOLUME = 1.0f;
    
    /// 音量转换常数
    static constexpr float DB_MIN = -60.0f;
    static constexpr float DB_MAX = 0.0f;
    
    /**
     * @brief 将分贝值转换为线性值
     * @param dB 分贝值
     * @return 线性音量值
     */
    float dbToLinear(float dB) const;
    
    /**
     * @brief 将线性值转换为分贝值
     * @param linear 线性音量值
     * @return 分贝值
     */
    float linearToDb(float linear) const;
};

} // namespace dsp
} // namespace audio

#endif // AUDIO_DSP_VOLUME_CONTROL_H