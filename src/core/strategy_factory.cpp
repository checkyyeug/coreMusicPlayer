#include "core/strategy_factory.h"
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

} // namespace core