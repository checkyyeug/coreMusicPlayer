#ifndef CORE_LEGACY_STRATEGY_H
#define CORE_LEGACY_STRATEGY_H

#include "core/player_strategy.h"
#include <string>

namespace core {

// Legacy播放策略 - 最小化实现，仅支持WAV格式和基本功能
class LegacyStrategy : public PlayerStrategy {
public:
    // 构造函数
    LegacyStrategy();
    
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

#endif // CORE_LEGACY_STRATEGY_H