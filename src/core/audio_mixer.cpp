#include "core/audio_mixer.h"
#include <iostream>
#include <algorithm>

namespace core {

AudioMixer::AudioMixer() : initialized_(false) {
    // 初始化音频混音器
}

AudioMixer::~AudioMixer() {
    // 析构函数
    shutdown();
}

bool AudioMixer::initialize() {
    std::cout << "Initializing audio mixer" << std::endl;
    
    // 在实际实现中，这里会初始化混音器
    
    initialized_ = true;
    return true;
}

void AudioMixer::shutdown() {
    if (initialized_) {
        std::cout << "Shutting down audio mixer" << std::endl;
        
        // 在实际实现中，这里会关闭混音器
        
        clear();
        initialized_ = false;
    }
}

bool AudioMixer::addTrack(const std::string& name, const AudioBuffer& buffer) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Adding audio track: " << name << std::endl;
    
    // 在实际实现中，这里会添加音频轨道
    
    tracks_.emplace_back(name, buffer);
    return true;
}

bool AudioMixer::removeTrack(const std::string& name) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Removing audio track: " << name << std::endl;
    
    // 在实际实现中，这里会移除音频轨道
    
    auto it = std::find_if(tracks_.begin(), tracks_.end(),
                          [&name](const Track& track) {
                              return track.name == name;
                          });
    
    if (it != tracks_.end()) {
        tracks_.erase(it);
        return true;
    }
    
    return false;
}

bool AudioMixer::setTrackVolume(const std::string& name, float volume) {
    if (!initialized_) {
        return false;
    }
    
    // 限制音量范围在0.0到1.0之间
    volume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;
    
    std::cout << "Setting track " << name << " volume to: " << volume << std::endl;
    
    // 在实际实现中，这里会设置轨道音量
    
    auto it = std::find_if(tracks_.begin(), tracks_.end(),
                          [&name](const Track& track) {
                              return track.name == name;
                          });
    
    if (it != tracks_.end()) {
        it->volume = volume;
        return true;
    }
    
    return false;
}

float AudioMixer::getTrackVolume(const std::string& name) const {
    auto it = std::find_if(tracks_.begin(), tracks_.end(),
                          [&name](const Track& track) {
                              return track.name == name;
                          });
    
    if (it != tracks_.end()) {
        return it->volume;
    }
    
    return 0.0f;
}

bool AudioMixer::setTrackMute(const std::string& name, bool mute) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Setting track " << name << " mute to: " << (mute ? "true" : "false") << std::endl;
    
    // 在实际实现中，这里会设置轨道静音状态
    
    auto it = std::find_if(tracks_.begin(), tracks_.end(),
                          [&name](const Track& track) {
                              return track.name == name;
                          });
    
    if (it != tracks_.end()) {
        it->mute = mute;
        return true;
    }
    
    return false;
}

bool AudioMixer::getTrackMute(const std::string& name) const {
    auto it = std::find_if(tracks_.begin(), tracks_.end(),
                          [&name](const Track& track) {
                              return track.name == name;
                          });
    
    if (it != tracks_.end()) {
        return it->mute;
    }
    
    return false;
}

bool AudioMixer::mix(AudioBuffer& output) {
    if (!initialized_) {
        return false;
    }
    
    std::cout << "Mixing audio tracks" << std::endl;
    
    // 在实际实现中，这里会混音所有轨道
    
    if (tracks_.empty()) {
        output.clear();
        return true;
    }
    
    // 获取最大缓冲区大小
    size_t max_size = 0;
    for (const auto& track : tracks_) {
        max_size = std::max(max_size, track.buffer.size());
    }
    
    // 初始化输出缓冲区
    output.resize(max_size);
    
    // 混音所有轨道
    for (size_t i = 0; i < max_size; ++i) {
        float sample = 0.0f;
        
        for (const auto& track : tracks_) {
            if (!track.mute && i < track.buffer.size()) {
                sample += track.buffer[i] * track.volume;
            }
        }
        
        output[i] = sample;
    }
    
    return true;
}

void AudioMixer::clear() {
    if (initialized_) {
        std::cout << "Clearing audio mixer tracks" << std::endl;
        
        // 在实际实现中，这里会清空所有轨道
        
        tracks_.clear();
    }
}

size_t AudioMixer::getTrackCount() const {
    return tracks_.size();
}

std::vector<std::string> AudioMixer::getTrackNames() const {
    std::vector<std::string> names;
    for (const auto& track : tracks_) {
        names.push_back(track.name);
    }
    return names;
}

} // namespace core