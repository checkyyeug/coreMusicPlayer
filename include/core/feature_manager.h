#ifndef CORE_FEATURE_MANAGER_H
#define CORE_FEATURE_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace core {

// 功能管理器类
class FeatureManager {
public:
    static FeatureManager& instance();

    // 功能开关控制
    void enableFeature(const std::string& featureName);
    void disableFeature(const std::string& featureName);
    bool isFeatureEnabled(const std::string& featureName) const;

    // 获取所有功能状态
    std::map<std::string, bool> getAllFeatures() const;
    
    // 重置所有功能为默认状态
    void resetToDefault();

private:
    FeatureManager() = default;
    
    // 功能状态映射
    std::map<std::string, bool> features_;
};

} // namespace core

#endif // CORE_FEATURE_MANAGER_H