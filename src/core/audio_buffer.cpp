#include "core/audio_buffer.h"
#include <algorithm>

namespace core {

AudioBuffer::AudioBuffer() = default;

AudioBuffer::AudioBuffer(size_t size) : buffer_(size, 0.0f) {}

AudioBuffer::~AudioBuffer() = default;

AudioBuffer::AudioBuffer(const AudioBuffer& other) : buffer_(other.buffer_) {}

AudioBuffer& AudioBuffer::operator=(const AudioBuffer& other) {
    if (this != &other) {
        buffer_ = other.buffer_;
    }
    return *this;
}

AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept : buffer_(std::move(other.buffer_)) {}

AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept {
    if (this != &other) {
        buffer_ = std::move(other.buffer_);
    }
    return *this;
}

size_t AudioBuffer::size() const {
    return buffer_.size();
}

void AudioBuffer::resize(size_t size) {
    buffer_.resize(size, 0.0f);
}

void AudioBuffer::clear() {
    buffer_.clear();
}

float& AudioBuffer::operator[](size_t index) {
    return buffer_[index];
}

const float& AudioBuffer::operator[](size_t index) const {
    return buffer_[index];
}

float* AudioBuffer::data() {
    return buffer_.data();
}

const float* AudioBuffer::data() const {
    return buffer_.data();
}

void AudioBuffer::copyFrom(const AudioBuffer& other) {
    if (this != &other) {
        buffer_ = other.buffer_;
    }
}

void AudioBuffer::append(const AudioBuffer& other) {
    buffer_.insert(buffer_.end(), other.buffer_.begin(), other.buffer_.end());
}

AudioBuffer AudioBuffer::subBuffer(size_t start, size_t length) const {
    if (start >= buffer_.size()) {
        return AudioBuffer(0);
    }
    
    size_t end = std::min(start + length, buffer_.size());
    return AudioBuffer(buffer_.data() + start, buffer_.data() + end);
}

} // namespace core