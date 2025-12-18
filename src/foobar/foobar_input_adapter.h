#ifndef FOOBAR_INPUT_ADAPTER_H
#define FOOBAR_INPUT_ADAPTER_H

#include <string>
#include <memory>
#include <map>

// Forward declarations for Foobar2000 types
class input_decoder;

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

// Foobar2000输入适配器
class FoobarInputAdapter {
public:
    FoobarInputAdapter(std::shared_ptr<input_decoder> decoder);
    ~FoobarInputAdapter() = default;

    // Foobar2000解码器接口
    bool open(const std::string& filename);
    bool close();
    size_t decode(float* buffer, size_t frames);
    bool seek(size_t frame);
    std::map<std::string, std::string> getMetadata() const;
    audio::AudioFormat getFormat() const;

private:
    std::shared_ptr<input_decoder> foobar_decoder_;
    std::string filename_;
    bool is_open_;
};

} // namespace audio

#endif // FOOBAR_INPUT_ADAPTER_H