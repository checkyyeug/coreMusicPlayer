#ifndef CORE_STRATEGIES_MULTI_FORMAT_STRATEGY_H
#define CORE_STRATEGIES_MULTI_FORMAT_STRATEGY_H

#include "core/strategies/audio_processing_strategy.h"

namespace core {

// 多格式音频处理策略
class MultiFormatStrategy : public AudioProcessingStrategy {
public:
    MultiFormatStrategy();
    ~MultiFormatStrategy() override = default;
    
    // 实现音频处理接口
    void processAudio(float* buffer, size_t frames) override;
    std::string getName() const override;
    bool isRealtime() const override;
    
private:
    // 多格式处理逻辑
    void applyMultiFormatEffects(float* buffer, size_t frames);
};

} // namespace core

#endif // CORE_STRATEGIES_MULTI_FORMAT_STRATEGY_H