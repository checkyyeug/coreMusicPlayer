#ifndef PLATFORM_PLATFORM_FACTORY_H
#define PLATFORM_PLATFORM_FACTORY_H

#include "platform/audio_manager.h"
#include "platform/filesystem_interface.h"
#include "platform/network_interface.h"
#include <memory>

namespace platform {

// 平台工厂基类
class PlatformFactory {
public:
    virtual ~PlatformFactory() = default;
    
    // 创建音频管理器
    virtual std::unique_ptr<AudioManager> createAudioManager() = 0;
    
    // 创建文件系统接口
    virtual std::unique_ptr<FileSystemInterface> createFileSystem() = 0;
    
    // 创建网络接口
    virtual std::unique_ptr<NetworkInterface> createNetworkInterface() = 0;
};

} // namespace platform

#endif // PLATFORM_PLATFORM_FACTORY_H