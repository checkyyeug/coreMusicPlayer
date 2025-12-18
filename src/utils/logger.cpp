#include "utils/logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace utils {

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::set_log_level(LogLevel level) {
    log_level_ = level;
}

void Logger::set_output_file(const std::string& filename) {
    output_file_ = filename;
    if (!output_file_.empty()) {
        file_stream_.open(output_file_, std::ios::out | std::ios::app);
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < log_level_) {
        return;
    }

    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    // 格式化时间
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    // 构造日志消息
    std::string level_str;
    switch (level) {
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::WARN:
            level_str = "WARN";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
    }

    std::string log_message = "[" + oss.str() + "] [" + level_str + "] " + message;

    // 输出到控制台
    std::cout << log_message << std::endl;

    // 输出到文件（如果已设置）
    if (file_stream_.is_open()) {
        file_stream_ << log_message << std::endl;
        file_stream_.flush();
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) {
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

} // namespace utils