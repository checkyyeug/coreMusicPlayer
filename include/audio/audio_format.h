#ifndef AUDIO_AUDIO_FORMAT_H
#define AUDIO_AUDIO_FORMAT_H

#include <cstdint>

namespace audio {

enum class SampleFormat {
    UNKNOWN = 0,
    PCM_S16LE,   // Signed 16-bit little endian
    PCM_S24LE,   // Signed 24-bit little endian
    PCM_S32LE,   // Signed 32-bit little endian
    PCM_FLOAT,   // 32-bit float
    PCM_DOUBLE   // 64-bit double
};

enum class ChannelLayout {
    UNKNOWN = 0,
    MONO,
    STEREO,
    QUAD,
    FIVE_POINT_ONE,
    SEVEN_POINT_ONE
};

struct AudioFormat {
    uint32_t sample_rate;      // 采样率 (Hz)
    SampleFormat format;       // 样本格式
    ChannelLayout channels;    // 声道布局
    
    // 构造函数
    AudioFormat() 
        : sample_rate(0), format(SampleFormat::UNKNOWN), channels(ChannelLayout::UNKNOWN) {}
    
    AudioFormat(uint32_t rate, SampleFormat fmt, ChannelLayout ch)
        : sample_rate(rate), format(fmt), channels(ch) {}
    
    // 检查格式是否有效
    bool is_valid() const {
        return sample_rate > 0 && 
               format != SampleFormat::UNKNOWN && 
               channels != ChannelLayout::UNKNOWN;
    }
};

} // namespace audio

#endif // AUDIO_AUDIO_FORMAT_H