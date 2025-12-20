#ifndef CORE_STRATEGY_FACTORY_H
#define CORE_STRATEGY_FACTORY_H

#include <memory>
#include <string>
#include <vector>

namespace core {

// 前向声明策略类
class PlayerStrategy;

// 策略工厂类
class StrategyFactory {
public:
    static StrategyFactory& instance();
    
    // 创建指定类型的播放策略
    std::unique_ptr<PlayerStrategy> createStrategy(const std::string& strategyType);
    
    // 获取所有支持的策略类型
    std::vector<std::string> getSupportedStrategies() const;

private:
    StrategyFactory() = default;
};

} // namespace core

#endif // CORE_STRATEGY_FACTORY_H