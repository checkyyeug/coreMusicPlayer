#include "core/strategy_factory.h"
#include "core/legacy_strategy.h"
#include "core/complete_strategy.h"
#include "core/realtime_strategy.h"
#include "core/production_strategy.h"
// #include "core/strategies/multi_format_strategy.h"
#include <vector>
#include <memory>

namespace core {

StrategyFactory& StrategyFactory::instance() {
    static StrategyFactory instance;
    return instance;
}

std::unique_ptr<core::PlayerStrategy> StrategyFactory::createStrategy(
    const std::string& strategyType) {
    
    if (strategyType == "legacy") {
        return std::make_unique<LegacyStrategy>();
    } else if (strategyType == "complete") {
        return std::make_unique<CompleteStrategy>();
    } else if (strategyType == "realtime") {
        return std::make_unique<RealtimeStrategy>();
    } else if (strategyType == "production") {
        return std::make_unique<ProductionStrategy>();
    // } else if (strategyType == "multi_format") {
    //     return std::make_unique<MultiFormatStrategy>();
    }
    
    // 默认返回完整策略
    return std::make_unique<CompleteStrategy>();
}

std::vector<std::string> StrategyFactory::getSupportedStrategies() const {
    return {
        "legacy",
        "complete", 
        "realtime",
        "production",
        // "multi_format"
    };
}

} // namespace core