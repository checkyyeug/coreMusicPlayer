#ifndef PLATFORM_FILESYSTEM_INTERFACE_H
#define PLATFORM_FILESYSTEM_INTERFACE_H

#include <string>
#include <vector>

namespace platform {

// 文件系统接口基类
class FileSystemInterface {
public:
    virtual ~FileSystemInterface() = default;
    
    // 检查文件是否存在
    virtual bool fileExists(const std::string& path) = 0;
    
    // 创建目录
    virtual bool createDirectory(const std::string& path) = 0;
    
    // 删除文件
    virtual bool deleteFile(const std::string& path) = 0;
    
    // 列出目录中的文件
    virtual std::vector<std::string> listFiles(const std::string& directory) = 0;
    
    // 获取临时目录路径
    virtual std::string getTempDirectory() = 0;
};

} // namespace platform

#endif // PLATFORM_FILESYSTEM_INTERFACE_H