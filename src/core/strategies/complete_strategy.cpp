#include "core/complete_strategy.h"
#include <iostream>

namespace core {

CompleteStrategy::CompleteStrategy() 
    : state_(PlayerState::STOPPED),
      volume_(0.5f),
      position_ms_(0),
      current_track_index_(0) {
}

bool CompleteStrategy::initialize() {
    // 完整策略初始化 - 实际项目中需要加载所有解码器和插件
    std::cout << "Complete strategy initialized\n";
    return true;
}

bool CompleteStrategy::play() {
    if (playlist_.empty()) {
        return false;
    }
    
    state_ = PlayerState::PLAYING;
    std::cout << "Playing: " << current_track_ << "\n";
    return true;
}

bool CompleteStrategy::stop() {
    state_ = PlayerState::STOPPED;
    position_ms_ = 0;
    std::cout << "Stopped\n";
    return true;
}

bool CompleteStrategy::pause() {
    if (state_ == PlayerState::PLAYING) {
        state_ = PlayerState::PAUSED;
        std::cout << "Paused\n";
    } else if (state_ == PlayerState::PAUSED) {
        state_ = PlayerState::PLAYING;
        std::cout << "Resumed\n";
    }
    return true;
}

bool CompleteStrategy::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << "\n";
    return true;
}

std::string CompleteStrategy::get_status() const {
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

long long CompleteStrategy::get_position_ms() const {
    return position_ms_;
}

bool CompleteStrategy::seek(long long position_ms) {
    // 处理跳转逻辑
    if (position_ms >= 0) {
        position_ms_ = position_ms;
        std::cout << "Seeked to: " << position_ms_ << "ms\n";
        return true;
    }
    return false;
}

bool CompleteStrategy::set_playlist(const std::vector<std::string>& playlist) {
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

std::string CompleteStrategy::get_current_track() const {
    return current_track_;
}

std::string CompleteStrategy::get_name() const {
    return "Complete";
}

} // namespace core