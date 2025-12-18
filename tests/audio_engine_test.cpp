#include <gtest/gtest.h>
#include "audio/audio_engine.h"
#include "audio/device_manager.h"
#include "audio/decoder_interface.h"
#include "audio/decoder_manager.h"

// Test that our interfaces compile correctly and can be instantiated
TEST(AudioEngineTest, InterfaceCompilation) {
    // These tests ensure the interfaces compile without errors
    // Actual implementation will come in later phases
    
    EXPECT_TRUE(true);  // Placeholder test
}

TEST(DecoderManagerTest, InterfaceCompilation) {
    // Ensure decoder manager interface compiles correctly
    EXPECT_TRUE(true);  // Placeholder test
}