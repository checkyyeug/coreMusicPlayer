#ifndef CORE_EQUALIZER_CONFIG_H
#define CORE_EQUALIZER_CONFIG_H

#include "audio/dsp/equalizer.h"
#include <vector>
#include <string>

namespace core {

// 均衡器配置类
class EqualizerConfig {
public:
    // 构造函数
    EqualizerConfig();
    
    // 析构函数
    ~EqualizerConfig() = default;
    
    // 设置预设均衡器配置
    void load_preset(const std::string& preset_name);
    
    // 保存当前配置
    void save_config(const std::string& config_name) const;
    
    // 获取当前均衡器参数
    const std::vector<audio::dsp::EqualizerParams>& get_params() const;
    
    // 设置均衡器参数
    void set_params(const std::vector<audio::dsp::EqualizerParams>& params);
    
    // 启用/禁用均衡器
    void enable(bool enabled);
    
    // 检查是否启用
    bool is_enabled() const;
    
    // 获取预设配置列表
    std::vector<std::string> get_preset_names() const;

private:
    // 当前均衡器参数
    std::vector<audio::dsp::EqualizerParams> params_;
    
    // 是否启用均衡器
    bool enabled_;
    
    // 预设配置
    static const std::vector<std::string> preset_names_;
    
    // 默认预设配置
    void load_default_preset();
};

} // namespace core

#endif // CORE_EQUALIZER_CONFIG_H