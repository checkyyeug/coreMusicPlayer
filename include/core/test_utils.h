#ifndef CORE_TEST_UTILS_H
#define CORE_TEST_UTILS_H

#include <string>
#include <vector>

namespace core {
namespace test {

// 测试辅助工具类
class TestUtils {
public:
    // 生成测试音频数据
    static std::vector<float> generate_test_audio_data(size_t size, float amplitude = 0.5f);
    
    // 验证两个浮点数是否相等（考虑精度误差）
    static bool nearly_equal(float a, float b, float epsilon = 1e-6f);
    
    // 创建临时文件路径
    static std::string create_temp_file_path(const std::string& prefix = "test_");
};

} // namespace test
} // namespace core

#endif // CORE_TEST_UTILS_H