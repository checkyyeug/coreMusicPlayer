#include "utils/performance_counter.h"
#include <iostream>
#include <iomanip>

namespace utils {

// 获取单例实例
std::shared_ptr<PerformanceCounter> PerformanceCounter::instance() {
    static std::shared_ptr<PerformanceCounter> instance = std::make_shared<PerformanceCounter>();
    return instance;
}

// 开始计时
void PerformanceCounter::start_timer(const std::string& name) {
    auto now = std::chrono::high_resolution_clock::now();
    timers_[name].start_time = now;
}

// 停止计时并记录结果
void PerformanceCounter::stop_timer(const std::string& name) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - timers_[name].start_time);

    timers_[name].total_duration_ms += duration.count() / 1000.0;
    timers_[name].count++;
}

// 获取特定计时器的耗时（毫秒）
double PerformanceCounter::get_elapsed_ms(const std::string& name) const {
    auto it = timers_.find(name);
    if (it != timers_.end()) {
        return it->second.total_duration_ms;
    }
    return 0.0;
}

// 重置所有计时器
void PerformanceCounter::reset() {
    timers_.clear();
}

// 输出性能报告
void PerformanceCounter::print_report() const {
    std::cout << "Performance Report:" << std::endl;
    for (const auto& pair : timers_) {
        const std::string& name = pair.first;
        const TimerData& data = pair.second;

        if (data.count > 0) {
            double avg_ms = data.total_duration_ms / data.count;
            std::cout << "Timer '" << name << "': "
                      << std::fixed << std::setprecision(2)
                      << data.total_duration_ms << "ms total, "
                      << data.count << " calls, "
                      << avg_ms << "ms average" << std::endl;
        }
    }
}

} // namespace utils