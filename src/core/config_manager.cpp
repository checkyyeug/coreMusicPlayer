#include "core/config_manager.h"
#include <fstream>
#include <iostream>

namespace core {

std::shared_ptr<ConfigManager> ConfigManager::instance() {
    static std::shared_ptr<ConfigManager> config =
        std::make_shared<ConfigManager>();
    return config;
}

bool ConfigManager::load_config(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return false;
    }

    Json::CharReaderBuilder builder;
    Json::parseFromStream(builder, file, &root_, nullptr);
    return true;
}

bool ConfigManager::save_config(const std::string& file_path) const {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        return false;
    }

    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root_, &file);
    return true;
}

std::string ConfigManager::get_string(const std::string& key,
                                    const std::string& default_value) const {
    const Json::Value& value = root_[key];
    if (value.isString()) {
        return value.asString();
    }
    return default_value;
}

int ConfigManager::get_int(const std::string& key, int default_value) const {
    const Json::Value& value = root_[key];
    if (value.isInt()) {
        return value.asInt();
    }
    return default_value;
}

double ConfigManager::get_double(const std::string& key, double default_value) const {
    const Json::Value& value = root_[key];
    if (value.isDouble()) {
        return value.asDouble();
    }
    return default_value;
}

bool ConfigManager::get_bool(const std::string& key, bool default_value) const {
    const Json::Value& value = root_[key];
    if (value.isBool()) {
        return value.asBool();
    }
    return default_value;
}

void ConfigManager::set_string(const std::string& key, const std::string& value) {
    root_[key] = value;
}

void ConfigManager::set_int(const std::string& key, int value) {
    root_[key] = value;
}

void ConfigManager::set_double(const std::string& key, double value) {
    root_[key] = value;
}

void ConfigManager::set_bool(const std::string& key, bool value) {
    root_[key] = value;
}

bool ConfigManager::has_key(const std::string& key) const {
    return root_.isMember(key);
}

bool ConfigManager::reload_config() {
    // 获取当前配置文件路径
    // 这里简化处理，实际项目中需要保存原始文件路径
    return false;  // 暂时返回false，因为没有保存原文件路径
}

} // namespace core