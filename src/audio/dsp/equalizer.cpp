#include "audio/dsp/equalizer.h"
#include <cmath>
#include <algorithm>

namespace audio {
namespace dsp {

// 均衡器类型名称映射
std::string Equalizer::get_type_name(EqualizerType type) {
    switch (type) {
        case EqualizerType::NONE: return "None";
        case EqualizerType::LOW_PASS: return "Low Pass";
        case EqualizerType::HIGH_PASS: return "High Pass";
        case EqualizerType::BAND_PASS: return "Band Pass";
        case EqualizerType::PEAKING: return "Peaking";
        default: return "Unknown";
    }
}

// 构造函数
Equalizer::Equalizer(int num_bands) 
    : enabled_(false), 
      num_bands_(std::max(1, num_bands)) {
    init_filter_states();
}

// 初始化滤波器状态
void Equalizer::init_filter_states() {
    // 为每个频段初始化两个二阶滤波器的状态（biquad）
    filter_states_.resize(num_bands_);
    for (auto& state : filter_states_) {
        state.resize(4, 0.0f); // [x1, x2, y1, y2]
    }
}

// 设置均衡器参数
void Equalizer::set_params(const std::vector<EqualizerParams>& params) {
    params_ = params;
    
    // 确保参数数量与频段数匹配
    if (params_.size() < static_cast<size_t>(num_bands_)) {
        params_.resize(num_bands_);
    }
}

// 获取当前参数
const std::vector<EqualizerParams>& Equalizer::get_params() const {
    return params_;
}

// 启用/禁用均衡器
void Equalizer::enable(bool enabled) {
    enabled_ = enabled;
}

// 检查是否启用
bool Equalizer::is_enabled() const {
    return enabled_;
}

// 重置均衡器状态
void Equalizer::reset() {
    for (auto& state : filter_states_) {
        std::fill(state.begin(), state.end(), 0.0f);
    }
}

// 应用单个频段的均衡处理
void Equalizer::apply_band_processing(float* data, size_t length, const EqualizerParams& param, int band_index) {
    if (!param.enabled || !enabled_) return;
    
    // 简化的滤波器实现 - 实际项目中应使用更复杂的数字滤波器算法
    // 这里使用一个简单的增益控制作为示例
    
    float gain_factor = std::pow(10.0f, param.gain / 20.0f);
    
    // 应用增益到音频数据
    for (size_t i = 0; i < length; ++i) {
        data[i] *= gain_factor;
    }
}

// 处理音频数据
void Equalizer::process(float* data, size_t length) {
    if (!enabled_ || length == 0) return;
    
    // 如果没有设置参数，使用默认的单频段处理
    if (params_.empty()) {
        EqualizerParams default_param;
        default_param.enabled = true;
        default_param.gain = 0.0f; // 无增益
        apply_band_processing(data, length, default_param, 0);
        return;
    }
    
    // 为每个频段应用处理
    for (size_t i = 0; i < std::min(static_cast<size_t>(num_bands_), params_.size()); ++i) {
        apply_band_processing(data, length, params_[i], static_cast<int>(i));
    }
}

} // namespace dsp
} // namespace audio