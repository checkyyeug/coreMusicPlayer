#ifndef UTILS_DEBUG_TOOLS_H
#define UTILS_DEBUG_TOOLS_H

#include <string>
#include <vector>

namespace utils {
namespace debug {

// 调试工具类
class DebugTools {
public:
    // 内存泄漏检测（简化实现）
    static void check_memory_leaks();
    
    // 堆栈跟踪（简化实现）
    static std::string get_stack_trace();
    
    // 性能分析工具
    static void start_profiling(const std::string& name);
    static void stop_profiling(const std::string& name);
    
    // 内存使用统计
    static size_t get_current_memory_usage();
    
    // 堆栈跟踪信息（用于调试）
    struct StackFrame {
        std::string function_name;
        std::string file_name;
        int line_number;
        
        StackFrame(const std::string& func, const std::string& file, int line)
            : function_name(func), file_name(file), line_number(line) {}
    };
    
    // 获取当前调用栈
    static std::vector<StackFrame> get_call_stack(int max_frames = 10);
};

} // namespace debug
} // namespace utils

#endif // UTILS_DEBUG_TOOLS_H