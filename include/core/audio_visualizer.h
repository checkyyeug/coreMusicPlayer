#ifndef CORE_AUDIO_VISUALIZER_H
#define CORE_AUDIO_VISUALIZER_H

#include "core/audio_buffer.h"
#include <vector>
#include <memory>

namespace core {

// 音频可视化器类
class AudioVisualizer {
public:
    // 构造函数
    AudioVisualizer();
    
    // 析构函数
    ~AudioVisualizer();
    
    // 初始化可视化器
    bool initialize();
    
    // 关闭可视化器
    void shutdown();
    
    // 生成频谱图数据
    std::vector<float> generateSpectrum(const AudioBuffer& buffer);
    
    // 生成波形图数据
    std::vector<float> generateWaveform(const AudioBuffer& buffer);
    
    // 生成频谱图和波形图数据
    void generateVisualizations(const AudioBuffer& buffer, 
                               std::vector<float>& spectrum_data,
                               std::vector<float>& waveform_data);
    
    // 设置可视化参数
    bool setParameters(int fft_size, int window_type);
    
    // 获取可视化参数
    void getParameters(int& fft_size, int& window_type) const;
    
private:
    // 私有成员变量
    bool initialized_;
    int fft_size_;
    int window_type_;
};

} // namespace core

#endif // CORE_AUDIO_VISUALIZER_H