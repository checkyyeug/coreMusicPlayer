#ifndef PLATFORM_WINDOWS_WASAPI_OUTPUT_H
#define PLATFORM_WINDOWS_WASAPI_OUTPUT_H

#include <memory>
#include <string>

namespace platform {
namespace windows {

// Windows WASAPI音频输出类
class WasapiOutput {
public:
    // 构造函数
    WasapiOutput();
    
    // 析构函数
    ~WasapiOutput();

    // 初始化音频输出
    bool initialize(const std::string& device_name = "");
    
    // 清理资源
    void cleanup();
    
    // 播放音频数据
    bool play_audio(const void* data, size_t size);
    
    // 停止播放
    bool stop();
    
    // 设置音量（0.0 - 1.0）
    bool set_volume(float volume);
    
    // 获取状态
    std::string get_status() const;
    
    // 检查是否正在播放
    bool is_playing() const;

private:
    // 私有数据成员
    bool initialized_;
    bool playing_;
    float volume_;
    std::string device_name_;
};

} // namespace windows
} // namespace platform

#endif // PLATFORM_WINDOWS_WASAPI_OUTPUT_H