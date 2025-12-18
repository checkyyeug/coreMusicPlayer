#include "core/legacy_strategy.h"
#include <iostream>

namespace core {

LegacyStrategy::LegacyStrategy() 
    : state_(PlayerState::STOPPED),
      volume_(0.5f),
      position_ms_(0),
      current_track_index_(0) {
}

bool LegacyStrategy::initialize() {
    // 简化实现 - 实际项目中需要初始化音频设备等
    std::cout << "Legacy strategy initialized\n";
    return true;
}

bool LegacyStrategy::play() {
    if (playlist_.empty()) {
        return false;
    }
    
    state_ = PlayerState::PLAYING;
    std::cout << "Playing: " << current_track_ << "\n";
    return true;
}

bool LegacyStrategy::stop() {
    state_ = PlayerState::STOPPED;
    position_ms_ = 0;
    std::cout << "Stopped\n";
    return true;
}

bool LegacyStrategy::pause() {
    if (state_ == PlayerState::PLAYING) {
        state_ = PlayerState::PAUSED;
        std::cout << "Paused\n";
    } else if (state_ == PlayerState::PAUSED) {
        state_ = PlayerState::PLAYING;
        std::cout << "Resumed\n";
    }
    return true;
}

bool LegacyStrategy::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << "\n";
    return true;
}

std::string LegacyStrategy::get_status() const {
    switch (state_) {
        case PlayerState::STOPPED:
            return "Stopped";
        case PlayerState::PLAYING:
            return "Playing";
        case PlayerState::PAUSED:
            return "Paused";
        default:
            return "Unknown";
    }
}

long long LegacyStrategy::get_position_ms() const {
    return position_ms_;
}

bool LegacyStrategy::seek(long long position_ms) {
    // 简化实现 - 实际项目中需要处理跳转逻辑
    if (position_ms >= 0) {
        position_ms_ = position_ms;
        std::cout << "Seeked to: " << position_ms_ << "ms\n";
        return true;
    }
    return false;
}

bool LegacyStrategy::set_playlist(const std::vector<std::string>& playlist) {
    playlist_ = playlist;
    if (!playlist_.empty()) {
        current_track_index_ = 0;
        current_track_ = playlist_[0];
    } else {
        current_track_ = "";
    }
    
    std::cout << "Playlist set with " << playlist_.size() << " tracks\n";
    return true;
}

std::string LegacyStrategy::get_current_track() const {
    return current_track_;
}

std::string LegacyStrategy::get_name() const {
    return "Legacy";
}

} // namespace core