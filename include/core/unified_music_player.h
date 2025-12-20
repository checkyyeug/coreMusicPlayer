#ifndef CORE_UNIFIED_MUSIC_PLAYER_H
#define CORE_UNIFIED_MUSIC_PLAYER_H

#include <memory>
#include <string>
#include <vector>
#include "core/player_strategy.h"

namespace core {

class UnifiedMusicPlayer {
public:
    // 获取单例实例
    static std::shared_ptr<UnifiedMusicPlayer> instance();
    
    // 初始化播放器
    bool initialize();
    
    // 设置播放策略
    bool set_strategy(const std::string& strategy_name);
    
    // 启动播放
    bool play();
    
    // 停止播放
    bool stop();
    
    // 暂停/恢复播放
    bool pause();
    
    // 设置音量（0.0 - 1.0）
    bool set_volume(float volume);
    
    // 获取当前状态
    std::string get_status() const;
    
    // 获取播放位置（毫秒）
    long long get_position_ms() const;
    
    // 跳转到指定位置
    bool seek(long long position_ms);
    
    // 设置播放列表
    bool set_playlist(const std::vector<std::string>& playlist);
    
    // 获取当前播放项
    std::string get_current_track() const;
    
    // 获取当前策略名称
    std::string get_active_strategy_name() const;
    
public:
    UnifiedMusicPlayer();
    
    // 当前使用的播放策略
    std::unique_ptr<PlayerStrategy> current_strategy_;
    
    // 策略名称
    std::string active_strategy_name_;
};

} // namespace core

#endif // CORE_UNIFIED_MUSIC_PLAYER_H