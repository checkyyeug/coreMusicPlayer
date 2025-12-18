#include "utils/performance_counter.h"
#include <chrono>
#include <iostream>

namespace utils {

std::shared_ptr<PerformanceCounter> PerformanceCounter::instance() {
    static std::shared_ptr<PerformanceCounter> counter =
        std::make_shared<PerformanceCounter>();
    return counter;
}

void PerformanceCounter::start_timer(const std::string& name) {
    auto now = std::chrono::high_resolution_clock::now();
    timers_[name].start_time = now;
}

void PerformanceCounter::stop_timer(const std::string& name) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        now - timers_[name].start_time);

    timers_[name].total_duration_ms += duration.count() / 1000.0;
    timers_[name].count++;
}

double PerformanceCounter::get_elapsed_ms(const std::string& name) const {
    auto it = timers_.find(name);
    if (it != timers_.end() && it->second.count > 0) {
        return it->second.total_duration_ms / it->second.count;
    }
    return 0.0;
}

void PerformanceCounter::reset() {
    timers_.clear();
}

void PerformanceCounter::print_report() const {
    std::cout << "Performance Report:" << std::endl;
    for (const auto& pair : timers_) {
        const auto& name = pair.first;
        const auto& data = pair.second;

        if (data.count > 0) {
            double avg_time = data.total_duration_ms / data.count;
            std::cout << "  " << name << ": " << avg_time << " ms ("
                      << data.count << " calls)" << std::endl;
        }
    }
}

} // namespace utils