#ifndef AUDIO_DECODERS_FLAC_DECODER_H
#define AUDIO_DECODERS_FLAC_DECODER_H

#include "audio/decoders/audio_decoder.h"
#include <string>

namespace audio {
namespace decoders {

// FLAC格式解码器
class FlacDecoder : public AudioDecoder {
public:
    FlacDecoder();
    ~FlacDecoder() override = default;
    
    // 实现音频解码接口
    bool open(const std::string& filename) override;
    bool close() override;
    size_t decode(float* buffer, size_t frames) override;
    bool seek(size_t frame) override;
    std::map<std::string, std::string> getMetadata() const override;
    DecoderAudioFormat getFormat() const override;
    
    // FLAC特定方法
    bool isFlacFile(const std::string& filename) const;

private:
    std::string filename_;
    bool is_open_;
};

} // namespace decoders
} // namespace audio

#endif // AUDIO_DECODERS_FLAC_DECODER_H