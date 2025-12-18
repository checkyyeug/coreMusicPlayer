#ifndef CORE_AUDIO_SCHEDULER_H
#define CORE_AUDIO_SCHEDULER_H

#include <memory>
#include <chrono>
#include <functional>

namespace core {

// 音频调度器类
class AudioScheduler {
public:
    // 构造函数
    AudioScheduler();
    
    // 析构函数
    ~AudioScheduler();
    
    // 启动调度器
    void start();
    
    // 停止调度器
    void stop();
    
    // 添加定时任务
    bool addTask(std::function<void()> task, 
                 std::chrono::milliseconds interval);
    
    // 移除定时任务
    bool removeTask(void* task_id);
    
    // 获取调度器状态
    bool isRunning() const;
    
private:
    // 私有成员变量
    bool running_;
};

} // namespace core

#endif // CORE_AUDIO_SCHEDULER_H