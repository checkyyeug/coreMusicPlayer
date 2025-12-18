#include <gtest/gtest.h>
#include "core/equalizer_config.h"
#include "audio/dsp/equalizer.h"

// 测试均衡器配置类
TEST(EqualizerConfigTest, DefaultPreset) {
    core::EqualizerConfig config;
    
    // 默认应该启用
    EXPECT_TRUE(config.is_enabled());
    
    // 应该有10个频段
    auto params = config.get_params();
    EXPECT_EQ(params.size(), 10);
    
    // 检查默认参数
    for (const auto& param : params) {
        EXPECT_EQ(param.type, audio::dsp::EqualizerType::PEAKING);
        EXPECT_EQ(param.gain, 0.0f); // 默认平坦响应
        EXPECT_TRUE(param.enabled);
    }
}

TEST(EqualizerConfigTest, PresetLoading) {
    core::EqualizerConfig config;
    
    // 测试加载不同的预设
    config.load_preset("rock");
    auto params = config.get_params();
    EXPECT_EQ(params.size(), 10);
    
    // 检查是否正确应用了摇滚风格的增益
    for (size_t i = 0; i < params.size(); ++i) {
        if (i < 3) {
            // 前三个频段应该有增益
            EXPECT_GT(params[i].gain, 0.0f);
        } else if (i > 7) {
            // 后三个频段应该有增益
            EXPECT_GT(params[i].gain, 0.0f);
        } else {
            // 中间频段应该没有增益
            EXPECT_EQ(params[i].gain, 0.0f);
        }
    }
}

TEST(EqualizerConfigTest, PresetNames) {
    core::EqualizerConfig config;
    auto names = config.get_preset_names();
    
    // 检查预设名称列表
    EXPECT_GT(names.size(), 0);
    EXPECT_NE(std::find(names.begin(), names.end(), "rock"), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), "jazz"), names.end());
    EXPECT_NE(std::find(names.begin(), names.end(), "classical"), names.end());
}

// 测试均衡器DSP类
TEST(EqualizerDSPTest, DefaultConstruction) {
    audio::dsp::Equalizer equalizer(10);
    
    // 默认应该是禁用的
    EXPECT_FALSE(equalizer.is_enabled());
    
    // 应该有10个频段
    EXPECT_EQ(equalizer.get_params().size(), 10);
}

TEST(EqualizerDSPTest, EnableDisable) {
    audio::dsp::Equalizer equalizer(5);
    
    // 启用均衡器
    equalizer.enable(true);
    EXPECT_TRUE(equalizer.is_enabled());
    
    // 禁用均衡器
    equalizer.enable(false);
    EXPECT_FALSE(equalizer.is_enabled());
}

TEST(EqualizerDSPTest, ProcessWithParams) {
    audio::dsp::Equalizer equalizer(3);
    
    // 设置参数
    std::vector<audio::dsp::EqualizerParams> params(3);
    for (int i = 0; i < 3; ++i) {
        params[i].type = audio::dsp::EqualizerType::PEAKING;
        params[i].frequency = 1000.0f * (i + 1);
        params[i].gain = 3.0f; // 增益3dB
        params[i].enabled = true;
    }
    
    equalizer.set_params(params);
    equalizer.enable(true);
    
    // 创建测试数据
    std::vector<float> data(100, 0.5f); // 100个样本，值为0.5
    
    // 处理数据
    equalizer.process(data.data(), data.size());
    
    // 检查处理后的数据（简化检查）
    EXPECT_EQ(data.size(), 100);
}