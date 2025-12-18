#ifndef CORE_CONFIG_MANAGER_H
#define CORE_CONFIG_MANAGER_H

#include <string>
#include <map>
#include <memory>

namespace core {

// 配置管理器类
class ConfigManager {
public:
    static ConfigManager& instance();

    // 基本配置操作
    void setConfig(const std::string& key, const std::string& value);
    std::string getConfig(const std::string& key) const;
    bool hasConfig(const std::string& key) const;
    
    // 获取所有配置项
    std::map<std::string, std::string> getAllConfigs() const;
    
    // 删除配置项
    void removeConfig(const std::string& key);
    
    // 重置为默认配置
    void resetToDefault();
    
    // 加载配置文件
    bool loadFromFile(const std::string& filename);
    
    // 保存配置到文件
    bool saveToFile(const std::string& filename) const;

private:
    ConfigManager() = default;
    
    // 配置数据存储
    std::map<std::string, std::string> configs_;
};

} // namespace core

#endif // CORE_CONFIG_MANAGER_H