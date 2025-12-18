#include "utils/logger.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace utils {

// 获取单例实例的实现
std::shared_ptr<Logger> Logger::instance() {
    static std::shared_ptr<Logger> logger = std::make_shared<Logger>();
    return logger;
}

// 构造函数
Logger::Logger() : level_(LogLevel::INFO) {}

// 设置日志级别
void Logger::set_level(LogLevel level) {
    level_ = level;
}

// 获取时间戳的辅助函数实现
std::string Logger::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

// 日志记录函数实现
void Logger::trace(const std::string& message) {
    if (level_ <= LogLevel::TRACE) {
        std::cout << "[TRACE] " << get_timestamp() << " - " << message << std::endl;
    }
}

void Logger::debug(const std::string& message) {
    if (level_ <= LogLevel::DEBUG) {
        std::cout << "[DEBUG] " << get_timestamp() << " - " << message << std::endl;
    }
}

void Logger::info(const std::string& message) {
    if (level_ <= LogLevel::INFO) {
        std::cout << "[INFO] " << get_timestamp() << " - " << message << std::endl;
    }
}

void Logger::warn(const std::string& message) {
    if (level_ <= LogLevel::WARN) {
        std::cerr << "[WARN] " << get_timestamp() << " - " << message << std::endl;
    }
}

void Logger::error(const std::string& message) {
    if (level_ <= LogLevel::ERROR) {
        std::cerr << "[ERROR] " << get_timestamp() << " - " << message << std::endl;
    }
}

} // namespace utils