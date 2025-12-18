#ifndef CORE_ERROR_H
#define CORE_ERROR_H

#include <string>

namespace core {

enum class ErrorCode {
    SUCCESS = 0,
    INVALID_ARGUMENT = 1,
    FILE_NOT_FOUND = 2,
    INTERNAL_ERROR = 3
};

class Error {
public:
    // 构造函数
    explicit Error(ErrorCode code, const std::string& message);

    // 获取错误代码
    ErrorCode code() const;

    // 获取错误消息
    const std::string& message() const;

private:
    ErrorCode code_;
    std::string message_;
};

// 创建成功错误对象的便捷函数
Error success();

} // namespace core

#endif // CORE_ERROR_H