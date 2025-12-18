#ifndef CORE_AUDIO_DYNAMIC_RANGE_COMPRESSOR_H
#define CORE_AUDIO_DYNAMIC_RANGE_COMPRESSOR_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频动态范围压缩器类
class AudioDynamicRangeCompressor {
public:
    // 构造函数
    AudioDynamicRangeCompressor();
    
    // 析构函数
    ~AudioDynamicRangeCompressor();
    
    // 初始化动态范围压缩器
    bool initialize();
    
    // 关闭动态范围压缩器
    void shutdown();
    
    // 应用动态范围压缩效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置压缩参数
    bool setParameters(float threshold, float ratio, float attack, float release, float makeup_gain);
    
    // 获取压缩参数
    void getParameters(float& threshold, float& ratio, float& attack, float& release, float& makeup_gain) const;
    
    // 重置动态范围压缩器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float threshold_;      // 阈值
    float ratio_;          // 压缩比
    float attack_;         // 攻击时间
    float release_;        // 释放时间
    float makeup_gain_;    // 补偿增益
};

} // namespace core

#endif // CORE_AUDIO_DYNAMIC_RANGE_COMPRESSOR_H