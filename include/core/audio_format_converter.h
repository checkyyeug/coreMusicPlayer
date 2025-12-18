#ifndef CORE_AUDIO_FORMAT_CONVERTER_H
#define CORE_AUDIO_FORMAT_CONVERTER_H

#include "core/audio_buffer.h"
#include <string>

namespace core {

// 音频格式转换器类
class AudioFormatConverter {
public:
    // 构造函数
    AudioFormatConverter();
    
    // 析构函数
    ~AudioFormatConverter();
    
    // 初始化转换器
    bool initialize();
    
    // 关闭转换器
    void shutdown();
    
    // 转换音频格式
    bool convert(const AudioBuffer& input, 
                 AudioBuffer& output,
                 const std::string& target_format);
    
    // 设置转换参数
    bool setParameters(int sample_rate, int channels, int bit_depth);
    
    // 获取转换参数
    void getParameters(int& sample_rate, int& channels, int& bit_depth) const;
    
    // 获取支持的格式列表
    std::vector<std::string> getSupportedFormats() const;
    
private:
    // 私有成员变量
    bool initialized_;
    int sample_rate_;
    int channels_;
    int bit_depth_;
};

} // namespace core

#endif // CORE_AUDIO_FORMAT_CONVERTER_H