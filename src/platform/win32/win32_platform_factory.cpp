#include "platform/win32/win32_platform_factory.h"
#include "platform/win32/win32_audio_manager.h"
#include "platform/win32/win32_filesystem.h"
#include "platform/win32/win32_network.h"

namespace platform {
namespace win32 {

Win32PlatformFactory::Win32PlatformFactory() {
    // 初始化Windows平台工厂
}

std::unique_ptr<AudioManager> Win32PlatformFactory::createAudioManager() {
    return std::make_unique<win32::Win32AudioManager>();
}

std::unique_ptr<FileSystemInterface> Win32PlatformFactory::createFileSystem() {
    return std::make_unique<win32::Win32FileSystem>();
}

std::unique_ptr<NetworkInterface> Win32PlatformFactory::createNetworkInterface() {
    return std::make_unique<win32::Win32Network>();
}

} // namespace win32
} // namespace platform