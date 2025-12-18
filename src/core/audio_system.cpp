#include "core/audio_system.h"
#include <iostream>

namespace core {

AudioSystem::AudioSystem() 
    : initialized_(false),
      player_manager_(std::make_unique<AudioPlayerManager>()),
      decoder_factory_(std::make_unique<AudioDecoderFactory>()),
      resampler_factory_(std::make_unique<AudioResamplerFactory>()) {
    // 初始化音频系统
}

AudioSystem::~AudioSystem() {
    // 析构函数
    shutdown();
}

bool AudioSystem::initialize() {
    std::cout << "Initializing audio system" << std::endl;
    
    // 在实际实现中，这里会初始化整个音频系统
    
    if (player_manager_ && player_manager_->initialize()) {
        initialized_ = true;
        return true;
    }
    
    return false;
}

void AudioSystem::shutdown() {
    if (initialized_) {
        std::cout << "Shutting down audio system" << std::endl;
        
        // 在实际实现中，这里会关闭整个音频系统
        
        player_manager_.reset();
        decoder_factory_.reset();
        resampler_factory_.reset();
        initialized_ = false;
    }
}

AudioPlayerManager& AudioSystem::getPlayerManager() {
    return *player_manager_;
}

AudioDecoderFactory& AudioSystem::getDecoderFactory() {
    return *decoder_factory_;
}

AudioResamplerFactory& AudioSystem::getResamplerFactory() {
    return *resampler_factory_;
}

} // namespace core