#include "core/audio_player_factory.h"
#include "core/audio_player.h"
#include <iostream>

namespace core {

std::unique_ptr<AudioPlayer> AudioPlayerFactory::createAudioPlayer() {
    std::cout << "Creating default audio player" << std::endl;
    
    // 在实际实现中，这里会根据平台或配置创建特定类型的播放器
    
    return std::make_unique<AudioPlayer>();
}

std::unique_ptr<AudioPlayer> AudioPlayerFactory::createAudioPlayer(const std::string& type) {
    std::cout << "Creating audio player of type: " << type << std::endl;
    
    // 在实际实现中，这里会根据类型创建特定类型的播放器
    
    return std::make_unique<AudioPlayer>();
}

} // namespace core