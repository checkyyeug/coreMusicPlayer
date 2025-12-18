#include "core/audio_resampler_factory.h"
#include "core/audio_resampler.h"
#include <iostream>

namespace core {

std::unique_ptr<AudioResampler> AudioResamplerFactory::createAudioResampler() {
    std::cout << "Creating default audio resampler" << std::endl;
    
    // 在实际实现中，这里会根据平台或配置创建特定类型的重采样器
    
    return nullptr;  // 模拟返回值
}

std::unique_ptr<AudioResampler> AudioResamplerFactory::createAudioResampler(const std::string& type) {
    std::cout << "Creating audio resampler of type: " << type << std::endl;
    
    // 在实际实现中，这里会根据类型创建特定类型的重采样器
    
    return nullptr;  // 模拟返回值
}

} // namespace core