#include <gtest/gtest.h>
#include "audio/audio_buffer.h"

TEST(AudioBufferTest, Construction) {
    audio::AudioBuffer buffer(2, 1024); // stereo, 1024 samples
    
    EXPECT_EQ(buffer.channels(), 2);
    EXPECT_EQ(buffer.samples(), 1024);
    EXPECT_EQ(buffer.total_samples(), 2048);
}

TEST(AudioBufferTest, ChannelData) {
    audio::AudioBuffer buffer(2, 1024); // stereo, 1024 samples
    
    float* left_channel = buffer.channel_data(0);
    float* right_channel = buffer.channel_data(1);
    
    EXPECT_NE(left_channel, nullptr);
    EXPECT_NE(right_channel, nullptr);
    EXPECT_NE(left_channel, right_channel);
}

TEST(AudioBufferTest, Clear) {
    audio::AudioBuffer buffer(2, 1024); // stereo, 1024 samples
    
    // Fill with some data
    float* left = buffer.channel_data(0);
    for (size_t i = 0; i < 1024; ++i) {
        left[i] = 1.0f;
    }
    
    // Clear the buffer
    buffer.clear();
    
    // Check that all values are zero
    for (size_t i = 0; i < 1024; ++i) {
        EXPECT_FLOAT_EQ(left[i], 0.0f);
    }
}

TEST(AudioBufferTest, Resize) {
    audio::AudioBuffer buffer(2, 1024); // stereo, 1024 samples
    
    buffer.resize(1, 512); // mono, 512 samples
    
    EXPECT_EQ(buffer.channels(), 1);
    EXPECT_EQ(buffer.samples(), 512);
    EXPECT_EQ(buffer.total_samples(), 512);
}