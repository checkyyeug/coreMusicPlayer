#ifndef AUDIO_DEVICE_MANAGER_H
#define AUDIO_DEVICE_MANAGER_H

#include <string>
#include <vector>
#include "audio/audio_format.h"

namespace audio {

// 音频设备管理器类
class DeviceManager {
public:
    // 获取单例实例
    static std::shared_ptr<DeviceManager> instance();
    
    // 枚举音频设备
    bool enumerate_devices();
    
    // 获取输出设备列表
    std::vector<std::string> get_output_devices() const;
    
    // 获取输入设备列表
    std::vector<std::string> get_input_devices() const;
    
    // 选择输出设备
    bool select_output_device(const std::string& device_name);
    
    // 选择输入设备
    bool select_input_device(const std::string& device_name);
    
    // 获取当前输出设备
    std::string get_current_output_device() const;
    
    // 获取当前输入设备
    std::string get_current_input_device() const;
    
    // 检查设备是否可用
    bool is_device_available(const std::string& device_name) const;
    
private:
    DeviceManager();
    ~DeviceManager() = default;
    
    // 设备信息结构体
    struct AudioDeviceInfo {
        std::string name;
        std::string description;
        bool is_output;
        bool is_input;
        bool is_available;
    };
    
    // 已枚举的设备列表
    std::vector<AudioDeviceInfo> output_devices_;
    std::vector<AudioDeviceInfo> input_devices_;
    
    // 当前选择的设备
    std::string current_output_device_;
    std::string current_input_device_;
};

} // namespace audio

#endif // AUDIO_DEVICE_MANAGER_H