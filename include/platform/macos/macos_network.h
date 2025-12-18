#ifndef PLATFORM_MACOS_MACOS_NETWORK_H
#define PLATFORM_MACOS_MACOS_NETWORK_H

#include "platform/network_interface.h"

namespace platform {
namespace macos {

// macOS平台网络接口
class MacosNetwork : public NetworkInterface {
public:
    MacosNetwork();
    ~MacosNetwork() override = default;
    
    // 实现网络接口
    bool connect(const std::string& host, int port) override;
    bool disconnect() override;
    bool send(const std::string& data) override;
    std::string receive() override;
    bool isOnline() const override;
    
private:
    // macOS网络相关方法
    bool connected_;
};

} // namespace macos
} // namespace platform

#endif // PLATFORM_MACOS_MACOS_NETWORK_H