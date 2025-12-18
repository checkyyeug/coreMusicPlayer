#include "audio/device_manager.h"
#include <iostream>

namespace audio {

std::shared_ptr<DeviceManager> DeviceManager::instance() {
    static std::shared_ptr<DeviceManager> manager =
        std::make_shared<DeviceManager>();
    return manager;
}

DeviceManager::DeviceManager()
    : current_output_device_("default"),
      current_input_device_("default") {
}

bool DeviceManager::initialize() {
    // 初始化设备管理器
    std::cout << "Initializing device manager..." << std::endl;

    // 枚举音频设备
    if (!enumerate_devices()) {
        std::cerr << "Failed to enumerate audio devices" << std::endl;
        return false;
    }

    std::cout << "Device manager initialized successfully" << std::endl;
    return true;
}

void DeviceManager::cleanup() {
    // 清理设备资源
    output_devices_.clear();
    input_devices_.clear();
    std::cout << "Device manager cleaned up" << std::endl;
}

bool DeviceManager::enumerate_devices() {
    // 枚举音频设备（简化实现）
    std::cout << "Enumerating audio devices..." << std::endl;

    // 添加一些示例设备
    output_devices_.clear();
    input_devices_.clear();

    // 模拟输出设备
    output_devices_.push_back({"default", "Default Output Device", true, false, true});
    output_devices_.push_back({"speakers", "Speakers", true, false, true});
    output_devices_.push_back({"headphones", "Headphones", true, false, true});

    // 模拟输入设备
    input_devices_.push_back({"default", "Default Input Device", false, true, true});
    input_devices_.push_back({"microphone", "Microphone", false, true, true});

    std::cout << "Found " << output_devices_.size() << " output device(s) and "
              << input_devices_.size() << " input device(s)" << std::endl;
    return true;
}

std::vector<std::string> DeviceManager::get_output_devices() const {
    std::vector<std::string> names;
    for (const auto& device : output_devices_) {
        names.push_back(device.name);
    }
    return names;
}

std::vector<std::string> DeviceManager::get_input_devices() const {
    std::vector<std::string> names;
    for (const auto& device : input_devices_) {
        names.push_back(device.name);
    }
    return names;
}

bool DeviceManager::select_output_device(const std::string& device_name) {
    // 检查设备是否存在
    for (const auto& device : output_devices_) {
        if (device.name == device_name && device.is_available) {
            current_output_device_ = device_name;
            std::cout << "Selected output device: " << device_name << std::endl;
            return true;
        }
    }

    std::cout << "Output device not found or unavailable: " << device_name << std::endl;
    return false;
}

bool DeviceManager::select_input_device(const std::string& device_name) {
    // 检查设备是否存在
    for (const auto& device : input_devices_) {
        if (device.name == device_name && device.is_available) {
            current_input_device_ = device_name;
            std::cout << "Selected input device: " << device_name << std::endl;
            return true;
        }
    }

    std::cout << "Input device not found or unavailable: " << device_name << std::endl;
    return false;
}

std::string DeviceManager::get_current_output_device() const {
    return current_output_device_;
}

std::string DeviceManager::get_current_input_device() const {
    return current_input_device_;
}

bool DeviceManager::is_device_available(const std::string& device_name) const {
    // 检查输出设备
    for (const auto& device : output_devices_) {
        if (device.name == device_name && device.is_available) {
            return true;
        }
    }

    // 检查输入设备
    for (const auto& device : input_devices_) {
        if (device.name == device_name && device.is_available) {
            return true;
        }
    }

    return false;
}

} // namespace audio