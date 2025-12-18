#ifndef CORE_AUDIO_SPECTRUM_ANALYZER_H
#define CORE_AUDIO_SPECTRUM_ANALYZER_H

#include "core/audio_buffer.h"
#include <vector>

namespace core {

// 音频频谱分析器类
class AudioSpectrumAnalyzer {
public:
    // 构造函数
    AudioSpectrumAnalyzer();
    
    // 析构函数
    ~AudioSpectrumAnalyzer();
    
    // 初始化分析器
    bool initialize();
    
    // 关闭分析器
    void shutdown();
    
    // 分析音频频谱
    std::vector<float> analyze(const AudioBuffer& buffer);
    
    // 获取频谱数据
    std::vector<float> getSpectrumData() const;
    
    // 设置分析参数
    bool setParameters(int fft_size, int window_type);
    
    // 获取分析参数
    void getParameters(int& fft_size, int& window_type) const;
    
    // 重置分析器
    void reset();
    
private:
    // 私有成员变量
    bool initialized_;
    int fft_size_;
    int window_type_;
    std::vector<float> spectrum_data_;
};

} // namespace core

#endif // CORE_AUDIO_SPECTRUM_ANALYZER_H