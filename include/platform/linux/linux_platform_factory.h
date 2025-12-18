#ifndef PLATFORM_LINUX_LINUX_PLATFORM_FACTORY_H
#define PLATFORM_LINUX_LINUX_PLATFORM_FACTORY_H

#include "platform/platform_factory.h"

namespace platform {
namespace linux {

// Linux平台工厂类
class LinuxPlatformFactory : public PlatformFactory {
public:
    LinuxPlatformFactory();
    ~LinuxPlatformFactory() override = default;
    
    // 创建音频管理器
    std::unique_ptr<AudioManager> createAudioManager() override;
    
    // 创建文件系统接口
    std::unique_ptr<FileSystemInterface> createFileSystem() override;
    
    // 创建网络接口
    std::unique_ptr<NetworkInterface> createNetworkInterface() override;
};

} // namespace linux
} // namespace platform

#endif // PLATFORM_LINUX_LINUX_PLATFORM_FACTORY_H