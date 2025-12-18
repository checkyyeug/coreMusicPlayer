#ifndef CORE_AUDIO_UTILS_H
#define CORE_AUDIO_UTILS_H

#include "core/audio_format.h"
#include "core/audio_metadata.h"
#include <string>

namespace core {

// 音频工具类
class AudioUtils {
public:
    // 获取音频文件格式
    static std::string getAudioFormat(const std::string& filename);
    
    // 解析音频元数据
    static AudioMetadata parseMetadata(const std::string& filename);
    
    // 计算音频持续时间
    static double calculateDuration(const std::string& filename);
    
    // 转换采样率
    static bool convertSampleRate(const std::string& input_file,
                                  const std::string& output_file,
                                  int target_sample_rate);
    
    // 音频格式转换
    static bool convertAudioFormat(const std::string& input_file,
                                   const std::string& output_file,
                                   const std::string& target_format);
};

} // namespace core

#endif // CORE_AUDIO_UTILS_H