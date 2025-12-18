#include "core/unified_music_player.h"
#include "core/strategy_factory.h"
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
    // 使用策略工厂创建指定名称的播放策略
    auto factory = StrategyFactory::instance();
    auto new_strategy = factory->create_strategy(strategy_name);

    if (!new_strategy) {
        std::cerr << "Failed to create strategy: " << strategy_name << std::endl;
        return false;
    }

    // 如果当前已有策略，先停止并清理
    if (current_strategy_) {
        current_strategy_->stop();
    }

    // 设置新的策略
    current_strategy_ = std::move(new_strategy);
    active_strategy_name_ = strategy_name;

    // 初始化新策略
    return current_strategy_->initialize();
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