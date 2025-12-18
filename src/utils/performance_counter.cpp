#include "utils/performance_counter.h"
#include <iostream>

namespace utils {

PerformanceCounter::PerformanceCounter() : start_time_(std::chrono::high_resolution_clock::now()) {}

void PerformanceCounter::start() {
    start_time_ = std::chrono::high_resolution_clock::now();
}

double PerformanceCounter::elapsed_ms() const {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
    return duration.count() / 1000.0;
}

void PerformanceCounter::print_elapsed(const std::string& label) const {
    std::cout << label << ": " << elapsed_ms() << " ms" << std::endl;
}

} // namespace utils