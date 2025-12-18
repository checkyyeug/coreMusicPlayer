#ifndef PLATFORM_PLATFORM_UTILS_H
#define PLATFORM_PLATFORM_UTILS_H

#include <string>
#include <vector>

namespace platform {

// 平台工具类
class PlatformUtils {
public:
    // 获取平台名称
    static std::string getPlatformName();
    
    // 检查是否为Windows平台
    static bool isWindows();
    
    // 检查是否为Linux平台
    static bool isLinux();
    
    // 检查是否为macOS平台
    static bool isMacOS();
    
    // 获取系统架构
    static std::string getArchitecture();
    
    // 获取系统内存信息
    static size_t getTotalMemory();
    
    // 获取可用CPU核心数
    static int getNumCpuCores();
    
    // 获取临时目录路径
    static std::string getTempDirectory();
    
    // 创建目录
    static bool createDirectory(const std::string& path);
    
    // 检查文件是否存在
    static bool fileExists(const std::string& path);
    
    // 删除文件
    static bool deleteFile(const std::string& path);
};

} // namespace platform

#endif // PLATFORM_PLATFORM_UTILS_H