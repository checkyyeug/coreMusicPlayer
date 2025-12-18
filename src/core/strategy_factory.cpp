#include "core/strategy_factory.h"
#include <vector>
#include <memory>

// 前向声明具体策略类
class LegacyStrategy;
class CompleteStrategy;
class RealtimeStrategy;
class ProductionStrategy;
class MultiFormatStrategy;

namespace core {

StrategyFactory& StrategyFactory::instance() {
    static StrategyFactory instance;
    return instance;
}

std::unique_ptr<AudioProcessingStrategy> StrategyFactory::createStrategy(
    const std::string& strategyType) {
    
    if (strategyType == "legacy") {
        return std::make_unique<LegacyStrategy>();
    } else if (strategyType == "complete") {
        return std::make_unique<CompleteStrategy>();
    } else if (strategyType == "realtime") {
        return std::make_unique<RealtimeStrategy>();
    } else if (strategyType == "production") {
        return std::make_unique<ProductionStrategy>();
    } else if (strategyType == "multi_format") {
        return std::make_unique<MultiFormatStrategy>();
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
        "multi_format"
    };
}

} // namespace core