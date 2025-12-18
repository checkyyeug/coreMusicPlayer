#include "platform/thread_manager.h"
#include <thread>

namespace platform {

// 创建并启动线程
std::thread ThreadManager::create_thread(std::function<void()> func) {
    return std::thread(func);
}

// 设置线程优先级（如果支持）
bool ThreadManager::set_thread_priority(std::thread& thread, int priority) {
    // Windows平台不直接支持设置线程优先级，返回false表示不支持
    return false;
}

// 设置线程亲和性（如果支持）
bool ThreadManager::set_thread_affinity(std::thread& thread, unsigned int cpu_id) {
    // Windows平台不直接支持设置线程亲和性，返回false表示不支持
    return false;
}

// 获取硬件线程数
size_t ThreadManager::hardware_concurrency() {
    return std::thread::hardware_concurrency();
}

} // namespace platform