#include "foobar_plugin_manager.h"
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

namespace core {

FoobarPluginManager& FoobarPluginManager::instance() {
    static FoobarPluginManager instance;
    return instance;
}

bool FoobarPluginManager::initialize(const std::string& pluginPath) {
    pluginPath_ = pluginPath;
    
    // 初始化Foobar2000核心服务
    if (!initializeCoreServices()) {
        return false;
    }
    
    // 扫描插件
    scanPlugins();
    
    return true;
}

void FoobarPluginManager::shutdown() {
    // 卸载所有已加载的插件
    for (auto& plugin : loadedPlugins_) {
#ifdef _WIN32
        FreeLibrary(static_cast<HMODULE>(plugin.second));
#endif
    }
    loadedPlugins_.clear();
    components_.clear();
}

void FoobarPluginManager::scanPlugins() {
    // 检查插件目录是否存在
    if (!std::filesystem::exists(pluginPath_)) {
        std::cout << "Plugin directory does not exist: " << pluginPath_ << std::endl;
        return;
    }
    
    // 遍历目录中的DLL文件
    for (const auto& entry : std::filesystem::directory_iterator(pluginPath_)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            std::string filename = entry.path().filename().string();
            
            // 检查是否为有效的Foobar2000插件
            if (isValidPlugin(entry.path().string())) {
                std::cout << "Found Foobar2000 plugin: " << filename << std::endl;
                // 可以在这里加载插件，但为了简化，我们只记录发现的插件
            }
        }
    }
}

bool FoobarPluginManager::isValidPlugin(const std::string& path) {
    // 简化的插件验证逻辑
    // 实际实现中应该检查DLL是否包含Foobar2000组件接口
    return true; // 暂时返回true，实际项目中需要更复杂的验证
}

bool FoobarPluginManager::initializeCoreServices() {
    // 初始化核心服务
    std::cout << "Initializing Foobar2000 core services..." << std::endl;
    return true;
}

std::vector<std::string> FoobarPluginManager::discoverPlugins(const std::string& path) {
    std::vector<std::string> plugins;
    
    try {
        if (std::filesystem::exists(path)) {
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                    plugins.push_back(entry.path().filename().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error discovering plugins: " << e.what() << std::endl;
    }
    
    return plugins;
}

bool FoobarPluginManager::loadPlugin(const std::string& filename) {
    std::string fullPath = pluginPath_ + "\\" + filename;
    
    // 检查文件是否存在
    if (!std::filesystem::exists(fullPath)) {
        std::cerr << "Plugin file not found: " << fullPath << std::endl;
        return false;
    }
    
#ifdef _WIN32
    // 加载DLL
    HMODULE hModule = LoadLibraryA(fullPath.c_str());
    if (!hModule) {
        std::cerr << "Failed to load plugin: " << filename << std::endl;
        return false;
    }
    
    // 存储已加载的插件
    loadedPlugins_[filename] = hModule;
    
    // 通知回调函数
    for (auto& callback : loadedCallbacks_) {
        callback(filename);
    }
    
    std::cout << "Successfully loaded plugin: " << filename << std::endl;
#endif
    
    return true;
}

void FoobarPluginManager::unloadPlugin(const std::string& filename) {
#ifdef _WIN32
    auto it = loadedPlugins_.find(filename);
    if (it != loadedPlugins_.end()) {
        FreeLibrary(static_cast<HMODULE>(it->second));
        loadedPlugins_.erase(it);
        
        // 通知回调函数
        for (auto& callback : unloadedCallbacks_) {
            callback(filename);
        }
        
        std::cout << "Successfully unloaded plugin: " << filename << std::endl;
    }
#endif
}

void FoobarPluginManager::reloadAllPlugins() {
    // 卸载所有插件
    shutdown();
    
    // 重新加载所有插件
    scanPlugins();
}

void FoobarPluginManager::setPluginPath(const std::string& path) {
    pluginPath_ = path;
}

std::string FoobarPluginManager::getPluginPath() const {
    return pluginPath_;
}

void FoobarPluginManager::onPluginLoaded(PluginEventCallback callback) {
    loadedCallbacks_.push_back(callback);
}

void FoobarPluginManager::onPluginUnloaded(PluginEventCallback callback) {
    unloadedCallbacks_.push_back(callback);
}

} // namespace core