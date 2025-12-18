#ifndef CORE_AUDIO_PLAYER_MANAGER_H
#define CORE_AUDIO_PLAYER_MANAGER_H

#include "core/audio_player.h"
#include <memory>
#include <vector>

namespace core {

// 音频播放器管理器类
class AudioPlayerManager {
public:
    // 构造函数
    AudioPlayerManager();
    
    // 析构函数
    ~AudioPlayerManager();
    
    // 初始化管理器
    bool initialize();
    
    // 关闭管理器
    void shutdown();
    
    // 创建音频播放器实例
    std::unique_ptr<AudioPlayer> createPlayer();
    
    // 获取当前播放器数量
    size_t getPlayerCount() const;
    
    // 获取所有播放器
    std::vector<std::unique_ptr<AudioPlayer>>& getPlayers();
    
private:
    // 私有成员变量
    bool initialized_;
    std::vector<std::unique_ptr<AudioPlayer>> players_;
};

} // namespace core

#endif // CORE_AUDIO_PLAYER_MANAGER_H