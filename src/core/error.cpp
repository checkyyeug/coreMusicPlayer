#include "core/error.h"

namespace core {

Error::Error(ErrorCode code, const std::string& message)
    : code_(code), message_(message) {}

ErrorCode Error::code() const {
    return code_;
}

const std::string& Error::message() const {
    return message_;
}

// 创建成功错误对象的便捷函数
Error success() {
    return Error(ErrorCode::SUCCESS, "Success");
}

} // namespace core