#include "platform/linux/linux_platform_factory.h"
#include "platform/linux/linux_audio_manager.h"
#include "platform/linux/linux_filesystem.h"
#include "platform/linux/linux_network.h"

namespace platform {
namespace linux {

LinuxPlatformFactory::LinuxPlatformFactory() {
    // 初始化Linux平台工厂
}

std::unique_ptr<AudioManager> LinuxPlatformFactory::createAudioManager() {
    return std::make_unique<linux::LinuxAudioManager>();
}

std::unique_ptr<FileSystemInterface> LinuxPlatformFactory::createFileSystem() {
    return std::make_unique<linux::LinuxFileSystem>();
}

std::unique_ptr<NetworkInterface> LinuxPlatformFactory::createNetworkInterface() {
    return std::make_unique<linux::LinuxNetwork>();
}

} // namespace linux
} // namespace platform