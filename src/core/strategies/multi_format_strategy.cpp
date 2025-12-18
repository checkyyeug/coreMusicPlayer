#include "core/strategies/multi_format_strategy.h"
#include <iostream>

namespace core {

MultiFormatStrategy::MultiFormatStrategy() {
    // 初始化多格式策略
}

void MultiFormatStrategy::processAudio(float* buffer, size_t frames) {
    std::cout << "Processing audio with multi-format strategy" << std::endl;
    
    // 应用多格式效果
    applyMultiFormatEffects(buffer, frames);
}

std::string MultiFormatStrategy::getName() const {
    return "Multi-Format Strategy";
}

bool MultiFormatStrategy::isRealtime() const {
    return true;  // 多格式策略支持实时处理
}

void MultiFormatStrategy::applyMultiFormatEffects(float* buffer, size_t frames) {
    // 实现多格式音频效果处理逻辑
    for (size_t i = 0; i < frames; ++i) {
        // 应用适用于多种格式的音频效果
        buffer[i] *= 1.05f;  // 增益提升
    }
}

} // namespace core