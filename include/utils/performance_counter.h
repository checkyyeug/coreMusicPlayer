#ifndef UTILS_PERFORMANCE_COUNTER_H
#define UTILS_PERFORMANCE_COUNTER_H

#include <chrono>
#include <string>

namespace utils {

class PerformanceCounter {
public:
    // 构造函数
    PerformanceCounter();
    
    // 开始计时
    void start();
    
    // 获取经过的时间（毫秒）
    double elapsed_ms() const;
    
    // 打印经过的时间
    void print_elapsed(const std::string& label) const;

private:
    std::chrono::high_resolution_clock::time_point start_time_;
};

} // namespace utils

#endif // UTILS_PERFORMANCE_COUNTER_H