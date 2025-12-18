#include "audio/decoders/mp3_decoder.h"
#include <fstream>
#include <iostream>

namespace audio {
namespace decoders {

Mp3Decoder::Mp3Decoder() = default;

Mp3Decoder::~Mp3Decoder() = default;

bool Mp3Decoder::can_decode(const std::string& file_path) const {
    // 简化实现 - 实际项目中需要检查文件头
    return file_path.length() > 4 && 
           file_path.substr(file_path.length() - 4) == ".mp3";
}

DecodeResult Mp3Decoder::decode(const std::string& file_path, 
                               std::vector<uint8_t>& output_buffer) {
    DecodeResult result;
    
    // 简化实现 - 实际项目中需要使用minimp3库解码
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        result.success = false;
        result.error_message = "Failed to open file";
        return result;
    }
    
    // 模拟解码过程
    output_buffer.resize(2048); // 占位符数据
    result.success = true;
    result.sample_rate = 44100;
    result.bits_per_sample = 16;
    result.channels = 2;
    result.duration_seconds = 180.0; // 假设3分钟
    
    std::cout << "MP3 decoder: Decoded " << file_path << std::endl;
    return result;
}

Metadata Mp3Decoder::get_metadata(const std::string& file_path) {
    Metadata metadata;
    
    // 简化实现 - 实际项目中需要解析MP3文件获取元数据
    metadata.title = "Unknown Title";
    metadata.artist = "Unknown Artist";
    metadata.album = "Unknown Album";
    metadata.duration_seconds = 180.0; // 假设3分钟
    
    std::cout << "MP3 decoder: Got metadata for " << file_path << std::endl;
    return metadata;
}

std::vector<std::string> Mp3Decoder::get_supported_formats() const {
    return {"mp3"};
}

DecoderInterface* Mp3Decoder::clone() const {
    return new Mp3Decoder(*this);
}

} // namespace decoders
} // namespace audio