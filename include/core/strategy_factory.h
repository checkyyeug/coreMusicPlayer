#ifndef CORE_STRATEGY_FACTORY_H
#define CORE_STRATEGY_FACTORY_H

#include <memory>
#include <string>

// 前向声明策略类
class AudioProcessingStrategy;

namespace core {

// 策略工厂类
class StrategyFactory {
public:
    static StrategyFactory& instance();
    
    // 创建指定类型的音频处理策略
    std::unique_ptr<AudioProcessingStrategy> createStrategy(const std::string& strategyType);
    
    // 获取所有支持的策略类型
    std::vector<std::string> getSupportedStrategies() const;

private:
    StrategyFactory() = default;
};

} // namespace core

#endif // CORE_STRATEGY_FACTORY_H