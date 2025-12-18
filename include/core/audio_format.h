#ifndef CORE_AUDIO_FORMAT_H
#define CORE_AUDIO_FORMAT_H

#include <string>
#include <map>

namespace core {

// 音频格式信息结构体
struct AudioFormat {
    int sample_rate;      // 采样率
    int channels;        // 声道数
    int bit_depth;       // 位深度
    std::string format;   // 格式名称
    
    // 构造函数
    AudioFormat() : sample_rate(0), channels(0), bit_depth(0) {}
    
    // 构造函数（带参数）
    AudioFormat(int sr, int ch, int bd, const std::string& fmt)
        : sample_rate(sr), channels(ch), bit_depth(bd), format(fmt) {}
};

} // namespace core

#endif // CORE_AUDIO_FORMAT_H