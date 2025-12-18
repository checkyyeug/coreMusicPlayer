#ifndef CORE_AUDIO_SYSTEM_H
#define CORE_AUDIO_SYSTEM_H

#include "core/audio_player_manager.h"
#include "core/audio_decoder_factory.h"
#include "core/audio_resampler_factory.h"
#include <memory>

namespace core {

// 音频系统主类
class AudioSystem {
public:
    // 构造函数
    AudioSystem();
    
    // 析构函数
    ~AudioSystem();
    
    // 初始化音频系统
    bool initialize();
    
    // 关闭音频系统
    void shutdown();
    
    // 获取播放器管理器
    AudioPlayerManager& getPlayerManager();
    
    // 获取解码器工厂
    AudioDecoderFactory& getDecoderFactory();
    
    // 获取重采样器工厂
    AudioResamplerFactory& getResamplerFactory();
    
private:
    // 私有成员变量
    bool initialized_;
    std::unique_ptr<AudioPlayerManager> player_manager_;
    std::unique_ptr<AudioDecoderFactory> decoder_factory_;
    std::unique_ptr<AudioResamplerFactory> resampler_factory_;
};

} // namespace core

#endif // CORE_AUDIO_SYSTEM_H