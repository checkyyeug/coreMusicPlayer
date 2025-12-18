#ifndef CORE_AUDIO_AUTO_WAH_H
#define CORE_AUDIO_AUTO_WAH_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频自动哇音器类
class AudioAutoWah {
public:
    // 构造函数
    AudioAutoWah();
    
    // 析构函数
    ~AudioAutoWah();
    
    // 初始化自动哇音器
    bool initialize();
    
    // 关闭自动哇音器
    void shutdown();
    
    // 应用自动哇音效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置自动哇音参数
    bool setParameters(float rate, float depth, float feedback, float mix);
    
    // 获取自动哇音参数
    void getParameters(float& rate, float& depth, float& feedback, float& mix) const;
    
    // 重置自动哇音器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float rate_;       // 调制速率
    float depth_;      // 调制深度
    float feedback_;   // 反馈量
    float mix_;        // 混合比例
};

} // namespace core

#endif // CORE_AUDIO_AUTO_WAH_H