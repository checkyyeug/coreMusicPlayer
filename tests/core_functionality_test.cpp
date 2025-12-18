#include <gtest/gtest.h>
#include "core/unified_music_player.h"
#include "core/strategy_factory.h"
#include "core/strategies/legacy_strategy.h"
#include "core/strategies/complete_strategy.h"
#include "audio/audio_engine.h"
#include "audio/device_manager.h"

// 测试统一播放器功能
TEST(CoreFunctionalityTest, UnifiedMusicPlayerCreation) {
    // 测试单例模式
    auto player1 = core::UnifiedMusicPlayer::instance();
    auto player2 = core::UnifiedMusicPlayer::instance();
    
    EXPECT_EQ(player1, player2);
}

TEST(CoreFunctionalityTest, StrategyFactoryRegistration) {
    // 测试策略工厂注册功能
    auto factory = core::StrategyFactory::instance();
    
    // 初始化默认策略
    factory->initialize_default_strategies();
    
    // 获取已注册的策略列表
    auto strategies = factory->get_registered_strategies();
    
    // 验证所有策略都已注册
    EXPECT_GT(strategies.size(), 0);
    EXPECT_TRUE(std::find(strategies.begin(), strategies.end(), "legacy") != strategies.end());
    EXPECT_TRUE(std::find(strategies.begin(), strategies.end(), "complete") != strategies.end());
    EXPECT_TRUE(std::find(strategies.begin(), strategies.end(), "realtime") != strategies.end());
    EXPECT_TRUE(std::find(strategies.begin(), strategies.end(), "production") != strategies.end());
    EXPECT_TRUE(std::find(strategies.begin(), strategies.end(), "multi_format") != strategies.end());
}

TEST(CoreFunctionalityTest, StrategyCreation) {
    // 测试策略创建功能
    auto factory = core::StrategyFactory::instance();
    factory->initialize_default_strategies();
    
    // 创建各种策略
    auto legacy_strategy = factory->create_strategy("legacy");
    auto complete_strategy = factory->create_strategy("complete");
    auto realtime_strategy = factory->create_strategy("realtime");
    auto production_strategy = factory->create_strategy("production");
    auto multi_format_strategy = factory->create_strategy("multi_format");
    
    // 验证策略创建成功
    EXPECT_NE(legacy_strategy, nullptr);
    EXPECT_NE(complete_strategy, nullptr);
    EXPECT_NE(realtime_strategy, nullptr);
    EXPECT_NE(production_strategy, nullptr);
    EXPECT_NE(multi_format_strategy, nullptr);
}

TEST(CoreFunctionalityTest, StrategySwitching) {
    // 测试策略切换功能
    auto player = core::UnifiedMusicPlayer::instance();
    
    // 初始化播放器
    EXPECT_TRUE(player->initialize());
    
    // 切换到Legacy策略
    EXPECT_TRUE(player->set_strategy("legacy"));
    EXPECT_EQ(player->get_active_strategy_name(), "legacy");
    
    // 切换到Complete策略
    EXPECT_TRUE(player->set_strategy("complete"));
    EXPECT_EQ(player->get_active_strategy_name(), "complete");
    
    // 切换到Realtime策略
    EXPECT_TRUE(player->set_strategy("realtime"));
    EXPECT_EQ(player->get_active_strategy_name(), "realtime");
}

TEST(CoreFunctionalityTest, AudioEngineInitialization) {
    // 测试音频引擎初始化
    auto engine = audio::AudioEngine::instance();
    
    // 初始化音频引擎
    EXPECT_TRUE(engine->initialize());
    
    // 验证引擎状态
    EXPECT_EQ(engine->get_status(), "Stopped");
    
    // 清理资源
    engine->cleanup();
}

TEST(CoreFunctionalityTest, DeviceManagerInitialization) {
    // 测试设备管理器初始化
    auto device_manager = audio::DeviceManager::instance();
    
    // 初始化设备管理器
    EXPECT_TRUE(device_manager->initialize());
    
    // 验证设备列表
    auto devices = device_manager->get_devices();
    EXPECT_GT(devices.size(), 0);
    
    // 清理资源
    device_manager->cleanup();
}

TEST(CoreFunctionalityTest, PlayerStrategyInterface) {
    // 测试播放器策略接口实现
    auto factory = core::StrategyFactory::instance();
    factory->initialize_default_strategies();
    
    auto strategy = factory->create_strategy("legacy");
    
    // 验证策略接口方法
    EXPECT_TRUE(strategy->initialize());
    EXPECT_EQ(strategy->get_name(), "Legacy");
    
    // 测试状态获取
    EXPECT_FALSE(strategy->is_playing());
    EXPECT_EQ(strategy->get_status(), "Stopped");
}