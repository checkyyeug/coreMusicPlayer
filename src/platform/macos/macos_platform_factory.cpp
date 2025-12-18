#include "platform/macos/macos_platform_factory.h"
#include "platform/macos/macos_audio_manager.h"
#include "platform/macos/macos_filesystem.h"
#include "platform/macos/macos_network.h"

namespace platform {
namespace macos {

MacosPlatformFactory::MacosPlatformFactory() {
    // 初始化macOS平台工厂
}

std::unique_ptr<AudioManager> MacosPlatformFactory::createAudioManager() {
    return std::make_unique<macos::MacosAudioManager>();
}

std::unique_ptr<FileSystemInterface> MacosPlatformFactory::createFileSystem() {
    return std::make_unique<macos::MacosFileSystem>();
}

std::unique_ptr<NetworkInterface> MacosPlatformFactory::createNetworkInterface() {
    return std::make_unique<macos::MacosNetwork>();
}

} // namespace macos
} // namespace platform