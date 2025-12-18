#ifndef PLATFORM_WIN32_WIN32_NETWORK_H
#define PLATFORM_WIN32_WIN32_NETWORK_H

#include "platform/network_interface.h"

namespace platform {
namespace win32 {

// Windows平台网络接口
class Win32Network : public NetworkInterface {
public:
    Win32Network();
    ~Win32Network() override = default;
    
    // 实现网络接口
    bool connect(const std::string& host, int port) override;
    bool disconnect() override;
    bool send(const std::string& data) override;
    std::string receive() override;
    bool isOnline() const override;
    
private:
    // Windows网络相关方法
    bool connected_;
};

} // namespace win32
} // namespace platform

#endif // PLATFORM_WIN32_WIN32_NETWORK_H