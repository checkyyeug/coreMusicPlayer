#include <gtest/gtest.h>
#include "core/result.h"
#include "core/error.h"
#include "core/config_manager.h"

// 测试Result类型功能
TEST(ResultTest, SuccessValue) {
    core::Result<int> result(42);

    EXPECT_TRUE(result.has_value());
    EXPECT_FALSE(result.is_error());
    EXPECT_EQ(result.value(), 42);
}

TEST(ResultTest, ErrorValue) {
    core::Result<int> result("Something went wrong");

    EXPECT_FALSE(result.has_value());
    EXPECT_TRUE(result.is_error());
    EXPECT_EQ(result.error(), "Something went wrong");
}

// 测试Error类功能
TEST(ErrorTest, Construction) {
    core::Error error(core::ErrorCategory::AudioInitialization,
                      "Failed to initialize audio device");

    EXPECT_EQ(error.category(), core::ErrorCategory::AudioInitialization);
    EXPECT_EQ(error.message(), "Failed to initialize audio device");
}

// 测试配置管理器功能
TEST(ConfigManagerTest, ConfigManagerCreation) {
    // 创建配置管理器实例
    auto config_manager = core::ConfigManager::instance();
    
    // 验证管理器可以创建
    EXPECT_NE(config_manager, nullptr);
}

TEST(ConfigManagerTest, DefaultConfigLoading) {
    // 创建配置管理器实例
    auto config_manager = core::ConfigManager::instance();
    
    // 加载默认配置
    EXPECT_TRUE(config_manager->load_default_config());
    
    // 验证配置加载成功
    EXPECT_FALSE(config_manager->get_string("unknown_key", "").empty());
}

// 测试配置验证功能
TEST(ConfigManagerTest, ConfigValidation) {
    // 创建配置管理器实例
    auto config_manager = core::ConfigManager::instance();
    
    // 加载默认配置
    EXPECT_TRUE(config_manager->load_default_config());
    
    // 验证配置项存在
    EXPECT_FALSE(config_manager->get_string("audio_device", "").empty());
}