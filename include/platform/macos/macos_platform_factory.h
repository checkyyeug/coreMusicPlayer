#ifndef PLATFORM_MACOS_MACOS_PLATFORM_FACTORY_H
#define PLATFORM_MACOS_MACOS_PLATFORM_FACTORY_H

#include "platform/platform_factory.h"

namespace platform {
namespace macos {

// macOS平台工厂类
class MacosPlatformFactory : public PlatformFactory {
public:
    MacosPlatformFactory();
    ~MacosPlatformFactory() override = default;
    
    // 创建音频管理器
    std::unique_ptr<AudioManager> createAudioManager() override;
    
    // 创建文件系统接口
    std::unique_ptr<FileSystemInterface> createFileSystem() override;
    
    // 创建网络接口
    std::unique_ptr<NetworkInterface> createNetworkInterface() override;
};

} // namespace macos
} // namespace platform

#endif // PLATFORM_MACOS_MACOS_PLATFORM_FACTORY_H