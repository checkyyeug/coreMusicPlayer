#ifndef CORE_AUDIO_EFFECTS_CHAIN_H
#define CORE_AUDIO_EFFECTS_CHAIN_H

#include "core/audio_buffer.h"
#include "core/audio_filter.h"
#include "core/audio_compressor.h"
#include "core/audio_delay.h"
#include "core/audio_reverb.h"
#include "core/audio_chorus.h"
#include "core/audio_flanger.h"
#include "core/audio_tremolo.h"
#include "core/audio_vocoder.h"
#include "core/audio_stereo_widener.h"
#include "core/audio_pitch_shifter.h"
#include "core/audio_time_stretch.h"
#include <vector>
#include <memory>

namespace core {

// 音频效果链类
class AudioEffectsChain {
public:
    // 构造函数
    AudioEffectsChain();
    
    // 析构函数
    ~AudioEffectsChain();
    
    // 初始化效果链
    bool initialize();
    
    // 关闭效果链
    void shutdown();
    
    // 添加效果到链中
    bool addEffect(std::unique_ptr<AudioFilter> effect);
    
    // 移除效果
    bool removeEffect(size_t index);
    
    // 应用效果链
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 清空效果链
    void clear();
    
    // 获取效果数量
    size_t getEffectCount() const;
    
    // 获取指定索引的效果
    std::unique_ptr<AudioFilter> getEffect(size_t index) const;
    
    // 启用/禁用效果
    bool enableEffect(size_t index, bool enabled);
    
    // 检查效果是否启用
    bool isEffectEnabled(size_t index) const;
    
private:
    // 私有成员变量
    bool initialized_;
    std::vector<std::unique_ptr<AudioFilter>> effects_;
    std::vector<bool> enabled_;
};

} // namespace core

#endif // CORE_AUDIO_EFFECTS_CHAIN_H