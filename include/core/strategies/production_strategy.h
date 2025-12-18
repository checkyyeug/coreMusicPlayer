#ifndef CORE_STRATEGIES_PRODUCTION_STRATEGY_H
#define CORE_STRATEGIES_PRODUCTION_STRATEGY_H

#include "core/strategies/audio_processing_strategy.h"

namespace core {

// 生产级音频处理策略
class ProductionStrategy : public AudioProcessingStrategy {
public:
    ProductionStrategy();
    ~ProductionStrategy() override = default;
    
    // 实现音频处理接口
    void processAudio(float* buffer, size_t frames) override;
    std::string getName() const override;
    bool isRealtime() const override;
    
private:
    // 生产级处理逻辑
    void applyProductionEffects(float* buffer, size_t frames);
};

} // namespace core

#endif // CORE_STRATEGIES_PRODUCTION_STRATEGY_H