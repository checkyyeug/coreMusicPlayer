#ifndef PLATFORM_MACOS_MACOS_FILESYSTEM_H
#define PLATFORM_MACOS_MACOS_FILESYSTEM_H

#include "platform/filesystem_interface.h"

namespace platform {
namespace macos {

// macOS平台文件系统接口
class MacosFileSystem : public FileSystemInterface {
public:
    MacosFileSystem();
    ~MacosFileSystem() override = default;
    
    // 实现文件系统接口
    bool fileExists(const std::string& path) override;
    bool createDirectory(const std::string& path) override;
    bool deleteFile(const std::string& path) override;
    std::vector<std::string> listFiles(const std::string& directory) override;
    std::string getTempDirectory() override;
    
private:
    // macOS文件系统相关方法
};

} // namespace macos
} // namespace platform

#endif // PLATFORM_MACOS_MACOS_FILESYSTEM_H