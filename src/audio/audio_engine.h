#ifndef AUDIO_AUDIO_ENGINE_H
#define AUDIO_AUDIO_ENGINE_H

#include <memory>
#include <string>
#include <vector>

namespace core {
class EqualizerConfig;
}

namespace audio {

// 音频格式枚举
enum class AudioFormatType {
    PCM_16BIT,
    PCM_24BIT,
    PCM_32BIT,
    FLOAT_32BIT
};

// 音频格式结构体
struct AudioFormat {
    AudioFormatType format;
    int sample_rate;
    int channels;
    
    AudioFormat() : format(AudioFormatType::PCM_16BIT), 
                    sample_rate(44100), 
                    channels(2) {}
};

// 音频缓冲区类型
using AudioBuffer = std::vector<float>;

// 引擎状态枚举
enum class EngineState {
    STOPPED,
    PLAYING,
    PAUSED
};

// 音频引擎类
class AudioEngine {
public:
    // 获取单例实例
    static std::shared_ptr<AudioEngine> instance();
    
    // 析构函数
    ~AudioEngine() = default;
    
    // 初始化音频引擎
    bool initialize();
    
    // 清理资源
    void cleanup();
    
    // 播放音频
    bool play_audio(const AudioBuffer& buffer, const AudioFormat& format);
    
    // 停止播放
    bool stop_playback();
    
    // 设置音量
    bool set_volume(float volume);
    
    // 获取状态
    std::string get_status() const;
    
    // 检查是否正在播放
    bool is_playing() const;
    
    // 设置均衡器配置
    void set_equalizer_config(std::shared_ptr<core::EqualizerConfig> config);

private:
    // 构造函数（私有，使用单例模式）
    AudioEngine();
    
    // 引擎状态
    EngineState state_;
    
    // 当前音量
    float volume_;
    
    // 均衡器配置
    std::shared_ptr<core::EqualizerConfig> equalizer_config_;
};

} // namespace audio

#endif // AUDIO_AUDIO_ENGINE_H