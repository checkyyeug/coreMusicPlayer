#include "core/config_manager.h"
#include <fstream>
#include <sstream>

namespace core {

ConfigManager& ConfigManager::instance() {
    static ConfigManager instance;
    return instance;
}

void ConfigManager::setConfig(const std::string& key, const std::string& value) {
    configs_[key] = value;
}

std::string ConfigManager::getConfig(const std::string& key) const {
    auto it = configs_.find(key);
    if (it != configs_.end()) {
        return it->second;
    }
    return "";
}

bool ConfigManager::hasConfig(const std::string& key) const {
    return configs_.find(key) != configs_.end();
}

std::map<std::string, std::string> ConfigManager::getAllConfigs() const {
    return configs_;
}

void ConfigManager::removeConfig(const std::string& key) {
    configs_.erase(key);
}

void ConfigManager::resetToDefault() {
    configs_.clear();
    // 设置默认配置
    configs_["audio_buffer_size"] = "1024";
    configs_["sample_rate"] = "44100";
    configs_["channels"] = "2";
    configs_["volume"] = "1.0";
    configs_["plugin_path"] = "./plugins";
    configs_["log_level"] = "INFO";
    configs_["enable_performance_monitoring"] = "true";
}

bool ConfigManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    configs_.clear();
    std::string line;
    while (std::getline(file, line)) {
        // 简单的键值对解析
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            configs_[key] = value;
        }
    }
    
    return true;
}

bool ConfigManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& pair : configs_) {
        file << pair.first << "=" << pair.second << "\n";
    }
    
    return true;
}

} // namespace core