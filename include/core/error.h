#ifndef CORE_ERROR_H
#define CORE_ERROR_H

#include <string>
#include <stdexcept>

namespace core {

// 基础错误类
class CoreError : public std::exception {
public:
    explicit CoreError(const std::string& message);
    const char* what() const noexcept override;
    
private:
    std::string message_;
};

// 音频相关错误
class AudioError : public CoreError {
public:
    explicit AudioError(const std::string& message);
};

// 文件操作错误
class FileError : public CoreError {
public:
    explicit FileError(const std::string& message);
};

// 网络相关错误
class NetworkError : public CoreError {
public:
    explicit NetworkError(const std::string& message);
};

// 配置相关错误
class ConfigError : public CoreError {
public:
    explicit ConfigError(const std::string& message);
};

} // namespace core

#endif // CORE_ERROR_H