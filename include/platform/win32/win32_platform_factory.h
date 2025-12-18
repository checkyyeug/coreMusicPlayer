#ifndef PLATFORM_WIN32_WIN32_PLATFORM_FACTORY_H
#define PLATFORM_WIN32_WIN32_PLATFORM_FACTORY_H

#include "platform/platform_factory.h"

namespace platform {
namespace win32 {

// Windows平台工厂类
class Win32PlatformFactory : public PlatformFactory {
public:
    Win32PlatformFactory();
    ~Win32PlatformFactory() override = default;
    
    // 创建音频管理器
    std::unique_ptr<AudioManager> createAudioManager() override;
    
    // 创建文件系统接口
    std::unique_ptr<FileSystemInterface> createFileSystem() override;
    
    // 创建网络接口
    std::unique_ptr<NetworkInterface> createNetworkInterface() override;
};

} // namespace win32
} // namespace platform

#endif // PLATFORM_WIN32_WIN32_PLATFORM_FACTORY_H