#include <gtest/gtest.h>
#include "audio/audio_buffer.h"
#include "audio/audio_format.h"
#include "audio/sample_rate_converter.h"
#include "audio/dsp/volume_control.h"
#include "audio/dsp/equalizer.h"

// 测试音频缓冲区功能
TEST(AudioBufferTest, BufferCreation) {
    // 创建双声道1024样本的音频缓冲区
    audio::AudioBuffer buffer(2, 1024);
    
    // 验证缓冲区属性
    EXPECT_EQ(buffer.channels(), 2);
    EXPECT_EQ(buffer.samples(), 1024);
    EXPECT_EQ(buffer.total_samples(), 2048);
}

TEST(AudioBufferTest, ChannelDataAccess) {
    // 创建双声道音频缓冲区
    audio::AudioBuffer buffer(2, 1024);
    
    // 获取通道数据指针
    float* left_channel = buffer.channel_data(0);
    float* right_channel = buffer.channel_data(1);
    
    // 验证指针不为空且不同
    EXPECT_NE(left_channel, nullptr);
    EXPECT_NE(right_channel, nullptr);
    EXPECT_NE(left_channel, right_channel);
}

TEST(AudioBufferTest, BufferClear) {
    // 创建音频缓冲区并填充数据
    audio::AudioBuffer buffer(2, 1024);
    
    float* left = buffer.channel_data(0);
    for (size_t i = 0; i < 1024; ++i) {
        left[i] = 1.0f;
    }
    
    // 清理缓冲区
    buffer.clear();
    
    // 验证所有值都为零
    for (size_t i = 0; i < 1024; ++i) {
        EXPECT_FLOAT_EQ(left[i], 0.0f);
    }
}

TEST(AudioBufferTest, BufferResize) {
    // 创建初始缓冲区
    audio::AudioBuffer buffer(2, 1024);
    
    // 调整大小为单声道512样本
    buffer.resize(1, 512);
    
    // 验证调整后的属性
    EXPECT_EQ(buffer.channels(), 1);
    EXPECT_EQ(buffer.samples(), 512);
    EXPECT_EQ(buffer.total_samples(), 512);
}

// 测试音频格式功能
TEST(AudioFormatTest, FormatCreation) {
    audio::AudioFormat format;
    
    // 验证默认格式属性
    EXPECT_EQ(format.sample_rate, 44100);
    EXPECT_EQ(format.format, audio::SampleFormat::PCM_FLOAT);
    EXPECT_EQ(format.channels, audio::ChannelLayout::STEREO);
}

// 测试采样率转换器功能
TEST(SampleRateConverterTest, ConverterInitialization) {
    // 创建采样率转换器
    audio::SampleRateConverter converter;
    
    // 验证转换器可以创建
    EXPECT_NE(&converter, nullptr);
}

// 测试音量控制功能
TEST(VolumeControlTest, VolumeControlCreation) {
    // 创建音量控制器
    audio::VolumeControl volume_control;
    
    // 验证控制器可以创建
    EXPECT_NE(&volume_control, nullptr);
}

TEST(VolumeControlTest, VolumeSetting) {
    // 创建音量控制器
    audio::VolumeControl volume_control;
    
    // 测试设置有效音量值
    EXPECT_TRUE(volume_control.set_volume(0.5f));
    EXPECT_FLOAT_EQ(volume_control.get_volume(), 0.5f);
    
    // 测试设置边界值
    EXPECT_TRUE(volume_control.set_volume(0.0f));
    EXPECT_FLOAT_EQ(volume_control.get_volume(), 0.0f);
    
    EXPECT_TRUE(volume_control.set_volume(1.0f));
    EXPECT_FLOAT_EQ(volume_control.get_volume(), 1.0f);
}

// 测试均衡器功能
TEST(EqualizerTest, EqualizerCreation) {
    // 创建均衡器
    audio::Equalizer equalizer;
    
    // 验证均衡器可以创建
    EXPECT_NE(&equalizer, nullptr);
}

TEST(EqualizerTest, EqualizerBands) {
    // 创建5段均衡器
    audio::Equalizer equalizer(5);
    
    // 验证均衡器段数
    EXPECT_EQ(equalizer.get_band_count(), 5);
    
    // 测试设置和获取频段增益
    EXPECT_TRUE(equalizer.set_band_gain(0, 0.5f));
    EXPECT_FLOAT_EQ(equalizer.get_band_gain(0), 0.5f);
}