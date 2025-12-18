#ifndef PLATFORM_WIN32_WIN32_FILESYSTEM_H
#define PLATFORM_WIN32_WIN32_FILESYSTEM_H

#include "platform/filesystem_interface.h"

namespace platform {
namespace win32 {

// Windows平台文件系统接口
class Win32FileSystem : public FileSystemInterface {
public:
    Win32FileSystem();
    ~Win32FileSystem() override = default;
    
    // 实现文件系统接口
    bool fileExists(const std::string& path) override;
    bool createDirectory(const std::string& path) override;
    bool deleteFile(const std::string& path) override;
    std::vector<std::string> listFiles(const std::string& directory) override;
    std::string getTempDirectory() override;
    
private:
    // Windows文件系统相关方法
};

} // namespace win32
} // namespace platform

#endif // PLATFORM_WIN32_WIN32_FILESYSTEM_H