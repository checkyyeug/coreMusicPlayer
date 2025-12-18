#ifndef CORE_EQUALIZER_CONFIG_H
#define CORE_EQUALIZER_CONFIG_H

#include <vector>
#include <string>

namespace core {

// 均衡器配置类
class EqualizerConfig {
public:
    // 频段数量
    static const int NUM_BANDS = 10;
    
    // 构造函数
    EqualizerConfig();
    
    // 获取和设置各个频段的增益值
    float getGain(int band) const;
    void setGain(int band, float gain);
    
    // 获取所有频段增益值
    std::vector<float> getAllGains() const;
    
    // 设置所有频段增益值
    void setAllGains(const std::vector<float>& gains);
    
    // 重置为默认配置
    void resetToDefault();
    
    // 获取均衡器名称
    std::string getName() const;
    void setName(const std::string& name);
    
    // 获取预设配置
    static EqualizerConfig getPresets(const std::string& presetName);

private:
    std::vector<float> gains_;
    std::string name_;
};

} // namespace core

#endif // CORE_EQUALIZER_CONFIG_H