#ifndef PLATFORM_THREAD_MANAGER_H
#define PLATFORM_THREAD_MANAGER_H

#include <thread>
#include <functional>
#include <memory>

namespace platform {

class ThreadManager {
public:
    // 创建并启动线程
    static std::thread create_thread(std::function<void()> func);

    // 设置线程优先级（如果支持）
    static bool set_thread_priority(std::thread& thread, int priority);

    // 设置线程亲和性（如果支持）
    static bool set_thread_affinity(std::thread& thread, unsigned int cpu_id);

    // 获取硬件线程数
    static size_t hardware_concurrency();
};

} // namespace platform

#endif // PLATFORM_THREAD_MANAGER_H