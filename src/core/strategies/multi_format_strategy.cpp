#include "core/strategies/multi_format_strategy.h"
#include <iostream>

namespace core {

MultiFormatStrategy::MultiFormatStrategy()
    : state_(PlayerState::STOPPED),
      volume_(0.5f),
      position_ms_(0),
      current_track_index_(0),
      auto_detect_format_(true) {
}

bool MultiFormatStrategy::initialize() {
    // 多格式策略初始化 - 自动检测和加载解码器
    std::cout << "MultiFormat strategy initialized\n";
    return true;
}

bool MultiFormatStrategy::play() {
    if (playlist_.empty()) {
        return false;
    }

    state_ = PlayerState::PLAYING;
    std::cout << "Playing in multi-format mode: " << current_track_ << "\n";
    return true;
}

bool MultiFormatStrategy::stop() {
    state_ = PlayerState::STOPPED;
    position_ms_ = 0;
    std::cout << "Stopped\n";
    return true;
}

bool MultiFormatStrategy::pause() {
    if (state_ == PlayerState::PLAYING) {
        state_ = PlayerState::PAUSED;
        std::cout << "Paused\n";
    } else if (state_ == PlayerState::PAUSED) {
        state_ = PlayerState::PLAYING;
        std::cout << "Resumed\n";
    }
    return true;
}

bool MultiFormatStrategy::set_volume(float volume) {
    // 限制音量范围在0.0到1.0之间
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    volume_ = volume;
    std::cout << "Volume set to: " << volume_ << "\n";
    return true;
}

std::string MultiFormatStrategy::get_status() const {
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

long long MultiFormatStrategy::get_position_ms() const {
    return position_ms_;
}

bool MultiFormatStrategy::seek(long long position_ms) {
    // 多格式模式下的跳转处理
    if (position_ms >= 0) {
        position_ms_ = position_ms;
        std::cout << "Seeked to: " << position_ms_ << "ms\n";
        return true;
    }
    return false;
}

bool MultiFormatStrategy::set_playlist(const std::vector<std::string>& playlist) {
    playlist_ = playlist;
    if (!playlist_.empty()) {
        current_track_index_ = 0;
        current_track_ = playlist_[0];
        
        // 自动检测格式
        if (auto_detect_format_) {
            size_t last_dot = current_track_.find_last_of('.');
            if (last_dot != std::string::npos) {
                current_format_ = current_track_.substr(last_dot + 1);
            }
        }
    } else {
        current_track_ = "";
        current_format_ = "";
    }

    std::cout << "Playlist set with " << playlist_.size() << " tracks\n";
    return true;
}

std::string MultiFormatStrategy::get_current_track() const {
    return current_track_;
}

std::string MultiFormatStrategy::get_name() const {
    return "MultiFormat";
}

} // namespace core