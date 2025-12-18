#ifndef CORE_PRODUCTION_STRATEGY_H
#define CORE_PRODUCTION_STRATEGY_H

#include "core/player_strategy.h"
#include <string>

namespace core {

// Production播放策略 - 生产环境使用，包含错误恢复和健康检查
class ProductionStrategy : public PlayerStrategy {
public:
    // 构造函数
    ProductionStrategy();
    
    // 初始化播放器
    bool initialize() override;
    
    // 启动播放
    bool play() override;
    
    // 停止播放
    bool stop() override;
    
    // 暂停/恢复播放
    bool pause() override;
    
    // 设置音量（0.0 - 1.0）
    bool set_volume(float volume) override;
    
    // 获取当前状态
    std::string get_status() const override;
    
    // 获取播放位置（毫秒）
    long long get_position_ms() const override;
    
    // 跳转到指定位置
    bool seek(long long position_ms) override;
    
    // 设置播放列表
    bool set_playlist(const std::vector<std::string>& playlist) override;
    
    // 获取当前播放项
    std::string get_current_track() const override;
    
    // 获取策略名称
    std::string get_name() const override;
    
private:
    // 状态变量
    enum class PlayerState {
        STOPPED,
        PLAYING,
        PAUSED
    };
    
    PlayerState state_;
    float volume_;
    long long position_ms_;
    std::vector<std::string> playlist_;
    size_t current_track_index_;
    std::string current_track_;
};

} // namespace core

#endif // CORE_PRODUCTION_STRATEGY_H