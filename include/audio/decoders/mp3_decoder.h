#ifndef AUDIO_DECODERS_MP3_DECODER_H
#define AUDIO_DECODERS_MP3_DECODER_H

#include "audio/decoder_interface.h"
#include <string>

namespace audio {
namespace decoders {

// MP3格式解码器实现（使用minimp3库）
class Mp3Decoder : public DecoderInterface {
public:
    // 构造函数
    Mp3Decoder();
    
    // 析构函数
    ~Mp3Decoder() override;
    
    // 检查文件是否为MP3格式
    bool can_decode(const std::string& file_path) const override;
    
    // 解码音频数据
    DecodeResult decode(const std::string& file_path, 
                       std::vector<uint8_t>& output_buffer) override;
    
    // 获取元数据
    Metadata get_metadata(const std::string& file_path) override;
    
    // 获取支持的格式列表
    std::vector<std::string> get_supported_formats() const override;
    
    // 克隆方法（用于工厂模式）
    DecoderInterface* clone() const override;

private:
    // 私有辅助函数
    bool read_mp3_header(const std::string& file_path, 
                         uint32_t& sample_rate, 
                         uint16_t& channels);
};

} // namespace decoders
} // namespace audio

#endif // AUDIO_DECODERS_MP3_DECODER_H