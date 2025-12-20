#ifndef AUDIO_DECODER_MANAGER_H
#define AUDIO_DECODER_MANAGER_H

#include <memory>
#include <vector>
#include <string>
#include "audio/decoder_interface.h"

namespace audio {

class DecoderManager {
public:
    // 获取单例实例
    static std::shared_ptr<DecoderManager> instance();
    
    // 注册解码器
    void register_decoder(std::unique_ptr<DecoderInterface> decoder);
    
    // 根据文件扩展名获取合适的解码器
    std::unique_ptr<DecoderInterface> get_decoder_for_file(const std::string& file_path) const;
    
    // 根据格式获取解码器
    std::unique_ptr<DecoderInterface> get_decoder_for_format(const std::string& format_name) const;
    
    // 获取所有已注册的解码器名称
    std::vector<std::string> get_registered_decoders() const;
    
public:
    DecoderManager() = default;
    
    // 已注册的解码器列表
    std::vector<std::unique_ptr<DecoderInterface>> decoders_;
};

} // namespace audio

#endif // AUDIO_DECODER_MANAGER_H