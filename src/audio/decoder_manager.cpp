#include "audio/decoder_manager.h"
#include <algorithm>

namespace audio {

std::shared_ptr<DecoderManager> DecoderManager::instance() {
    static std::shared_ptr<DecoderManager> manager = 
        std::make_shared<DecoderManager>();
    return manager;
}

void DecoderManager::register_decoder(std::unique_ptr<DecoderInterface> decoder) {
    if (decoder) {
        decoders_.push_back(std::move(decoder));
    }
}

std::unique_ptr<DecoderInterface> DecoderManager::get_decoder_for_file(const std::string& file_path) const {
    // 根据文件扩展名查找合适的解码器
    std::string lower_path = file_path;
    std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(), ::tolower);
    
    for (const auto& decoder : decoders_) {
        if (decoder && decoder->can_decode(file_path)) {
            return std::unique_ptr<DecoderInterface>(decoder->clone());
        }
    }
    
    // 如果没有找到合适的解码器，返回空指针
    return nullptr;
}

std::unique_ptr<DecoderInterface> DecoderManager::get_decoder_for_format(const std::string& format_name) const {
    for (const auto& decoder : decoders_) {
        if (decoder && decoder->get_name() == format_name) {
            return std::unique_ptr<DecoderInterface>(decoder->clone());
        }
    }
    
    // 如果没有找到合适的解码器，返回空指针
    return nullptr;
}

std::vector<std::string> DecoderManager::get_registered_decoders() const {
    std::vector<std::string> names;
    for (const auto& decoder : decoders_) {
        if (decoder) {
            names.push_back(decoder->get_name());
        }
    }
    return names;
}

} // namespace audio