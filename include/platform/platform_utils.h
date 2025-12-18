#ifndef PLATFORM_PLATFORM_UTILS_H
#define PLATFORM_PLATFORM_UTILS_H

#include <string>

namespace platform {

// 平台检测函数
std::string get_platform_name();
bool is_windows();
bool is_linux();
bool is_macos();

// 架构检测函数
std::string get_architecture();
bool is_64bit();

// 编译器特性检测
bool has_sse_support();
bool has_avx_support();

} // namespace platform

#endif // PLATFORM_PLATFORM_UTILS_H