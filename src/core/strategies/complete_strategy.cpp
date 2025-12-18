#include "core/strategies/complete_strategy.h"
#include <iostream>

namespace core {

CompleteStrategy::CompleteStrategy() {
    // 初始化完整策略
}

void CompleteStrategy::processAudio(float* buffer, size_t frames) {
    std::cout << "Processing audio with complete strategy" << std::endl;
    
    // 应用完整效果
    applyCompleteEffects(buffer, frames);
}

std::string CompleteStrategy::getName() const {
    return "Complete Strategy";
}

bool CompleteStrategy::isRealtime() const {
    return true;  // 完整策略支持实时处理
}

void CompleteStrategy::applyCompleteEffects(float* buffer, size_t frames) {
    // 实现完整的音频效果处理逻辑
    for (size_t i = 0; i < frames; ++i) {
        // 应用一些基础的音频效果
        buffer[i] *= 1.1f;  // 简单增益提升
    }
}

} // namespace core