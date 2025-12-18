#ifndef PLATFORM_LINUX_LINUX_NETWORK_H
#define PLATFORM_LINUX_LINUX_NETWORK_H

#include "platform/network_interface.h"

namespace platform {
namespace linux {

// Linux平台网络接口
class LinuxNetwork : public NetworkInterface {
public:
    LinuxNetwork();
    ~LinuxNetwork() override = default;
    
    // 实现网络接口
    bool connect(const std::string& host, int port) override;
    bool disconnect() override;
    bool send(const std::string& data) override;
    std::string receive() override;
    bool isOnline() const override;
    
private:
    // Linux网络相关方法
    bool connected_;
};

} // namespace linux
} // namespace platform

#endif // PLATFORM_LINUX_LINUX_NETWORK_H