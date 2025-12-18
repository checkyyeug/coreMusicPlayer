#include "core/audio_playlist.h"
#include <iostream>

namespace core {

AudioPlaylist::AudioPlaylist() 
    : initialized_(false), current_index_(0) {
    // 初始化音频播放列表
}

AudioPlaylist::~AudioPlaylist() {
    // 析构函数
    shutdown();
}

bool AudioPlaylist::initialize() {
    std::cout << "Initializing audio playlist" << std::endl;
    
    // 在实际实现中，这里会初始化播放列表
    
    initialized_ = true;
    return true;
}

void AudioPlaylist::shutdown() {
    if (initialized_) {
        std::cout << "Shutting down audio playlist" << std::endl;
        
        // 在实际实现中，这里会关闭播放列表
        
        files_.clear();
        initialized_ = false;
    }
}

bool AudioPlaylist::addFile(const std::string& filename) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Adding audio file to playlist: " << filename << std::endl;
    
    // 在实际实现中，这里会添加音频文件到播放列表
    
    files_.push_back(filename);
    return true;
}

bool AudioPlaylist::removeFile(const std::string& filename) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Removing audio file from playlist: " << filename << std::endl;
    
    // 在实际实现中，这里会从播放列表中移除音频文件
    
    for (auto it = files_.begin(); it != files_.end(); ++it) {
        if (*it == filename) {
            files_.erase(it);
            return true;
        }
    }
    
    return false;
}

void AudioPlaylist::clear() {
    if (initialized_) {
        std::cout << "Clearing audio playlist" << std::endl;
        
        // 在实际实现中，这里会清空播放列表
        
        files_.clear();
        current_index_ = 0;
    }
}

size_t AudioPlaylist::getFileCount() const {
    return files_.size();
}

std::string AudioPlaylist::getFileName(size_t index) const {
    if (index >= files_.size()) {
        return "";
    }
    
    return files_[index];
}

size_t AudioPlaylist::getCurrentIndex() const {
    return current_index_;
}

bool AudioPlaylist::setCurrentIndex(size_t index) {
    if (!initialized_ || index >= files_.size()) {
        return false;
    }
    
    std::cout << "Setting playlist current index to: " << index << std::endl;
    
    // 在实际实现中，这里会设置当前播放的文件索引
    
    current_index_ = index;
    return true;
}

bool AudioPlaylist::next() {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Playing next audio file in playlist" << std::endl;
    
    // 在实际实现中，这里会播放下一个文件
    
    if (current_index_ < files_.size() - 1) {
        current_index_++;
        return true;
    }
    
    return false;
}

bool AudioPlaylist::previous() {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Playing previous audio file in playlist" << std::endl;
    
    // 在实际实现中，这里会播放上一个文件
    
    if (current_index_ > 0) {
        current_index_--;
        return true;
    }
    
    return false;
}

void AudioPlaylist::getStatus(std::vector<std::string>& files, 
                              size_t& current_index, 
                              size_t& file_count) const {
    // 在实际实现中，这里会获取播放列表状态
    
    files = files_;
    current_index = current_index_;
    file_count = files_.size();
}

} // namespace core