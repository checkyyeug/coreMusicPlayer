#ifndef CORE_AUDIO_RESAMPLER_FACTORY_H
#define CORE_AUDIO_RESAMPLER_FACTORY_H

#include "core/audio_resampler.h"
#include <memory>

namespace core {

// 音频重采样器工厂类
class AudioResamplerFactory {
public:
    // 创建音频重采样器实例
    static std::unique_ptr<AudioResampler> createAudioResampler();
    
    // 创建特定类型的音频重采样器
    static std::unique_ptr<AudioResampler> createAudioResampler(const std::string& type);
};

} // namespace core

#endif // CORE_AUDIO_RESAMPLER_FACTORY_H