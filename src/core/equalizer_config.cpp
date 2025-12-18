#include "core/equalizer_config.h"
#include <algorithm>
#include <iostream>

namespace core {

// 预设配置名称列表
const std::vector<std::string> EqualizerConfig::preset_names_ = {
    "flat",      // 平坦响应
    "rock",      // 摇滚
    "jazz",      // 爵士
    "classical", // 古典
    "pop",       // 流行
    "dance",     // 舞曲
    "bass_boost" // 低音增强
};

// 构造函数
EqualizerConfig::EqualizerConfig() : enabled_(false) {
    load_default_preset();
}

// 加载默认预设
void EqualizerConfig::load_default_preset() {
    // 创建平坦响应的均衡器配置（无增益）
    params_.clear();
    for (int i = 0; i < 10; ++i) {
        audio::dsp::EqualizerParams param;
        param.type = audio::dsp::EqualizerType::PEAKING;
        param.frequency = 100.0f * (i + 1); // 频率从100Hz到1000Hz
        param.gain = 0.0f; // 无增益
        param.q_factor = 1.0f;
        param.enabled = true;
        params_.push_back(param);
    }
}

// 加载预设配置
void EqualizerConfig::load_preset(const std::string& preset_name) {
    // 根据名称设置不同的均衡器配置
    if (preset_name == "rock") {
        // 摇滚风格：增强低频和高频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i < 3) {
                // 增强低频
                param.gain = 3.0f; 
            } else if (i > 7) {
                // 增强高频
                param.gain = 2.0f;
            } else {
                // 中频保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else if (preset_name == "jazz") {
        // 爵士风格：增强中频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i >= 3 && i <= 6) {
                // 增强中频
                param.gain = 2.0f;
            } else {
                // 其他频率保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else if (preset_name == "classical") {
        // 古典风格：增强高频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i > 6) {
                // 增强高频
                param.gain = 2.0f;
            } else {
                // 其他频率保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else if (preset_name == "pop") {
        // 流行风格：增强低频和中频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i < 4) {
                // 增强低频
                param.gain = 2.0f;
            } else if (i >= 4 && i <= 6) {
                // 增强中频
                param.gain = 1.0f;
            } else {
                // 高频保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else if (preset_name == "dance") {
        // 舞曲风格：增强低频和高频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i < 3 || i > 7) {
                // 增强低频和高频
                param.gain = 2.0f;
            } else {
                // 中频保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else if (preset_name == "bass_boost") {
        // 低音增强：大幅增强低频
        params_.clear();
        for (int i = 0; i < 10; ++i) {
            audio::dsp::EqualizerParams param;
            param.type = audio::dsp::EqualizerType::PEAKING;
            param.frequency = 100.0f * (i + 1);
            if (i < 4) {
                // 增强低频
                param.gain = 6.0f;
            } else {
                // 其他频率保持平坦
                param.gain = 0.0f;
            }
            param.q_factor = 1.0f;
            param.enabled = true;
            params_.push_back(param);
        }
    } else {
        // 默认平坦响应
        load_default_preset();
    }
}

// 保存当前配置
void EqualizerConfig::save_config(const std::string& config_name) const {
    // 实际项目中这里会将配置保存到文件或数据库
    std::cout << "Saving equalizer config: " << config_name << std::endl;
}

// 获取当前均衡器参数
const std::vector<audio::dsp::EqualizerParams>& EqualizerConfig::get_params() const {
    return params_;
}

// 设置均衡器参数
void EqualizerConfig::set_params(const std::vector<audio::dsp::EqualizerParams>& params) {
    params_ = params;
}

// 启用/禁用均衡器
void EqualizerConfig::enable(bool enabled) {
    enabled_ = enabled;
}

// 检查是否启用
bool EqualizerConfig::is_enabled() const {
    return enabled_;
}

// 获取预设配置列表
std::vector<std::string> EqualizerConfig::get_preset_names() const {
    return preset_names_;
}

} // namespace core