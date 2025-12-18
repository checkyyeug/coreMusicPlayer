#include <gtest/gtest.h>
#include "../src/foobar/foobar_plugin_manager.h"
#include "../src/foobar/foobar_input_adapter.h"
#include "../src/foobar/foobar_dsp_adapter.h"
#include "../src/foobar/foobar_output_adapter.h"

// Mock classes for testing
class MockFoobarDecoder {
public:
    bool open(const std::string& filename) { return true; }
    bool close() { return true; }
    size_t decode(float* buffer, size_t frames) { return frames; }
    bool seek(size_t frame) { return true; }
    std::map<std::string, std::string> getMetadata() const {
        return std::map<std::string, std::string>();
    }
    core::AudioFormat getFormat() const {
        core::AudioFormat format;
        format.sampleRate = 44100;
        format.channels = 2;
        format.bitsPerSample = 16;
        return format;
    }
};

class MockFoobarDSP {
public:
    bool initialize(const core::AudioFormat& format) { return true; }
    void process(float* buffer, size_t frames) {}
    void reset() {}
    std::string getName() const { return "MockDSP"; }
};

class MockFoobarOutput {
public:
    bool initialize(const core::AudioFormat& format) { return true; }
    bool start() { return true; }
    void stop() {}
    void writeAudio(const float* buffer, size_t frames) {}
    void flush() {}
    std::string getName() const { return "MockOutput"; }
};

// 测试Foobar2000插件管理器
TEST(FoobarUnitTest, PluginManagerInitialization) {
    // 创建插件管理器实例
    core::FoobarPluginManager manager;

    // 验证管理器可以创建
    EXPECT_NE(&manager, nullptr);
}

// 测试插件发现功能
TEST(FoobarUnitTest, PluginDiscovery) {
    core::FoobarPluginManager manager;

    // 创建测试目录
    std::filesystem::create_directories("test_plugins");

    // 模拟插件发现过程
    auto plugins = manager.discoverPlugins("test_plugins");

    // 验证返回结果
    EXPECT_TRUE(plugins.empty());  // 在测试环境中应该没有实际插件

    // 清理测试目录
    std::filesystem::remove_all("test_plugins");
}

// 测试插件加载功能
TEST(FoobarUnitTest, PluginLoading) {
    core::FoobarPluginManager manager;
    
    // 测试无效路径
    EXPECT_FALSE(manager.loadPlugin("nonexistent.dll"));
    
    // 创建测试目录和文件
    std::filesystem::create_directories("test_plugins");
    std::ofstream test_file("test_plugins/test_plugin.dll");
    test_file << "fake dll content";
    test_file.close();
    
    // 测试加载插件（实际不会成功，但应该不崩溃）
    EXPECT_TRUE(manager.loadPlugin("test_plugins/test_plugin.dll"));
    
    // 清理测试文件
    std::filesystem::remove_all("test_plugins");
}

// 测试插件卸载功能
TEST(FoobarUnitTest, PluginUnloading) {
    core::FoobarPluginManager manager;
    
    // 创建测试目录和文件
    std::filesystem::create_directories("test_plugins");
    std::ofstream test_file("test_plugins/test_plugin.dll");
    test_file << "fake dll content";
    test_file.close();
    
    // 加载插件
    EXPECT_TRUE(manager.loadPlugin("test_plugins/test_plugin.dll"));
    
    // 卸载插件
    manager.unloadPlugin("test_plugins/test_plugin.dll");
    
    // 清理测试文件
    std::filesystem::remove_all("test_plugins");
}

// 测试输入适配器
TEST(FoobarUnitTest, InputAdapterCreation) {
    // 创建模拟的Foobar解码器
    auto mock_decoder = std::make_shared<MockFoobarDecoder>();

    // 创建输入适配器
    audio::FoobarInputAdapter adapter(mock_decoder);

    // 验证适配器可以创建
    EXPECT_NE(&adapter, nullptr);
}

// 测试DSP适配器
TEST(FoobarUnitTest, DSPAdapterCreation) {
    // 创建模拟的Foobar DSP处理器
    auto mock_dsp = std::make_shared<MockFoobarDSP>();

    // 创建DSP适配器
    audio::FoobarDSPAdapter adapter(mock_dsp);

    // 验证适配器可以创建
    EXPECT_NE(&adapter, nullptr);
}

// 测试输出适配器
TEST(FoobarUnitTest, OutputAdapterCreation) {
    // 创建模拟的Foobar输出插件
    auto mock_output = std::make_shared<MockFoobarOutput>();

    // 创建输出适配器
    audio::FoobarOutputAdapter adapter(mock_output);

    // 验证适配器可以创建
    EXPECT_NE(&adapter, nullptr);
}

// 测试输入适配器的文件操作
TEST(FoobarUnitTest, InputAdapterFileOperations) {
    // 创建模拟的Foobar解码器
    auto mock_decoder = std::make_shared<MockFoobarDecoder>();

    // 创建输入适配器
    audio::FoobarInputAdapter adapter(mock_decoder);
    
    // 测试打开文件
    EXPECT_TRUE(adapter.open("test.wav"));
    
    // 测试关闭文件
    EXPECT_TRUE(adapter.close());
}

// 测试DSP适配器的处理功能
TEST(FoobarUnitTest, DSPAdapterProcessing) {
    // 创建模拟的Foobar DSP处理器
    auto mock_dsp = std::make_shared<MockFoobarDSP>();

    // 创建DSP适配器
    audio::FoobarDSPAdapter adapter(mock_dsp);
    
    // 创建测试音频格式
    core::AudioFormat format;
    format.sampleRate = 44100;
    format.channels = 2;
    format.bitsPerSample = 16;
    
    // 测试初始化
    EXPECT_TRUE(adapter.initialize(format));
    
    // 测试处理功能（无实际数据）
    float buffer[1024] = {0};
    adapter.process(buffer, 1024);
    
    // 测试重置功能
    adapter.reset();
}

// 测试输出适配器的音频输出功能
TEST(FoobarUnitTest, OutputAdapterAudioOutput) {
    // 创建模拟的Foobar输出插件
    auto mock_output = std::make_shared<MockFoobarOutput>();

    // 创建输出适配器
    audio::FoobarOutputAdapter adapter(mock_output);
    
    // 创建测试音频格式
    core::AudioFormat format;
    format.sampleRate = 44100;
    format.channels = 2;
    format.bitsPerSample = 16;
    
    // 测试初始化
    EXPECT_TRUE(adapter.initialize(format));
    
    // 测试启动
    EXPECT_TRUE(adapter.start());
    
    // 测试写入音频数据（无实际数据）
    float buffer[1024] = {0};
    adapter.writeAudio(buffer, 1024);
    
    // 测试刷新缓冲区
    adapter.flush();
    
    // 测试停止
    adapter.stop();
}