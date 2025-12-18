#ifndef CORE_AUDIO_THREAD_POOL_H
#define CORE_AUDIO_THREAD_POOL_H

#include <thread>
#include <queue>
#include <memory>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace core {

// 音频线程池类
class AudioThreadPool {
public:
    // 构造函数
    explicit AudioThreadPool(size_t num_threads);
    
    // 析构函数
    ~AudioThreadPool();
    
    // 提交任务到线程池
    template<typename F>
    auto submit(F&& f) -> std::future<decltype(f())>;
    
    // 获取线程数量
    size_t getThreadCount() const;
    
    // 启动线程池
    void start();
    
    // 停止线程池
    void stop();
    
private:
    // 私有成员变量
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_;
};

} // namespace core

#endif // CORE_AUDIO_THREAD_POOL_H