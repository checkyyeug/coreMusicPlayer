#ifndef CORE_FEATURE_MANAGER_H
#define CORE_FEATURE_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace core {

/**
 * @brief 特性管理器类，用于管理播放器的各种特性
 */
class FeatureManager {
public:
    /**
     * @brief 构造函数
     */
    FeatureManager();

    /**
     * @brief 析构函数
     */
    ~FeatureManager();

    /**
     * @brief 注册一个新特性
     * @param featureName 特性名称
     * @param enabled 是否启用该特性
     * @return 是否注册成功
     */
    bool registerFeature(const std::string& featureName, bool enabled = true);

    /**
     * @brief 启用指定特性
     * @param featureName 特性名称
     * @return 是否启用成功
     */
    bool enableFeature(const std::string& featureName);

    /**
     * @brief 禁用指定特性
     * @param featureName 特性名称
     * @return 是否禁用成功
     */
    bool disableFeature(const std::string& featureName);

    /**
     * @brief 检查指定特性是否启用
     * @param featureName 特性名称
     * @return 如果启用返回true，否则返回false
     */
    bool isFeatureEnabled(const std::string& featureName) const;

    /**
     * @brief 获取所有已注册的特性列表
     * @return 特性名称列表
     */
    std::vector<std::string> getAllFeatures() const;

    /**
     * @brief 获取启用的特性列表
     * @return 启用的特性名称列表
     */
    std::vector<std::string> getEnabledFeatures() const;

    /**
     * @brief 保存特性配置到文件
     * @param filename 配置文件路径
     * @return 是否保存成功
     */
    bool saveConfiguration(const std::string& filename) const;

    /**
     * @brief 从文件加载特性配置
     * @param filename 配置文件路径
     * @return 是否加载成功
     */
    bool loadConfiguration(const std::string& filename);

private:
    /// 特性状态映射表
    std::unordered_map<std::string, bool> features_;
};

} // namespace core

#endif // CORE_FEATURE_MANAGER_H