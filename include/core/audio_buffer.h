#ifndef CORE_AUDIO_BUFFER_H
#define CORE_AUDIO_BUFFER_H

#include <vector>
#include <memory>

namespace core {

// 音频缓冲区类
class AudioBuffer {
public:
    // 构造函数
    AudioBuffer();
    
    // 带大小的构造函数
    explicit AudioBuffer(size_t size);
    
    // 析构函数
    ~AudioBuffer();
    
    // 拷贝构造函数
    AudioBuffer(const AudioBuffer& other);
    
    // 赋值操作符
    AudioBuffer& operator=(const AudioBuffer& other);
    
    // 移动构造函数
    AudioBuffer(AudioBuffer&& other) noexcept;
    
    // 移动赋值操作符
    AudioBuffer& operator=(AudioBuffer&& other) noexcept;
    
    // 获取缓冲区大小
    size_t size() const;
    
    // 设置缓冲区大小
    void resize(size_t size);
    
    // 清空缓冲区
    void clear();
    
    // 获取指定索引的样本值
    float& operator[](size_t index);
    
    // 获取指定索引的样本值（常量）
    const float& operator[](size_t index) const;
    
    // 获取数据指针
    float* data();
    
    // 获取数据指针（常量）
    const float* data() const;
    
    // 从其他缓冲区复制数据
    void copyFrom(const AudioBuffer& other);
    
    // 将数据添加到缓冲区末尾
    void append(const AudioBuffer& other);
    
    // 获取缓冲区的子集
    AudioBuffer subBuffer(size_t start, size_t length) const;
    
private:
    std::vector<float> buffer_;
};

} // namespace core

#endif // CORE_AUDIO_BUFFER_H