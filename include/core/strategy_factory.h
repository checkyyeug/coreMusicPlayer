#ifndef CORE_STRATEGY_FACTORY_H
#define CORE_STRATEGY_FACTORY_H

#include <memory>
#include <string>
#include <functional>
#include "core/player_strategy.h"

namespace core {

// 策略工厂类，用于创建不同类型的播放策略
class StrategyFactory {
public:
    // 获取单例实例
    static std::shared_ptr<StrategyFactory> instance();
    
    // 注册策略创建函数
    void register_strategy(const std::string& name, 
                         std::function<std::unique_ptr<PlayerStrategy>()> creator);
    
    // 创建指定名称的播放策略
    std::unique_ptr<PlayerStrategy> create_strategy(const std::string& name) const;
    
    // 获取所有已注册的策略名称
    std::vector<std::string> get_registered_strategies() const;
    
private:
    StrategyFactory() = default;
    ~StrategyFactory() = default;
    
    // 策略创建函数映射表
    mutable std::map<std::string, std::function<std::unique_ptr<PlayerStrategy>()>> creators_;
};

} // namespace core

#endif // CORE_STRATEGY_FACTORY_H