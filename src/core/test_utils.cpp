#include "core/test_utils.h"
#include <random>
#include <fstream>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace core {
namespace test {

std::vector<float> TestUtils::generate_test_audio_data(size_t size, float amplitude) {
    std::vector<float> data(size);
    
    // 生成正弦波测试数据
    for (size_t i = 0; i < size; ++i) {
        data[i] = amplitude * static_cast<float>(sin(2.0 * M_PI * i / size));
    }
    
    return data;
}

bool TestUtils::nearly_equal(float a, float b, float epsilon) {
    return std::abs(a - b) < epsilon;
}

std::string TestUtils::create_temp_file_path(const std::string& prefix) {
#ifdef _WIN32
    char temp_path[MAX_PATH];
    if (GetTempPathA(MAX_PATH, temp_path)) {
        // 生成随机文件名
        char random_name[10];
        for (int i = 0; i < 8; ++i) {
            random_name[i] = 'a' + rand() % 26;
        }
        random_name[8] = '\0';
        
        return std::string(temp_path) + prefix + random_name + ".tmp";
    }
#else
    // Linux/macOS 使用 /tmp 目录
    return "/tmp/" + prefix + std::to_string(rand()) + ".tmp";
#endif
    
    // 如果平台特定方法失败，返回默认路径
    return "./" + prefix + "temp.tmp";
}

} // namespace test
} // namespace core