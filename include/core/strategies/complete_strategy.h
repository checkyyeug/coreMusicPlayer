#ifndef CORE_STRATEGIES_COMPLETE_STRATEGY_H
#define CORE_STRATEGIES_COMPLETE_STRATEGY_H

#include "core/strategies/audio_processing_strategy.h"

namespace core {

// 完整音频处理策略
class CompleteStrategy : public AudioProcessingStrategy {
public:
    CompleteStrategy();
    ~CompleteStrategy() override = default;
    
    // 实现音频处理接口
    void processAudio(float* buffer, size_t frames) override;
    std::string getName() const override;
    bool isRealtime() const override;
    
private:
    // 完整处理逻辑
    void applyCompleteEffects(float* buffer, size_t frames);
};

} // namespace core

#endif // CORE_STRATEGIES_COMPLETE_STRATEGY_H