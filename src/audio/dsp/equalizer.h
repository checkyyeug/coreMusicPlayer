#ifndef AUDIO_DSP_EQUALIZER_H
#define AUDIO_DSP_EQUALIZER_H

#include <vector>
#include <array>
#include <cstdint>

namespace audio {
namespace dsp {

// 均衡器类型定义
enum class EqualizerType {
    NONE,      // 无均衡器
    LOW_PASS,  // 低通滤波器
    HIGH_PASS, // 高通滤波器
    BAND_PASS, // 带通滤波器
    PEAKING    // 峰值均衡器
};

// 均衡器参数结构体
struct EqualizerParams {
    EqualizerType type;
    float frequency;   // 中心频率 (Hz)
    float gain;        // 增益 (dB)
    float q_factor;    // Q因子 (品质因数)
    bool enabled;      // 是否启用

    EqualizerParams() : type(EqualizerType::NONE), 
                        frequency(0.0f), 
                        gain(0.0f), 
                        q_factor(1.0f), 
                        enabled(false) {}
};

// 均衡器类
class Equalizer {
public:
    // 构造函数
    explicit Equalizer(int num_bands = 10);
    
    // 析构函数
    ~Equalizer() = default;
    
    // 设置均衡器参数
    void set_params(const std::vector<EqualizerParams>& params);
    
    // 获取当前参数
    const std::vector<EqualizerParams>& get_params() const;
    
    // 启用/禁用均衡器
    void enable(bool enabled);
    
    // 检查是否启用
    bool is_enabled() const;
    
    // 处理音频数据
    void process(float* data, size_t length);
    
    // 重置均衡器状态
    void reset();
    
    // 获取均衡器类型名称
    static std::string get_type_name(EqualizerType type);

private:
    // 均衡器参数
    std::vector<EqualizerParams> params_;
    
    // 是否启用
    bool enabled_;
    
    // 均衡器带数
    int num_bands_;
    
    // 用于存储滤波器状态的缓冲区
    std::vector<std::vector<float>> filter_states_;
    
    // 初始化滤波器状态
    void init_filter_states();
    
    // 应用单个频段的均衡处理
    void apply_band_processing(float* data, size_t length, const EqualizerParams& param, int band_index);
};

} // namespace dsp
} // namespace audio

#endif // AUDIO_DSP_EQUALIZER_H