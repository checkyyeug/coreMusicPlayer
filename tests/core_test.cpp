#include <gtest/gtest.h>
#include "core/result.h"
#include "core/error.h"

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

TEST(ErrorTest, Construction) {
    core::Error error(core::ErrorCategory::AudioInitialization, 
                      "Failed to initialize audio device");
    
    EXPECT_EQ(error.category(), core::ErrorCategory::AudioInitialization);
    EXPECT_EQ(error.message(), "Failed to initialize audio device");
}