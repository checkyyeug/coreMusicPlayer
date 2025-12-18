#ifndef CORE_AUDIO_DECODER_FACTORY_H
#define CORE_AUDIO_DECODER_FACTORY_H

#include "core/audio_decoder.h"
#include <memory>

namespace core {

// 音频解码器工厂类
class AudioDecoderFactory {
public:
    // 创建音频解码器实例
    static std::unique_ptr<AudioDecoder> createAudioDecoder();
    
    // 创建特定类型的音频解码器
    static std::unique_ptr<AudioDecoder> createAudioDecoder(const std::string& type);
};

} // namespace core

#endif // CORE_AUDIO_DECODER_FACTORY_H