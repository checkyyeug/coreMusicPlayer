#ifndef CORE_AUDIO_EQUALIZER_H
#define CORE_AUDIO_EQUALIZER_H

#include "core/audio_buffer.h"
#include <vector>

namespace core {

// 音频均衡器类
class AudioEqualizer {
public:
    // 构造函数
    AudioEqualizer();
    
    // 析构函数
    ~AudioEqualizer();
    
    // 初始化均衡器
    bool initialize();
    
    // 关闭均衡器
    void shutdown();
    
    // 应用均衡效果
    bool apply(const AudioBuffer& input, AudioBuffer& output);
    
    // 设置频段增益
    bool setBandGain(int band_index, float gain);
    
    // 获取频段增益
    float getBandGain(int band_index) const;
    
    // 设置所有频段增益
    bool setAllBands(const std::vector<float>& gains);
    
    // 获取所有频段增益
    std::vector<float> getAllBands() const;
    
    // 设置均衡器参数
    bool setParameters(int num_bands, int sample_rate);
    
    // 获取均衡器参数
    void getParameters(int& num_bands, int& sample_rate) const;
    
    // 重置均衡器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    int num_bands_;
    int sample_rate_;
    std::vector<float> band_gains_;
};

} // namespace core

#endif // CORE_AUDIO_EQUALIZER_H