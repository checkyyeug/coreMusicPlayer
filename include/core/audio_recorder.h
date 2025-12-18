#ifndef CORE_AUDIO_RECORDER_H
#define CORE_AUDIO_RECORDER_H

#include "core/audio_buffer.h"
#include <string>
#include <memory>

namespace core {

// 音频录制器类
class AudioRecorder {
public:
    // 构造函数
    AudioRecorder();
    
    // 析构函数
    ~AudioRecorder();
    
    // 初始化录制器
    bool initialize();
    
    // 关闭录制器
    void shutdown();
    
    // 开始录制
    bool startRecording(const std::string& filename);
    
    // 停止录制
    bool stopRecording();
    
    // 暂停录制
    bool pauseRecording();
    
    // 恢复录制
    bool resumeRecording();
    
    // 获取录制状态
    bool isRecording() const;
    
    // 获取录制时长
    double getDuration() const;
    
    // 获取录制文件名
    std::string getFilename() const;
    
    // 设置录音参数
    bool setParameters(int sample_rate, int channels, int bit_depth);
    
    // 获取录音参数
    void getParameters(int& sample_rate, int& channels, int& bit_depth) const;
    
    // 添加音频数据到录制缓冲区
    bool addAudioData(const AudioBuffer& buffer);
    
    // 保存录制的音频数据
    bool saveRecording();
    
private:
    // 私有成员变量
    bool initialized_;
    bool recording_;
    std::string filename_;
    double duration_;
    int sample_rate_;
    int channels_;
    int bit_depth_;
    AudioBuffer audio_buffer_;
};

} // namespace core

#endif // CORE_AUDIO_RECORDER_H