#ifndef CORE_AUDIO_PHASER_MODULATED_H
#define CORE_AUDIO_PHASER_MODULATED_H

#include "core/audio_buffer.h"
#include <memory>

namespace core {

// 音频调制相位器类
class AudioPhaserModulated {
public:
    // 构造函数
    AudioPhaserModulated();
    
    // 析构函数
    ~AudioPhaserModulated();
    
    // 初始化调制相位器
    bool initialize();
    
    // 关闭调制相位器
    void shutdown();
    
    // 应用调制相位效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置调制相位参数
    bool setParameters(float rate, float depth, float feedback, float mix, float modulation_rate, float modulation_depth);
    
    // 获取调制相位参数
    void getParameters(float& rate, float& depth, float& feedback, float& mix, float& modulation_rate, float& modulation_depth) const;
    
    // 重置调制相位器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    float rate_;              // 基础调制速率
    float depth_;             // 基础调制深度
    float feedback_;          // 反馈量
    float mix_;               // 混合比例
    float modulation_rate_;  // 调制速率
    float modulation_depth_; // 调制深度
};

} // namespace core

#endif // CORE_AUDIO_PHASER_MODULATED_H