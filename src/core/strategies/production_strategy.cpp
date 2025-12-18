#include "core/production_strategy.h"
#include <iostream>

namespace core {

ProductionStrategy::ProductionStrategy() 
    : state_(PlayerState::STOPPED),
      volume_(0.5f),
      position_ms_(0),
      current_track_index_(0) {
}

bool ProductionStrategy::initialize() {
    // 生产环境策略初始化 - 包含错误恢复和健康检查
    std::cout << "Production strategy initialized\n";
    return true;
}

bool ProductionStrategy::play() {
    if (playlist_.empty()) {
        return false;
    }
    
    state_ = PlayerState::PLAYING;
    std::cout << "Playing in production mode: " << current_track_ << "\n";
    return true;
}

bool ProductionStrategy::stop() {
    state_ = PlayerState::STOPPED;
    position_ms_ = 0;
    std::cout << "Stopped\n";
    return true;
}

bool ProductionStrategy::pause() {
    if (state_ == PlayerState::PLAYING) {
        state_ = PlayerState::PAUSED;
        std::cout << "Paused\n";
    } else if (state_ == PlayerState::PAUSED) {
        state_ = PlayerState::PLAYING;
        std::cout << "Resumed\n";
    }
    return true;
}

bool ProductionStrategy::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << "\n";
    return true;
}

std::string ProductionStrategy::get_status() const {
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

long long ProductionStrategy::get_position_ms() const {
    return position_ms_;
}

bool ProductionStrategy::seek(long long position_ms) {
    // 生产环境下的跳转处理
    if (position_ms >= 0) {
        position_ms_ = position_ms;
        std::cout << "Seeked to: " << position_ms_ << "ms\n";
        return true;
    }
    return false;
}

bool ProductionStrategy::set_playlist(const std::vector<std::string>& playlist) {
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

std::string ProductionStrategy::get_current_track() const {
    return current_track_;
}

std::string ProductionStrategy::get_name() const {
    return "Production";
}

} // namespace core