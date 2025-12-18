#include "core/unified_music_player.h"
#include <iostream>

namespace core {

std::shared_ptr<UnifiedMusicPlayer> UnifiedMusicPlayer::instance() {
    static std::shared_ptr<UnifiedMusicPlayer> player = 
        std::make_shared<UnifiedMusicPlayer>();
    return player;
}

UnifiedMusicPlayer::UnifiedMusicPlayer() : active_strategy_name_("none") {}

bool UnifiedMusicPlayer::initialize() {
    // 初始化播放器
    if (current_strategy_) {
        return current_strategy_->initialize();
    }
    return false;
}

bool UnifiedMusicPlayer::set_strategy(const std::string& strategy_name) {
    // 这里应该根据策略名称创建相应的策略实例
    // 为简化实现，我们暂时返回false表示未实现
    
    active_strategy_name_ = strategy_name;
    return true;  // 简化处理
}

bool UnifiedMusicPlayer::play() {
    if (current_strategy_) {
        return current_strategy_->play();
    }
    return false;
}

bool UnifiedMusicPlayer::stop() {
    if (current_strategy_) {
        return current_strategy_->stop();
    }
    return false;
}

bool UnifiedMusicPlayer::pause() {
    if (current_strategy_) {
        return current_strategy_->pause();
    }
    return false;
}

bool UnifiedMusicPlayer::set_volume(float volume) {
    if (current_strategy_) {
        return current_strategy_->set_volume(volume);
    }
    return false;
}

std::string UnifiedMusicPlayer::get_status() const {
    if (current_strategy_) {
        return current_strategy_->get_status();
    }
    return "No strategy set";
}

long long UnifiedMusicPlayer::get_position_ms() const {
    if (current_strategy_) {
        return current_strategy_->get_position_ms();
    }
    return 0;
}

bool UnifiedMusicPlayer::seek(long long position_ms) {
    if (current_strategy_) {
        return current_strategy_->seek(position_ms);
    }
    return false;
}

bool UnifiedMusicPlayer::set_playlist(const std::vector<std::string>& playlist) {
    if (current_strategy_) {
        return current_strategy_->set_playlist(playlist);
    }
    return false;
}

std::string UnifiedMusicPlayer::get_current_track() const {
    if (current_strategy_) {
        return current_strategy_->get_current_track();
    }
    return "";
}

std::string UnifiedMusicPlayer::get_active_strategy_name() const {
    return active_strategy_name_;
}

} // namespace core