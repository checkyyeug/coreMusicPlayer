#ifndef FOOBAR_OUTPUT_ADAPTER_H
#define FOOBAR_OUTPUT_ADAPTER_H

#include <string>
#include <memory>

// Forward declarations for Foobar2000 types
class output;

namespace audio {

// Foobar2000输出适配器
class FoobarOutputAdapter {
public:
    FoobarOutputAdapter(std::shared_ptr<output> output);
    ~FoobarOutputAdapter() = default;

    // Foobar2000输出插件接口
    bool initialize(const audio::AudioFormat& format);
    bool start();
    void stop();
    void writeAudio(const float* buffer, size_t frames);
    void flush();
    std::string getName() const;

private:
    std::shared_ptr<output> foobar_output_;
    bool initialized_;
    bool started_;
};

} // namespace audio

#endif // FOOBAR_OUTPUT_ADAPTER_H