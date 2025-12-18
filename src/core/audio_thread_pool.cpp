#include "core/audio_thread_pool.h"
#include <iostream>

namespace core {

AudioThreadPool::AudioThreadPool(size_t num_threads) 
    : stop_(false) {
    // 初始化线程池
    
    for (size_t i = 0; i < num_threads; ++i) {
        threads_.emplace_back([this] {
            for (;;) {
                std::function<void()> task;
                
                {
                    std::unique_lock<std::mutex> lock(this->mutex_);
                    this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });
                    
                    if (this->stop_ && this->tasks_.empty()) {
                        return;
                    }
                    
                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }
                
                task();
            }
        });
    }
}

AudioThreadPool::~AudioThreadPool() {
    stop();
}

template<typename F>
auto AudioThreadPool::submit(F&& f) -> std::future<decltype(f())> {
    using return_type = decltype(f());
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::forward<F>(f)
    );
    
    std::future<return_type> res = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (stop_) {
            throw std::runtime_error("submit on stopped ThreadPool");
        }
        
        tasks_.emplace([task]() { (*task)(); });
    }
    
    condition_.notify_one();
    
    return res;
}

size_t AudioThreadPool::getThreadCount() const {
    return threads_.size();
}

void AudioThreadPool::start() {
    // 启动线程池
    std::cout << "Starting audio thread pool with " << threads_.size() << " threads" << std::endl;
}

void AudioThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    
    condition_.notify_all();
    
    for (std::thread& worker : threads_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

} // namespace core