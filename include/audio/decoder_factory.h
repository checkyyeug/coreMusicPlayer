#ifndef AUDIO_DECODER_FACTORY_H
#define AUDIO_DECODER_FACTORY_H

#include "audio/decoder_interface.h"
#include <memory>
#include <string>
#include <vector>

namespace audio {

// 解码器工厂类 - 负责创建和管理解码器实例
class DecoderFactory {
public:
    // 单例模式
    static DecoderFactory& get_instance();
    
    // 注册解码器
    void register_decoder(std::unique_ptr<DecoderInterface> decoder);
    
    // 获取解码器
    std::unique_ptr<DecoderInterface> get_decoder(const std::string& file_path) const;
    
    // 获取支持的所有格式
    std::vector<std::string> get_supported_formats() const;
    
    // 清理所有注册的解码器
    void clear_decoders();

private:
    // 私有构造函数
    DecoderFactory();
    
    // 解码器列表
    std::vector<std::unique_ptr<DecoderInterface>> decoders_;
};

} // namespace audio

#endif // AUDIO_DECODER_FACTORY_H