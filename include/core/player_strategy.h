#ifndef CORE_PLAYER_STRATEGY_H
#define CORE_PLAYER_STRATEGY_H

#include <string>
#include <memory>

namespace core {

// 播放器策略接口类
class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;
    
    // 初始化播放器
    virtual bool initialize() = 0;
    
    // 启动播放
    virtual bool play() = 0;
    
    // 停止播放
    virtual bool stop() = 0;
    
    // 暂停/恢复播放
    virtual bool pause() = 0;
    
    // 设置音量
    virtual bool set_volume(float volume) = 0;
    
    // 获取当前状态
    virtual std::string get_status() const = 0;
    
    // 获取播放位置（毫秒）
    virtual long long get_position_ms() const = 0;
    
    // 跳转到指定位置
    virtual bool seek(long long position_ms) = 0;
    
    // 设置播放列表
    virtual bool set_playlist(const std::vector<std::string>& playlist) = 0;
    
    // 获取当前播放项
    virtual std::string get_current_track() const = 0;
    
    // 获取策略名称
    virtual std::string get_name() const = 0;
};

} // namespace core

#endif // CORE_PLAYER_STRATEGY_H