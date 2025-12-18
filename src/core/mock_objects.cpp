#include "core/mock_objects.h"

namespace core {
namespace mock {

std::string MockConfigManager::get_string(const std::string& key, const std::string& default_value) const {
    // 查找已存在的配置项
    for (const auto& pair : string_config_) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    return default_value;
}

int MockConfigManager::get_int(const std::string& key, int default_value) const {
    // 查找已存在的配置项
    for (const auto& pair : int_config_) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    return default_value;
}

void MockConfigManager::set_string(const std::string& key, const std::string& value) {
    // 查找是否已存在该键
    for (auto& pair : string_config_) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    // 如果不存在，添加新项
    string_config_.emplace_back(key, value);
}

void MockConfigManager::set_int(const std::string& key, int value) {
    // 查找是否已存在该键
    for (auto& pair : int_config_) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    // 如果不存在，添加新项
    int_config_.emplace_back(key, value);
}

} // namespace mock
} // namespace core