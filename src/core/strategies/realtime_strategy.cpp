#include "core/strategies/realtime_strategy.h"
#include <iostream>

namespace core {

RealtimeStrategy::RealtimeStrategy() {
    // 初始化实时策略
}

void RealtimeStrategy::processAudio(float* buffer, size_t frames) {
    std::cout << "Processing audio with realtime strategy" << std::endl;
    
    // 应用实时效果
    applyRealtimeEffects(buffer, frames);
}

std::string RealtimeStrategy::getName() const {
    return "Realtime Strategy";
}

bool RealtimeStrategy::isRealtime() const {
    return true;  // 实时策略支持实时处理
}

void RealtimeStrategy::applyRealtimeEffects(float* buffer, size_t frames) {
    // 实现实时音频效果处理逻辑
    for (size_t i = 0; i < frames; ++i) {
        // 应用一些简单的实时效果
        buffer[i] *= 1.02f;  // 轻微增益提升
    }
}

} // namespace core