#include "foobar_input_adapter.h"
#include <iostream>

namespace audio {

FoobarInputAdapter::FoobarInputAdapter(std::shared_ptr<input_decoder> decoder)
    : foobar_decoder_(decoder), is_open_(false) {
    if (!decoder) {
        throw std::invalid_argument("Foobar decoder cannot be null");
    }
}

bool FoobarInputAdapter::open(const std::string& filename) {
    if (!foobar_decoder_) {
        return false;
    }

    filename_ = filename;
    is_open_ = true;

    // 在实际实现中，这里会调用Foobar2000的输入插件打开文件
    std::cout << "Opening file with Foobar2000 decoder: " << filename << std::endl;

    return true;
}

bool FoobarInputAdapter::close() {
    if (!is_open_) {
        return false;
    }

    is_open_ = false;
    filename_.clear();

    // 在实际实现中，这里会调用Foobar2000的输入插件关闭文件
    std::cout << "Closing file with Foobar2000 decoder" << std::endl;

    return true;
}

size_t FoobarInputAdapter::decode(float* buffer, size_t frames) {
    if (!is_open_ || !foobar_decoder_) {
        return 0;
    }

    // 在实际实现中，这里会调用Foobar2000的输入插件解码音频数据
    std::cout << "Decoding " << frames << " frames with Foobar2000 decoder" << std::endl;

    // 模拟解码过程
    return frames; // 返回实际解码的帧数
}

bool FoobarInputAdapter::seek(size_t frame) {
    if (!is_open_ || !foobar_decoder_) {
        return false;
    }

    // 在实际实现中，这里会调用Foobar2000的输入插件进行跳转
    std::cout << "Seeking to frame: " << frame << std::endl;

    return true;
}

std::map<std::string, std::string> FoobarInputAdapter::getMetadata() const {
    std::map<std::string, std::string> metadata;

    if (!is_open_ || !foobar_decoder_) {
        return metadata;
    }

    // 在实际实现中，这里会从Foobar2000插件获取元数据
    std::cout << "Getting metadata from Foobar2000 decoder" << std::endl;

    return metadata;
}

audio::AudioFormat FoobarInputAdapter::getFormat() const {
    audio::AudioFormat format;

    if (!is_open_ || !foobar_decoder_) {
        return format;
    }

    // 在实际实现中，这里会从Foobar2000插件获取音频格式信息
    std::cout << "Getting audio format from Foobar2000 decoder" << std::endl;

    // 占位符值，实际应该从Foobar2000插件获取
    format.sample_rate = 44100;
    format.format = audio::SampleFormat::PCM_FLOAT;
    format.channels = audio::ChannelLayout::STEREO;

    return format;
}

} // namespace audio