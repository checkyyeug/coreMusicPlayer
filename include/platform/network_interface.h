#ifndef PLATFORM_NETWORK_INTERFACE_H
#define PLATFORM_NETWORK_INTERFACE_H

#include <string>

namespace platform {

// 网络接口基类
class NetworkInterface {
public:
    virtual ~NetworkInterface() = default;
    
    // 连接到网络
    virtual bool connect(const std::string& host, int port) = 0;
    
    // 断开网络连接
    virtual bool disconnect() = 0;
    
    // 发送数据
    virtual bool send(const std::string& data) = 0;
    
    // 接收数据
    virtual std::string receive() = 0;
    
    // 检查是否在线
    virtual bool isOnline() const = 0;
};

} // namespace platform

#endif // PLATFORM_NETWORK_INTERFACE_H