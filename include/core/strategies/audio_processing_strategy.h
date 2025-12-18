#ifndef CORE_STRATEGIES_AUDIO_PROCESSING_STRATEGY_H
#define CORE_STRATEGIES_AUDIO_PROCESSING_STRATEGY_H

#include <string>

// 前向声明音频缓冲区类
class AudioBuffer;

namespace core {

// 音频处理策略基类
class AudioProcessingStrategy {
public:
    virtual ~AudioProcessingStrategy() = default;
    
    // 处理音频数据
    virtual void processAudio(float* buffer, size_t frames) = 0;
    
    // 获取策略名称
    virtual std::string getName() const = 0;
    
    // 检查是否为实时处理
    virtual bool isRealtime() const = 0;
};

} // namespace core

#endif // CORE_STRATEGIES_AUDIO_PROCESSING_STRATEGY_H