#include "audio/decoders/wav_decoder.h"
#include <fstream>
#include <iostream>

namespace audio {
namespace decoders {

WavDecoder::WavDecoder() = default;

WavDecoder::~WavDecoder() = default;

bool WavDecoder::can_decode(const std::string& file_path) const {
    // 简化实现 - 实际项目中需要检查文件头
    return file_path.length() > 4 && 
           file_path.substr(file_path.length() - 4) == ".wav";
}

DecodeResult WavDecoder::decode(const std::string& file_path, 
                              std::vector<uint8_t>& output_buffer) {
    DecodeResult result;
    
    // 简化实现 - 实际项目中需要读取WAV文件并解码
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        result.success = false;
        result.error_message = "Failed to open file";
        return result;
    }
    
    // 模拟解码过程
    output_buffer.resize(1024); // 占位符数据
    result.success = true;
    result.sample_rate = 44100;
    result.bits_per_sample = 16;
    result.channels = 2;
    result.duration_seconds = 180.0; // 假设3分钟
    
    std::cout << "WAV decoder: Decoded " << file_path << std::endl;
    return result;
}

Metadata WavDecoder::get_metadata(const std::string& file_path) {
    Metadata metadata;
    
    // 简化实现 - 实际项目中需要解析WAV文件头获取元数据
    metadata.title = "Unknown Title";
    metadata.artist = "Unknown Artist";
    metadata.album = "Unknown Album";
    metadata.duration_seconds = 180.0; // 假设3分钟
    
    std::cout << "WAV decoder: Got metadata for " << file_path << std::endl;
    return metadata;
}

std::vector<std::string> WavDecoder::get_supported_formats() const {
    return {"wav", "wave"};
}

} // namespace decoders
} // namespace audio