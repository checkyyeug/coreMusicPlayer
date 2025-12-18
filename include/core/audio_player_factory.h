#ifndef CORE_AUDIO_PLAYER_FACTORY_H
#define CORE_AUDIO_PLAYER_FACTORY_H

#include "core/audio_player.h"
#include <memory>

namespace core {

// 音频播放器工厂类
class AudioPlayerFactory {
public:
    // 创建音频播放器实例
    static std::unique_ptr<AudioPlayer> createAudioPlayer();
    
    // 创建特定类型的音频播放器
    static std::unique_ptr<AudioPlayer> createAudioPlayer(const std::string& type);
};

} // namespace core

#endif // CORE_AUDIO_PLAYER_FACTORY_H