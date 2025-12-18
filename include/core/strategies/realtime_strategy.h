#ifndef CORE_STRATEGIES_REALTIME_STRATEGY_H
#define CORE_STRATEGIES_REALTIME_STRATEGY_H

#include "core/strategies/audio_processing_strategy.h"

namespace core {

// 实时音频处理策略
class RealtimeStrategy : public AudioProcessingStrategy {
public:
    RealtimeStrategy();
    ~RealtimeStrategy() override = default;
    
    // 实现音频处理接口
    void processAudio(float* buffer, size_t frames) override;
    std::string getName() const override;
    bool isRealtime() const override;
    
private:
    // 实时处理逻辑
    void applyRealtimeEffects(float* buffer, size_t frames);
};

} // namespace core

#endif // CORE_STRATEGIES_REALTIME_STRATEGY_H