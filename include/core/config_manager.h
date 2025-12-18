#ifndef CORE_CONFIG_MANAGER_H
#define CORE_CONFIG_MANAGER_H

#include <string>
#include <memory>

// 包含JSON库头文件
#include <json/json.h> // 假设使用JsonCpp库

namespace core {

class ConfigManager {
public:
    // 获取单例实例
    static std::shared_ptr<ConfigManager> instance();

    // 加载配置文件
    bool load_config(const std::string& file_path);

    // 保存配置到文件
    bool save_config(const std::string& file_path) const;

    // 获取字符串值
    std::string get_string(const std::string& key,
                          const std::string& default_value = "") const;

    // 获取整数值
    int get_int(const std::string& key, int default_value = 0) const;

    // 获取浮点数值
    double get_double(const std::string& key, double default_value = 0.0) const;

    // 获取布尔值
    bool get_bool(const std::string& key, bool default_value = false) const;

    // 设置字符串值
    void set_string(const std::string& key, const std::string& value);

    // 设置整数值
    void set_int(const std::string& key, int value);

    // 设置浮点数值
    void set_double(const std::string& key, double value);

    // 设置布尔值
    void set_bool(const std::string& key, bool value);

    // 检查键是否存在
    bool has_key(const std::string& key) const;

    // 热重载配置（如果支持）
    bool reload_config();

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    Json::Value root_;
};

} // namespace core

#endif // CORE_CONFIG_MANAGER_H