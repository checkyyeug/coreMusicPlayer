#include "core/strategy_factory.h"
#include "core/strategies/legacy_strategy.h"
#include "core/strategies/complete_strategy.h"
#include "core/strategies/realtime_strategy.h"
#include "core/strategies/production_strategy.h"
#include "core/strategies/multi_format_strategy.h"
#include <algorithm>

namespace core {

std::shared_ptr<StrategyFactory> StrategyFactory::instance() {
    static std::shared_ptr<StrategyFactory> factory =
        std::make_shared<StrategyFactory>();
    return factory;
}

void StrategyFactory::register_strategy(const std::string& name,
                                      std::function<std::unique_ptr<PlayerStrategy>()> creator) {
    creators_[name] = creator;
}

std::unique_ptr<PlayerStrategy> StrategyFactory::create_strategy(const std::string& name) const {
    auto it = creators_.find(name);
    if (it != creators_.end()) {
        return it->second();
    }
    return nullptr;  // 策略未找到
}

std::vector<std::string> StrategyFactory::get_registered_strategies() const {
    std::vector<std::string> names;
    for (const auto& pair : creators_) {
        names.push_back(pair.first);
    }
    return names;
}

// 初始化函数，注册所有可用的策略
void StrategyFactory::initialize_default_strategies() {
    register_strategy("legacy", []() { return std::make_unique<LegacyStrategy>(); });
    register_strategy("complete", []() { return std::make_unique<CompleteStrategy>(); });
    register_strategy("realtime", []() { return std::make_unique<RealtimeStrategy>(); });
    register_strategy("production", []() { return std::make_unique<ProductionStrategy>(); });
    register_strategy("multi_format", []() { return std::make_unique<MultiFormatStrategy>(); });
}

} // namespace core