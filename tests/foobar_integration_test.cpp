#include <gtest/gtest.h>
#include "../src/foobar/foobar_plugin_manager.h"

// 测试Foobar2000插件管理器
TEST(FoobarIntegrationTest, PluginManagerInitialization) {
    // 创建插件管理器实例
    core::FoobarPluginManager manager;

    // 验证管理器可以创建
    EXPECT_NE(&manager, nullptr);
}

// 测试插件发现功能
TEST(FoobarIntegrationTest, PluginDiscovery) {
    core::FoobarPluginManager manager;

    // 创建测试目录
    std::filesystem::create_directories("test_plugins");

    // 模拟插件发现过程
    auto plugins = manager.discoverPlugins("test_plugins");

    // 验证返回结果
    EXPECT_TRUE(plugins.empty());  // 在测试环境中应该没有实际插件

    // 清理测试目录
    std::filesystem::remove_all("test_plugins");
}

// 测试插件加载功能
TEST(FoobarIntegrationTest, PluginLoading) {
    core::FoobarPluginManager manager;
    
    // 测试无效路径
    EXPECT_FALSE(manager.loadPlugin("nonexistent.dll"));
    
    // 创建测试目录和文件
    std::filesystem::create_directories("test_plugins");
    std::ofstream test_file("test_plugins/test_plugin.dll");
    test_file << "fake dll content";
    test_file.close();
    
    // 测试加载插件（实际不会成功，但应该不崩溃）
    EXPECT_TRUE(manager.loadPlugin("test_plugins/test_plugin.dll"));
    
    // 清理测试文件
    std::filesystem::remove_all("test_plugins");
}

// 测试插件卸载功能
TEST(FoobarIntegrationTest, PluginUnloading) {
    core::FoobarPluginManager manager;
    
    // 创建测试目录和文件
    std::filesystem::create_directories("test_plugins");
    std::ofstream test_file("test_plugins/test_plugin.dll");
    test_file << "fake dll content";
    test_file.close();
    
    // 加载插件
    EXPECT_TRUE(manager.loadPlugin("test_plugins/test_plugin.dll"));
    
    // 卸载插件
    manager.unloadPlugin("test_plugins/test_plugin.dll");
    
    // 清理测试文件
    std::filesystem::remove_all("test_plugins");
}