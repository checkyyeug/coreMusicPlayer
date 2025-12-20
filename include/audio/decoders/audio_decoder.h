#ifndef AUDIO_DECODERS_AUDIO_DECODER_H
#define AUDIO_DECODERS_AUDIO_DECODER_H

#include <string>
#include <map>

namespace audio {

// 音频格式枚举（解码器专用）
enum class DecoderAudioFormat {
    PCM_S16,
    PCM_S32,
    PCM_FLOAT,
    PCM_DOUBLE
};

// 音频解码器基类
class AudioDecoder {
public:
    virtual ~AudioDecoder() = default;
    
    // 打开音频文件
    virtual bool open(const std::string& filename) = 0;
    
    // 关闭音频文件
    virtual bool close() = 0;
    
    // 解码音频数据
    virtual size_t decode(float* buffer, size_t frames) = 0;
    
    // 跳转到指定帧
    virtual bool seek(size_t frame) = 0;
    
    // 获取元数据
    virtual std::map<std::string, std::string> getMetadata() const = 0;
    
    // 获取音频格式信息
    virtual DecoderAudioFormat getFormat() const = 0;
};

} // namespace audio

#endif // AUDIO_DECODERS_AUDIO_DECODER_H