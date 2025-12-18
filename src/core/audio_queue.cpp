#include "core/audio_queue.h"
#include <iostream>

namespace core {

AudioQueue::AudioQueue() : max_size_(100) {
    // 初始化音频队列
}

AudioQueue::~AudioQueue() = default;

bool AudioQueue::push(const AudioBuffer& buffer) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // 等待队列非满
    not_full_.wait(lock, [this] { return queue_.size() < max_size_; });
    
    queue_.push(buffer);
    
    // 通知等待出队的线程
    not_empty_.notify_one();
    
    return true;
}

bool AudioQueue::pop(AudioBuffer& buffer) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // 等待队列非空
    not_empty_.wait(lock, [this] { return !queue_.empty(); });
    
    buffer = queue_.front();
    queue_.pop();
    
    // 通知等待入队的线程
    not_full_.notify_one();
    
    return true;
}

size_t AudioQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

bool AudioQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

void AudioQueue::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    while (!queue_.empty()) {
        queue_.pop();
    }
    
    // 通知所有等待的线程
    not_empty_.notify_all();
    not_full_.notify_all();
}

void AudioQueue::waitNotEmpty() {
    std::unique_lock<std::mutex> lock(mutex_);
    not_empty_.wait(lock, [this] { return !queue_.empty(); });
}

void AudioQueue::waitNotFull() {
    std::unique_lock<std::mutex> lock(mutex_);
    not_full_.wait(lock, [this] { return queue_.size() < max_size_; });
}

} // namespace core