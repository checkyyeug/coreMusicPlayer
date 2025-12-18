#ifndef FOOBAR_DSP_ADAPTER_H
#define FOOBAR_DSP_ADAPTER_H

#include <string>
#include <memory>
#include <vector>

// Forward declarations for Foobar2000 types
class dsp_chain_entry;

namespace audio {

// 前向声明 AudioFormat 类型
enum class AudioFormat {
    PCM_S16,
    PCM_S32,
    PCM_FLOAT,
    PCM_DOUBLE
};

enum class ChannelLayout {
    MONO,
    STEREO
};

// Foobar2000 DSP处理器适配器
class FoobarDSPAdapter {
public:
    FoobarDSPAdapter(std::shared_ptr<dsp_chain_entry> dsp);
    ~FoobarDSPAdapter() = default;

    // Foobar2000 DSP处理器接口
    bool initialize(const audio::AudioFormat& format);
    void process(float* buffer, size_t frames);
    void reset();
    std::string getName() const;

private:
    std::shared_ptr<dsp_chain_entry> foobar_dsp_;
    bool initialized_;
};

} // namespace audio

#endif // FOOBAR_DSP_ADAPTER_H