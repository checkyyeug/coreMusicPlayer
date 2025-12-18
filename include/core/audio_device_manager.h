#ifndef CORE_AUDIO_DEVICE_MANAGER_H
#define CORE_AUDIO_DEVICE_MANAGER_H

#include <string>
#include <vector>
#include <memory>

namespace core {

// 音频设备管理器类
class AudioDeviceManager {
public:
    // 构造函数
    AudioDeviceManager();
    
    // 析构函数
    ~AudioDeviceManager();
    
    // 初始化设备管理器
    bool initialize();
    
    // 关闭设备管理器
    void shutdown();
    
    // 获取音频输入设备列表
    std::vector<std::string> getInputDevices() const;
    
    // 获取音频输出设备列表
    std::vector<std::string> getOutputDevices() const;
    
    // 设置当前输入设备
    bool setInputDevice(const std::string& device_name);
    
    // 设置当前输出设备
    bool setOutputDevice(const std::string& device_name);
    
    // 获取当前输入设备
    std::string getCurrentInputDevice() const;
    
    // 获取当前输出设备
    std::string getCurrentOutputDevice() const;
    
    // 测试音频设备
    bool testDevice(const std::string& device_name, bool is_input);
    
    // 获取设备信息
    void getDeviceInfo(const std::string& device_name, 
                       std::string& name, 
                       std::string& type,
                       int& sample_rate,
                       int& channels) const;
    
private:
    // 私有成员变量
    bool initialized_;
    std::string current_input_device_;
    std::string current_output_device_;
    std::vector<std::string> input_devices_;
    std::vector<std::string> output_devices_;
};

} // namespace core

#endif // CORE_AUDIO_DEVICE_MANAGER_H