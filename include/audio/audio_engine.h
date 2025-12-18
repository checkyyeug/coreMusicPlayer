#ifndef AUDIO_AUDIO_ENGINE_H
#define AUDIO_AUDIO_ENGINE_H

#include <memory>
#include <string>
#include "audio/audio_buffer.h"
#include "audio/audio_format.h"

namespace audio {

// 音频引擎主类
class AudioEngine {
public:
    // 获取单例实例
    static std::shared_ptr<AudioEngine> instance();
    
    // 初始化音频引擎
    bool initialize();
    
    // 清理资源
    void cleanup();
    
    // 播放音频数据
    bool play_audio(const AudioBuffer& buffer, const AudioFormat& format);
    
    // 停止播放
    bool stop_playback();
    
    // 设置音量（0.0 - 1.0）
    bool set_volume(float volume);
    
    // 获取当前状态
    std::string get_status() const;
    
    // 检查是否正在播放
    bool is_playing() const;
    
private:
    AudioEngine();
    ~AudioEngine() = default;
    
    // 状态变量
    enum class EngineState {
        STOPPED,
        PLAYING,
        PAUSED
    };
    
    EngineState state_;
    float volume_;
};

} // namespace audio

#endif // AUDIO_AUDIO_ENGINE_H