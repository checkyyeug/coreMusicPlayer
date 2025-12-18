#include "platform/platform_utils.h"
#include <string>

#ifdef _WIN32
    #define PLATFORM_WINDOWS 1
#elif __linux__
    #define PLATFORM_LINUX 1
#elif __APPLE__
    #define PLATFORM_MACOS 1
#endif

namespace platform {

std::string get_platform_name() {
#if defined(PLATFORM_WINDOWS)
    return "Windows";
#elif defined(PLATFORM_LINUX)
    return "Linux";
#elif defined(PLATFORM_MACOS)
    return "macOS";
#else
    return "Unknown";
#endif
}

bool is_windows() {
#if defined(PLATFORM_WINDOWS)
    return true;
#else
    return false;
#endif
}

bool is_linux() {
#if defined(PLATFORM_LINUX)
    return true;
#else
    return false;
#endif
}

bool is_macos() {
#if defined(PLATFORM_MACOS)
    return true;
#else
    return false;
#endif
}

std::string get_architecture() {
#ifdef _WIN64
    return "x86_64";
#elif defined(_WIN32)
    return "x86";
#elif defined(__x86_64__)
    return "x86_64";
#elif defined(__i386__)
    return "x86";
#elif defined(__arm64__) || defined(__aarch64__)
    return "ARM64";
#elif defined(__arm__)
    return "ARM32";
#else
    return "Unknown";
#endif
}

bool is_64bit() {
#ifdef _WIN64
    return true;
#elif defined(__x86_64__)
    return true;
#elif defined(__aarch64__)
    return true;
#else
    return false;
#endif
}

// 简化的SIMD检测（实际项目中可能需要更复杂的实现）
bool has_sse_support() {
#ifdef __SSE__
    return true;
#else
    return false;
#endif
}

bool has_avx_support() {
#ifdef __AVX__
    return true;
#else
    return false;
#endif
}

} // namespace platform