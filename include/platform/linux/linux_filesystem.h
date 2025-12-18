#ifndef PLATFORM_LINUX_LINUX_FILESYSTEM_H
#define PLATFORM_LINUX_LINUX_FILESYSTEM_H

#include "platform/filesystem_interface.h"

namespace platform {
namespace linux {

// Linux平台文件系统接口
class LinuxFileSystem : public FileSystemInterface {
public:
    LinuxFileSystem();
    ~LinuxFileSystem() override = default;
    
    // 实现文件系统接口
    bool fileExists(const std::string& path) override;
    bool createDirectory(const std::string& path) override;
    bool deleteFile(const std::string& path) override;
    std::vector<std::string> listFiles(const std::string& directory) override;
    std::string getTempDirectory() override;
    
private:
    // Linux文件系统相关方法
};

} // namespace linux
} // namespace platform

#endif // PLATFORM_LINUX_LINUX_FILESYSTEM_H