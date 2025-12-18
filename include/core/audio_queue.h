#ifndef CORE_AUDIO_QUEUE_H
#define CORE_AUDIO_QUEUE_H

#include "core/audio_buffer.h"
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace core {

// 音频队列类
class AudioQueue {
public:
    // 构造函数
    AudioQueue();
    
    // 析构函数
    ~AudioQueue();
    
    // 入队音频数据
    bool push(const AudioBuffer& buffer);
    
    // 出队音频数据
    bool pop(AudioBuffer& buffer);
    
    // 获取队列大小
    size_t size() const;
    
    // 检查队列是否为空
    bool empty() const;
    
    // 清空队列
    void clear();
    
    // 等待队列非空（阻塞）
    void waitNotEmpty();
    
    // 等待队列非满（阻塞）
    void waitNotFull();
    
private:
    // 私有成员变量
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    std::queue<AudioBuffer> queue_;
    size_t max_size_;
};

} // namespace core

#endif // CORE_AUDIO_QUEUE_H