#include "core/audio_decoder_factory.h"
#include "core/audio_decoder.h"
#include <iostream>

namespace core {

std::unique_ptr<AudioDecoder> AudioDecoderFactory::createAudioDecoder() {
    std::cout << "Creating default audio decoder" << std::endl;
    
    // 在实际实现中，这里会根据平台或配置创建特定类型的解码器
    
    return nullptr;  // 模拟返回值
}

std::unique_ptr<AudioDecoder> AudioDecoderFactory::createAudioDecoder(const std::string& type) {
    std::cout << "Creating audio decoder of type: " << type << std::endl;
    
    // 在实际实现中，这里会根据类型创建特定类型的解码器
    
    return nullptr;  // 模拟返回值
}

} // namespace core