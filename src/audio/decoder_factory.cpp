#include "audio/decoder_factory.h"
#include "audio/decoders/wav_decoder.h"
#include <algorithm>

namespace audio {

DecoderFactory::DecoderFactory() = default;

DecoderFactory& DecoderFactory::get_instance() {
    static DecoderFactory instance;
    return instance;
}

void DecoderFactory::register_decoder(std::unique_ptr<DecoderInterface> decoder) {
    decoders_.push_back(std::move(decoder));
}

std::unique_ptr<DecoderInterface> DecoderFactory::get_decoder(const std::string& file_path) const {
    // 遍历所有注册的解码器，找到能处理该文件的解码器
    for (const auto& decoder : decoders_) {
        if (decoder->can_decode(file_path)) {
            return std::unique_ptr<DecoderInterface>(decoder->clone());
        }
    }
    
    // 如果没有找到合适的解码器，返回空指针
    return nullptr;
}

std::vector<std::string> DecoderFactory::get_supported_formats() const {
    std::vector<std::string> formats;
    for (const auto& decoder : decoders_) {
        auto decoder_formats = decoder->get_supported_formats();
        formats.insert(formats.end(), decoder_formats.begin(), decoder_formats.end());
    }
    return formats;
}

void DecoderFactory::clear_decoders() {
    decoders_.clear();
}

} // namespace audio