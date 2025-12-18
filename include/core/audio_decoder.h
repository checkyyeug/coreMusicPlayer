#ifndef CORE_AUDIO_DECODER_H
#define CORE_AUDIO_DECODER_H

#include "core/audio_buffer.h"
#include <string>
#include <memory>

namespace core {

// 音频解码器基类
class AudioDecoder {
public:
    virtual ~AudioDecoder() = default;
    
    // 解码音频文件
    virtual bool decode(const std::string& filename, AudioBuffer& buffer) = 0;
    
    // 获取音频格式信息
    virtual std::string getFormat() const = 0;
    
    // 获取元数据
    virtual std::map<std::string, std::string> getMetadata() const = 0;
};

} // namespace core

#endif // CORE_AUDIO_DECODER_H