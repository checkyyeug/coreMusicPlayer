#include "audio/audio_buffer.h"

namespace audio {

// 构造函数实现
AudioBuffer::AudioBuffer(size_t size) : buffer_(size), size_(size) {}

// 拷贝构造函数实现
AudioBuffer::AudioBuffer(const AudioBuffer& other) = default;

// 移动构造函数实现
AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept = default;

// 赋值操作符（拷贝）实现
AudioBuffer& AudioBuffer::operator=(const AudioBuffer& other) = default;

// 赋值操作符（移动）实现
AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept = default;

// 析构函数实现
AudioBuffer::~AudioBuffer() = default;

} // namespace audio