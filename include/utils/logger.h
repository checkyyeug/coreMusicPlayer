#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include <string>
#include <memory>
#include <sstream>
#include <fstream>

namespace utils {

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    OFF = 5
};

class Logger {
public:
    // 获取单例实例
    static Logger& instance();

    // 设置日志级别
    void set_log_level(LogLevel level);
    
    // 设置输出文件
    void set_output_file(const std::string& filename);

    // 日志记录函数
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

private:
    Logger();
    ~Logger() = default;

    // 日志级别
    LogLevel log_level_;
    
    // 输出文件
    std::string output_file_;
    
    // 文件流
    std::ofstream file_stream_;
};

// 便捷宏定义
#define LOG_TRACE(msg) utils::Logger::instance()->trace(msg)
#define LOG_DEBUG(msg) utils::Logger::instance()->debug(msg)
#define LOG_INFO(msg) utils::Logger::instance()->info(msg)
#define LOG_WARN(msg) utils::Logger::instance()->warn(msg)
#define LOG_ERROR(msg) utils::Logger::instance()->error(msg)

} // namespace utils

#endif // UTILS_LOGGER_H