#include "feature_manager.h"
#include <fstream>
#include <iostream>

namespace core {

FeatureManager::FeatureManager() {
    // 初始化默认特性
    registerFeature("audio_output", true);
    registerFeature("equalizer", true);
    registerFeature("volume_control", true);
    registerFeature("effects", false);
    registerFeature("visualizer", false);
}

FeatureManager::~FeatureManager() = default;

bool FeatureManager::registerFeature(const std::string& featureName, bool enabled) {
    if (features_.find(featureName) != features_.end()) {
        return false; // 特性已存在
    }
    
    features_[featureName] = enabled;
    return true;
}

bool FeatureManager::enableFeature(const std::string& featureName) {
    auto it = features_.find(featureName);
    if (it == features_.end()) {
        return false; // 特性不存在
    }
    
    it->second = true;
    return true;
}

bool FeatureManager::disableFeature(const std::string& featureName) {
    auto it = features_.find(featureName);
    if (it == features_.end()) {
        return false; // 特性不存在
    }
    
    it->second = false;
    return true;
}

bool FeatureManager::isFeatureEnabled(const std::string& featureName) const {
    auto it = features_.find(featureName);
    if (it == features_.end()) {
        return false; // 特性不存在
    }
    
    return it->second;
}

std::vector<std::string> FeatureManager::getAllFeatures() const {
    std::vector<std::string> result;
    for (const auto& pair : features_) {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<std::string> FeatureManager::getEnabledFeatures() const {
    std::vector<std::string> result;
    for (const auto& pair : features_) {
        if (pair.second) {
            result.push_back(pair.first);
        }
    }
    return result;
}

bool FeatureManager::saveConfiguration(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& pair : features_) {
        file << pair.first << "=" << (pair.second ? "true" : "false") << "\n";
    }
    
    file.close();
    return true;
}

bool FeatureManager::loadConfiguration(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            bool enabled = (value == "true");
            features_[name] = enabled;
        }
    }
    
    file.close();
    return true;
}

} // namespace core