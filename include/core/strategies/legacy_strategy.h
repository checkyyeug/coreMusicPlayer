#ifndef CORE_STRATEGIES_LEGACY_STRATEGY_H
#define CORE_STRATEGIES_LEGACY_STRATEGY_H

#include "core/strategies/audio_processing_strategy.h"

namespace core {

// 传统音频处理策略
class LegacyStrategy : public AudioProcessingStrategy {
public:
    LegacyStrategy();
    ~LegacyStrategy() override = default;
    
    // 实现音频处理接口
    void processAudio(float* buffer, size_t frames) override;
    std::string getName() const override;
    bool isRealtime() const override;
    
private:
    // 传统处理逻辑
    void applyLegacyEffects(float* buffer, size_t frames);
};

} // namespace core

#endif // CORE_STRATEGIES_LEGACY_STRATEGY_H