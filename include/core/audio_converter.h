#ifndef CORE_AUDIO_CONVERTER_H
#define CORE_AUDIO_CONVERTER_H

#include "core/audio_buffer.h"
#include <string>
#include <memory>

namespace core {

// 音频转换器类
class AudioConverter {
public:
    // 构造函数
    AudioConverter();
    
    // 析构函数
    ~AudioConverter();
    
    // 初始化转换器
    bool initialize();
    
    // 关闭转换器
    void shutdown();
    
    // 转换音频格式
    bool convert(const std::string& input_file, 
                const std::string& output_file,
                const std::string& target_format);
    
    // 转换采样率
    bool convertSampleRate(const std::string& input_file,
                          const std::string& output_file,
                          int target_sample_rate);
    
    // 转换声道数
    bool convertChannels(const std::string& input_file,
                        const std::string& output_file,
                        int target_channels);
    
    // 转换位深度
    bool convertBitDepth(const std::string& input_file,
                        const std::string& output_file,
                        int target_bit_depth);
    
private:
    // 私有成员变量
    bool initialized_;
};

} // namespace core

#endif // CORE_AUDIO_CONVERTER_H