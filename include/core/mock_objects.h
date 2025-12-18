#ifndef CORE_MOCK_OBJECTS_H
#define CORE_MOCK_OBJECTS_H

#include <string>
#include <vector>

// Mock对象用于测试，这里提供一些基本的mock类定义

namespace core {
namespace mock {

// Mock音频缓冲区
class MockAudioBuffer {
public:
    MockAudioBuffer(size_t size = 0) : size_(size) {}
    
    void resize(size_t new_size) { size_ = new_size; }
    size_t size() const { return size_; }
    
private:
    size_t size_;
};

// Mock配置管理器
class MockConfigManager {
public:
    // 模拟获取配置值
    std::string get_string(const std::string& key, const std::string& default_value = "") const;
    int get_int(const std::string& key, int default_value = 0) const;
    
    // 模拟设置配置值
    void set_string(const std::string& key, const std::string& value);
    void set_int(const std::string& key, int value);
    
private:
    // 存储模拟的配置数据
    mutable std::vector<std::pair<std::string, std::string>> string_config_;
    mutable std::vector<std::pair<std::string, int>> int_config_;
};

} // namespace mock
} // namespace core

#endif // CORE_MOCK_OBJECTS_H