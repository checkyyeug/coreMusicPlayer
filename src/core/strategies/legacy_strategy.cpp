#include "core/strategies/legacy_strategy.h"
#include <iostream>

namespace core {

LegacyStrategy::LegacyStrategy() {
    // 初始化传统策略
}

void LegacyStrategy::processAudio(float* buffer, size_t frames) {
    std::cout << "Processing audio with legacy strategy" << std::endl;
    
    // 应用传统效果
    applyLegacyEffects(buffer, frames);
}

std::string LegacyStrategy::getName() const {
    return "Legacy Strategy";
}

bool LegacyStrategy::isRealtime() const {
    return false;  // 传统策略通常不是实时的
}

void LegacyStrategy::applyLegacyEffects(float* buffer, size_t frames) {
    // 实现传统音频效果处理逻辑
    // 这里可以添加一些简单的音频处理算法
    for (size_t i = 0; i < frames; ++i) {
        // 简单的增益控制
        buffer[i] *= 1.05f;
    }
}

} // namespace core