#ifndef AUDIO_DECODER_INTERFACE_H
#define AUDIO_DECODER_INTERFACE_H

#include <string>
#include <memory>
#include <vector>
#include "audio/audio_buffer.h"
#include "audio/audio_format.h"

namespace audio {

// 解码器接口类
class DecoderInterface {
public:
    virtual ~DecoderInterface() = default;
    
    // 获取解码器名称
    virtual std::string get_name() const = 0;
    
    // 检查文件格式是否支持
    virtual bool can_decode(const std::string& file_path) const = 0;
    
    // 解码音频数据到缓冲区
    virtual bool decode_file(const std::string& file_path, 
                            AudioBuffer& buffer,
                            AudioFormat& format) = 0;
    
    // 解码内存中的音频数据
    virtual bool decode_buffer(const void* data, size_t size,
                              AudioBuffer& buffer,
                              AudioFormat& format) = 0;
    
    // 获取元数据信息
    virtual std::string get_metadata(const std::string& file_path) const = 0;
    
    // 克隆解码器（用于工厂模式）
    virtual std::unique_ptr<DecoderInterface> clone() const = 0;
    
    // 获取支持的格式
    virtual std::vector<std::string> get_supported_formats() const = 0;
};

} // namespace audio

#endif // AUDIO_DECODER_INTERFACE_H