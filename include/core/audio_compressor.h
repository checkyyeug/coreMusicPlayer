#ifndef CORE_AUDIO_COMPRESSOR_H
#define CORE_AUDIO_COMPRESSOR_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频压缩器类
class AudioCompressor {
public:
    // 构造函数
    AudioCompressor();
    
    // 析构函数
    ~AudioCompressor();
    
    // 初始化压缩器
    bool initialize();
    
    // 关闭压缩器
    void shutdown();
    
    // 应用压缩效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置压缩参数
    bool setParameters(float threshold, float ratio, float attack, float release);
    
    // 获取压缩参数
    void getParameters(float& threshold, float& ratio, float& attack, float& release) const;
    
    // 重置压缩器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float threshold_;   // 阈值
    float ratio_;       // 压缩比
    float attack_;      // 攻击时间
    float release_;     // 释放时间
};

} // namespace core

#endif // CORE_AUDIO_COMPRESSOR_H