#ifndef FOOBAR_PLUGIN_MANAGER_H
#define FOOBAR_PLUGIN_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

// Forward declarations for Foobar2000 types (simplified)
struct foobar2000_component_api;
class input_decoder;
class output;
class dsp_chain_entry;

namespace core {

// Foobar2000插件管理器
class FoobarPluginManager {
public:
    static FoobarPluginManager& instance();

    // Plugin lifecycle
    bool initialize(const std::string& pluginPath);
    void shutdown();

    // Plugin discovery
    std::vector<std::string> discoverPlugins(const std::string& path);
    bool loadPlugin(const std::string& filename);
    void unloadPlugin(const std::string& filename);
    void reloadAllPlugins();

    // Component access
    std::shared_ptr<input_decoder> getInputDecoder(const std::string& extension);
    std::shared_ptr<dsp_chain_entry> getDSPProcessor(const std::string& name);
    std::shared_ptr<output> getOutputPlugin(const std::string& name);

    // Configuration
    void setPluginPath(const std::string& path);
    std::string getPluginPath() const;

    // Events
    using PluginEventCallback = std::function<void(const std::string&)>;
    void onPluginLoaded(PluginEventCallback callback);
    void onPluginUnloaded(PluginEventCallback callback);

    // 获取可用插件列表
    std::vector<std::string> getAvailablePlugins() const;

    // 获取已加载插件列表
    std::vector<std::string> getLoadedPlugins() const;

private:
    struct PluginInfo {
        std::string filename;
        std::string path;
        bool loaded;
    };

    FoobarPluginManager() = default;
    void scanPlugins();
    bool isValidPlugin(const std::string& path);
    bool initializeCoreServices();

    std::map<std::string, void*> loadedPlugins_;
    std::map<std::string, std::shared_ptr<void>> components_;
    std::vector<PluginInfo> availablePlugins_;
    std::string pluginPath_;
    std::vector<PluginEventCallback> loadedCallbacks_;
    std::vector<PluginEventCallback> unloadedCallbacks_;
};

} // namespace core

#endif // FOOBAR_PLUGIN_MANAGER_H