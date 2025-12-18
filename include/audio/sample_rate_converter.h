#ifndef AUDIO_SAMPLE_RATE_CONVERTER_H
#define AUDIO_SAMPLE_RATE_CONVERTER_H

#include <memory>
#include "audio/audio_buffer.h"
#include "audio/audio_format.h"

namespace audio {

// 重采样器接口类
class SampleRateConverter {
public:
    virtual ~SampleRateConverter() = default;
    
    // 设置输入和输出格式
    virtual bool set_formats(const AudioFormat& input_format, 
                           const AudioFormat& output_format) = 0;
    
    // 转换音频数据
    virtual bool convert(const AudioBuffer& input_buffer, 
                        AudioBuffer& output_buffer) = 0;
    
    // 获取转换质量等级（1-5）
    virtual int get_quality() const = 0;
    
    // 设置转换质量等级（1-5）
    virtual void set_quality(int quality) = 0;
};

// 工厂类用于创建重采样器
class SampleRateConverterFactory {
public:
    static std::unique_ptr<SampleRateConverter> create_converter();
};

} // namespace audio

#endif // AUDIO_SAMPLE_RATE_CONVERTER_H