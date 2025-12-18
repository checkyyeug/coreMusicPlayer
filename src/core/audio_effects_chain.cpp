#include "core/audio_effects_chain.h"
#include <iostream>

namespace core {

AudioEffectsChain::AudioEffectsChain() 
    : initialized_(false) {
    // 初始化音频效果链
}

AudioEffectsChain::~AudioEffectsChain() {
    // 析构函数
    shutdown();
}

bool AudioEffectsChain::initialize() {
    std::cout << "Initializing audio effects chain" << std::endl;
    
    // 在实际实现中，这里会初始化效果链
    
    initialized_ = true;
    return true;
}

void AudioEffectsChain::shutdown() {
    if (initialized_) {
        std::cout << "Shutting down audio effects chain" << std::endl;
        
        // 在实际实现中，这里会关闭效果链
        
        clear();
        initialized_ = false;
    }
}

bool AudioEffectsChain::addEffect(std::unique_ptr<AudioFilter> effect) {
    if (!initialized_) {
        return false;
    }
    
    if (!effect) {
        return false;
    }
    
    std::cout << "Adding effect to chain: " << effect->getName() << std::endl;
    
    // 在实际实现中，这里会添加效果到链中
    
    effects_.push_back(std::move(effect));
    enabled_.push_back(true);
    return true;
}

bool AudioEffectsChain::removeEffect(size_t index) {
    if (!initialized_ || index >= effects_.size()) {
        return false;
    }
    
    std::cout << "Removing effect from chain at index: " << index << std::endl;
    
    // 在实际实现中，这里会移除效果
    
    effects_.erase(effects_.begin() + index);
    enabled_.erase(enabled_.begin() + index);
    return true;
}

bool AudioEffectsChain::apply(const AudioBuffer& input, AudioBuffer& output) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Applying audio effects chain" << std::endl;
    
    // 在实际实现中，这里会应用效果链
    
    output = input;  // 模拟处理
    
    // 应用所有启用的效果
    for (size_t i = 0; i < effects_.size(); ++i) {
        if (enabled_[i]) {
            std::cout << "Applying effect: " << effects_[i]->getName() << std::endl;
            effects_[i]->apply(output, output);
        }
    }
    
    return true;
}

void AudioEffectsChain::clear() {
    if (initialized_) {
        std::cout << "Clearing audio effects chain" << std::endl;
        
        // 在实际实现中，这里会清空效果链
        
        effects_.clear();
        enabled_.clear();
    }
}

size_t AudioEffectsChain::getEffectCount() const {
    return effects_.size();
}

std::unique_ptr<AudioFilter> AudioEffectsChain::getEffect(size_t index) const {
    if (index >= effects_.size()) {
        return nullptr;
    }
    
    return std::unique_ptr<AudioFilter>(effects_[index].release());
}

bool AudioEffectsChain::enableEffect(size_t index, bool enabled) {
    if (!initialized_ || index >= enabled_.size()) {
        return false;
    }
    
    std::cout << "Setting effect at index " << index << " to " 
              << (enabled ? "enabled" : "disabled") << std::endl;
    
    // 在实际实现中，这里会启用/禁用效果
    
    enabled_[index] = enabled;
    return true;
}

bool AudioEffectsChain::isEffectEnabled(size_t index) const {
    if (index >= enabled_.size()) {
        return false;
    }
    
    return enabled_[index];
}

} // namespace core