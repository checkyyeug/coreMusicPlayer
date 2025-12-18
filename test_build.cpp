#include <iostream>
#include <vector>
#include "audio/audio_engine.h"
#include "core/equalizer_config.h"

int main() {
    std::cout << "Testing core music player components...\n";
    
    // 测试音频引擎
    auto engine = audio::AudioEngine::instance();
    if (engine->initialize()) {
        std::cout << "Audio engine initialized successfully\n";
        
        // 创建测试音频数据
        audio::AudioBuffer buffer(1024, 0.5f); // 1024个样本，值为0.5
        audio::AudioFormat format;
        format.sample_rate = 44100;
        format.channels = 2;
        
        // 测试播放音频
        if (engine->play_audio(buffer, format)) {
            std::cout << "Audio playback test passed\n";
        } else {
            std::cout << "Audio playback test failed\n";
        }
        
        // 测试音量设置
        if (engine->set_volume(0.8f)) {
            std::cout << "Volume setting test passed\n";
        } else {
            std::cout << "Volume setting test failed\n";
        }
        
        // 测试均衡器配置
        auto equalizer_config = std::make_shared<core::EqualizerConfig>();
        if (equalizer_config) {
            std::cout << "Equalizer config created successfully\n";
            
            // 加载预设
            equalizer_config->load_preset("rock");
            std::cout << "Loaded 'rock' preset\n";
            
            // 启用均衡器
            equalizer_config->enable(true);
            if (equalizer_config->is_enabled()) {
                std::cout << "Equalizer enabled successfully\n";
            }
            
            // 设置到音频引擎
            engine->set_equalizer_config(equalizer_config);
            std::cout << "Equalizer config set to audio engine\n";
        }
        
        // 清理资源
        engine->cleanup();
        std::cout << "Audio engine cleaned up\n";
    } else {
        std::cout << "Failed to initialize audio engine\n";
    }
    
    std::cout << "All tests completed.\n";
    return 0;
}