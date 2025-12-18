#include "core/feature_manager.h"
#include <algorithm>

namespace core {

FeatureManager& FeatureManager::instance() {
    static FeatureManager instance;
    return instance;
}

void FeatureManager::enableFeature(const std::string& featureName) {
    features_[featureName] = true;
}

void FeatureManager::disableFeature(const std::string& featureName) {
    features_[featureName] = false;
}

bool FeatureManager::isFeatureEnabled(const std::string& featureName) const {
    auto it = features_.find(featureName);
    if (it != features_.end()) {
        return it->second;
    }
    // 默认情况下禁用未定义的功能
    return false;
}

std::map<std::string, bool> FeatureManager::getAllFeatures() const {
    return features_;
}

void FeatureManager::resetToDefault() {
    features_.clear();
    // 设置默认功能状态
    features_["audio_processing"] = true;
    features_["network_features"] = false;
    features_["gui_features"] = false;
    features_["foobar_integration"] = true;
    features_["performance_monitoring"] = true;
}

} // namespace core