#include "core/error.h"

namespace core {

// CoreError 实现
CoreError::CoreError(const std::string& message) : message_(message) {}

const char* CoreError::what() const noexcept {
    return message_.c_str();
}

// AudioError 实现
AudioError::AudioError(const std::string& message) : CoreError(message) {}

// FileError 实现
FileError::FileError(const std::string& message) : CoreError(message) {}

// NetworkError 实现
NetworkError::NetworkError(const std::string& message) : CoreError(message) {}

// ConfigError 实现
ConfigError::ConfigError(const std::string& message) : CoreError(message) {}

} // namespace core