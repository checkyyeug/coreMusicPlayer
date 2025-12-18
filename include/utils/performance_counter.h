#ifndef UTILS_PERFORMANCE_COUNTER_H
#define UTILS_PERFORMANCE_COUNTER_H

#include <chrono>
#include <string>
#include <map>
#include <memory>

namespace utils {

class PerformanceCounter {
public:
    // 获取单例实例
    static std::shared_ptr<PerformanceCounter> instance();

    // 开始计时
    void start_timer(const std::string& name);

    // 停止计时并记录结果
    void stop_timer(const std::string& name);

    // 获取特定计时器的耗时（毫秒）
    double get_elapsed_ms(const std::string& name) const;

    // 重置所有计时器
    void reset();

    // 输出性能报告
    void print_report() const;

private:
    PerformanceCounter() = default;
    ~PerformanceCounter() = default;

    struct TimerData {
        std::chrono::high_resolution_clock::time_point start_time;
        double total_duration_ms;
        size_t count;

        TimerData() : total_duration_ms(0.0), count(0) {}
    };

    std::map<std::string, TimerData> timers_;
};

} // namespace utils

#endif // UTILS_PERFORMANCE_COUNTER_H