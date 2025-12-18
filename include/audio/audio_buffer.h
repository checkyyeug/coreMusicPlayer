#ifndef AUDIO_AUDIO_BUFFER_H
#define AUDIO_AUDIO_BUFFER_H

#include <vector>
#include <cstdint>

namespace audio {

class AudioBuffer {
public:
    // 构造函数
    explicit AudioBuffer(size_t size = 0) : buffer_(size), size_(size) {}

    // 拷贝构造函数
    AudioBuffer(const AudioBuffer& other) = default;

    // 移动构造函数
    AudioBuffer(AudioBuffer&& other) noexcept = default;

    // 赋值操作符（拷贝）
    AudioBuffer& operator=(const AudioBuffer& other) = default;

    // 赋值操作符（移动）
    AudioBuffer& operator=(AudioBuffer&& other) noexcept = default;

    // 析构函数
    ~AudioBuffer() = default;

    // 获取缓冲区大小
    size_t size() const { return size_; }

    // 重置缓冲区大小
    void resize(size_t new_size) {
        buffer_.resize(new_size);
        size_ = new_size;
    }

    // 清空缓冲区
    void clear() {
        buffer_.clear();
        size_ = 0;
    }

    // 获取数据指针（只读）
    const float* data() const { return buffer_.data(); }

    // 获取数据指针（可写）
    float* data() { return buffer_.data(); }

    // 设置所有元素为0
    void zero() {
        for (auto& sample : buffer_) {
            sample = 0.0f;
        }
    }

private:
    std::vector<float> buffer_;
    size_t size_;
};

} // namespace audio

#endif // AUDIO_AUDIO_BUFFER_H