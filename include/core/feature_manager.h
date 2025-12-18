#ifndef CORE_FEATURE_MANAGER_H
#define CORE_FEATURE_MANAGER_H

#include <string>
#include <set>
#include <map>

namespace core {

// 特性管理器类，用于动态启用/禁用功能模块
class FeatureManager {
public:
    // 获取单例实例
    static std::shared_ptr<FeatureManager> instance();
    
    // 注册特性
    void register_feature(const std::string& name, bool default_enabled = true);
    
    // 启用特性
    void enable_feature(const std::string& name);
    
    // 禁用特性
    void disable_feature(const std::string& name);
    
    // 检查特性是否启用
    bool is_feature_enabled(const std::string& name) const;
    
    // 获取所有已注册的特性名称
    std::set<std::string> get_registered_features() const;
    
    // 获取当前启用的特性
    std::set<std::string> get_enabled_features() const;
    
    // 从配置加载特性状态
    bool load_from_config(const std::string& config_path);
    
    // 保存特性状态到配置
    bool save_to_config(const std::string& config_path) const;
    
private:
    FeatureManager();
    ~FeatureManager() = default;
    
    // 已注册的特性集合
    std::set<std::string> registered_features_;
    
    // 启用的特性集合
    std::set<std::string> enabled_features_;
    
    // 特性默认状态映射表
    std::map<std::string, bool> default_states_;
};

} // namespace core

#endif // CORE_FEATURE_MANAGER_H