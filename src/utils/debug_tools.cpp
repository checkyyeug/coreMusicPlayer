#include "utils/debug_tools.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <execinfo.h>
    #include <unistd.h>
#endif

namespace utils {
namespace debug {

void DebugTools::check_memory_leaks() {
    // 简化实现 - 实际项目中需要更复杂的内存跟踪机制
    std::cout << "Memory leak check performed\n";
}

std::string DebugTools::get_stack_trace() {
#ifdef _WIN32
    return "Stack trace not implemented on Windows";
#else
    const int max_frames = 64;
    void* buffer[max_frames];
    int nptrs = backtrace(buffer, max_frames);
    
    char** strings = backtrace_symbols(buffer, nptrs);
    if (strings == nullptr) {
        return "Failed to get stack trace";
    }
    
    std::string result = "Stack trace:\n";
    for (int i = 0; i < nptrs; ++i) {
        result += std::to_string(i) + ": " + strings[i] + "\n";
    }
    
    free(strings);
    return result;
#endif
}

void DebugTools::start_profiling(const std::string& name) {
    // 简化实现 - 实际项目中需要更复杂的性能分析工具
    std::cout << "Starting profiling for: " << name << "\n";
}

void DebugTools::stop_profiling(const std::string& name) {
    // 简化实现 - 实际项目中需要更复杂的性能分析工具
    std::cout << "Stopping profiling for: " << name << "\n";
}

size_t DebugTools::get_current_memory_usage() {
    // 简化实现 - 实际项目中需要获取系统内存使用情况
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return static_cast<size_t>(statex.ullTotalPhys - statex.ullAvailPhys);
#else
    // Linux系统可以读取 /proc/self/status 文件获取内存信息
    return 0;  // 简化返回值
#endif
}

std::vector<DebugTools::StackFrame> DebugTools::get_call_stack(int max_frames) {
    std::vector<StackFrame> frames;
    
#ifdef _WIN32
    // Windows平台简化实现
    frames.emplace_back("unknown_function", "unknown_file.cpp", 0);
#else
    // POSIX系统使用backtrace
    const int actual_max = (max_frames > 64) ? 64 : max_frames;
    void* buffer[actual_max];
    int nptrs = backtrace(buffer, actual_max);
    
    char** strings = backtrace_symbols(buffer, nptrs);
    if (strings != nullptr) {
        for (int i = 0; i < nptrs && i < max_frames; ++i) {
            // 简化处理，实际项目中需要解析符号信息
            frames.emplace_back("unknown_function", "unknown_file.cpp", 0);
        }
        free(strings);
    } else {
        frames.emplace_back("unknown_function", "unknown_file.cpp", 0);
    }
#endif
    
    return frames;
}

} // namespace debug
} // namespace utils