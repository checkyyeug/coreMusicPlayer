# coreMusicPlayer 术语表与规范

## 文档目的

本文档定义了 coreMusicPlayer 项目中使用的统一术语、命名规范和标准定义，作为所有其他文档的权威参考源。

## 一、播放器模式定义

### 1.1 统一播放器架构

coreMusicPlayer 采用统一播放器架构，将原有的5个独立播放器整合为一个可执行文件，通过策略模式实现运行时模式切换。

### 1.2 播放器模式定义

| 模式名称 | 代码标识 | 描述 | 特点 | 使用场景 |
|---------|---------|------|------|----------|
| **Legacy** | `PlayerMode::Legacy` | 基础WAV播放器 | • 最小依赖<br>• 仅支持WAV格式<br>• 2KB缓冲区 | 嵌入式系统、简单播放需求 |
| **Complete** | `PlayerMode::Complete` | 全功能播放器 | • 所有功能启用<br>• 完整插件系统<br>• 高级DSP<br>• 4KB缓冲区 | 桌面音乐播放器 |
| **Realtime** | `PlayerMode::Realtime` | 实时音频处理器 | • 极低延迟<br>• 64采样点缓冲<br>• 实时优先级<br>• 禁用插件 | 现场表演、DJ软件 |
| **Production** | `PlayerMode::Production` | 生产级播放器 | • 最大稳定性<br>• 错误恢复<br>• 详细日志<br>• 可选插件 | 专业音频制作 |
| **MultiFormat** | `PlayerMode::MultiFormat` | 多格式播放器（默认） | • 平衡性能<br>• 全解码器加载<br>• 智能格式检测<br>• 2KB缓冲区 | 通用音频播放器 |

### 1.3 模式切换规范

```cpp
// 统一的模式切换API
enum class PlayerMode {
    Legacy = 0,
    Complete = 1,
    Realtime = 2,
    Production = 3,
    MultiFormat = 4  // 默认值
};
```

## 二、核心功能定义

### 2.1 音频格式支持

| 格式 | 编解码器 | 扩展名 | 备注 |
|------|---------|--------|------|
| WAV | 内置 | .wav | 支持8/16/24/32位 |
| MP3 | minimp3 | .mp3 | 支持ID3标签 |
| FLAC | libflac | .flac | 无损压缩 |
| OGG Vorbis | libogg | .ogg | 开源格式 |

### 2.2 核心功能列表

| 功能类别 | 功能项 | 必需性 | 特性标识 |
|---------|--------|--------|----------|
| 基础播放 | 音频文件加载 | 必需 | BasicPlayback |
| 基础播放 | 播放/暂停/停止 | 必需 | BasicPlayback |
| 基础播放 | 音量控制 | 必需 | VolumeControl |
| 音频处理 | 采样率转换 | 必需 | SampleRateConversion |
| 音频处理 | SIMD优化 | 推荐 | SIMDOptimization |
| 音频处理 | 高级DSP | 可选 | AdvancedDSP |
| 系统功能 | 插件支持 | 可选 | PluginSupport |
| 系统功能 | 实时模式 | 可选 | RealtimeMode |
| 系统功能 | 错误恢复 | 推荐 | ErrorRecovery |

### 2.3 功能依赖关系

```
BasicPlayback (基础)
    ├── VolumeControl (依赖)
    ├── SampleRateConversion (依赖)
    └── MultiFormat (依赖)
            ├── MP3Support
            ├── FLACSupport
            └── OGGSupport

AdvancedDSP (依赖)
    └── SampleRateConversion

RealtimeMode (依赖)
    ├── LowLatency
    └── SIMDOptimization

PluginSupport (依赖)
    └── BasicPlayback
```

## 三、性能指标标准

### 3.1 延迟要求

| 模式 | 目标延迟 | 最大可接受延迟 | 测试条件 |
|------|----------|--------------|----------|
| Standard | < 50ms | 100ms | 默认配置 |
| Realtime | < 10ms | 20ms | 64样本缓冲 |
| Production | < 20ms | 50ms | 启用错误恢复 |

### 3.2 资源使用限制

| 指标 | 目标值 | 最大值 | 测试方法 |
|------|--------|--------|----------|
| CPU使用率 | < 5% | 10% | 播放48kHz立体声 |
| 内存使用 | < 100MB | 200MB | 10万曲目库 |
| 启动时间 | < 2秒 | 3秒 | 冷启动 |
| 响应时间 | < 100ms | 200ms | UI操作响应 |

### 3.3 音频质量标准

| 参数 | 最小值 | 标准值 | 最大值 |
|------|--------|--------|--------|
| 采样率 | 8kHz | 44.1kHz/48kHz | 192kHz |
| 位深度 | 16-bit | 24-bit | 32-bit float |
| 通道数 | 1(单声道) | 2(立体声) | 8 |
| 动态范围 | 90dB | >120dB | - |
| THD+N | - | < 0.01% | 0.1% |

## 四、构建环境规范

### 4.1 最低系统要求

#### Windows
- **操作系统**: Windows 10 1903 或更高版本
- **开发工具**: Visual Studio 2019 16.8 或更高版本
- **Windows SDK**: 10.0.19041 或更高版本
- **C++标准**: C++17

#### Linux
- **发行版**: Ubuntu 20.04 LTS 或等效发行版
- **编译器**: GCC 9.0 或 Clang 10.0 或更高版本
- **C++标准**: C++17
- **必需库**: ALSA development package

#### macOS
- **操作系统**: macOS 10.15 Catalina 或更高版本
- **开发工具**: Xcode 12.0 或更高版本
- **C++标准**: C++17

### 4.2 依赖库版本

| 库名 | 最低版本 | 推荐版本 | 用途 |
|------|----------|----------|------|
| Qt6 | 6.2.0 | 6.5.0 | GUI框架 |
| CMake | 3.16.0 | 3.25.0 | 构建系统 |
| minimp3 | 0.1.0 | 最新版 | MP3解码 |
| libflac | 1.3.0 | 1.4.0 | FLAC解码 |
| libogg | 1.3.0 | 1.3.5 | OGG容器 |

### 4.3 编译选项规范

```cmake
# 最低C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译器特定选项
if(MSVC)
    add_compile_options(/W4 /WX)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Werror)
    add_compile_options(-fno-exceptions)
endif()
```

## 五、命名规范

### 5.1 类命名

```cpp
// 核心类：PascalCase
class AudioEngine;
class UnifiedMusicPlayer;
class FeatureManager;

// 策略类：{Feature}Strategy
class LegacyStrategy;
class RealtimeStrategy;

// 工厂类：{Type}Factory or {Type}Manager
class DecoderManager;
class AudioOutputFactory;
```

### 5.2 函数和方法命名

```cpp
// 公共API：camelCase
class AudioEngine {
public:
    VoidResult initialize(const AudioConfig& config);
    Result<std::unique_ptr<AudioBuffer>> decodeFile(const std::string& filename);

    // 布尔返回值使用is/has/should前缀
    bool isInitialized() const;
    bool hasFeature(Feature feature) const;
};

// 私有方法：camelCase，以下划线开头用于复杂名称
private:
    void processAudioWithSIMD();
    size_t calculateOptimalBufferSize_();
};
```

### 5.3 常量和枚举命名

```cpp
// 常量：kPascalCase 或 UPPER_CASE
constexpr size_t kDefaultBufferSize = 4096;
constexpr uint32_t AUDIO_BUFFER_MAX_SIZE = 1 << 20;

// 枚举：PascalCase
enum class PlayerMode {
    Legacy,
    Complete,
    Realtime,
    Production,
    MultiFormat
};

// 枚举值：PascalCase
enum class ErrorCategory {
    AudioInitialization = 1000,
    AudioDeviceNotFound = 1001,
    // ...
};
```

### 5.4 文件命名

```cpp
// 头文件：snake_case.h
audio_engine.h
sample_rate_converter.h

// 源文件：snake_case.cpp
audio_engine.cpp
sample_rate_converter.cpp

// 测试文件：{module}_test.cpp
audio_engine_test.cpp
sample_rate_converter_test.cpp
```

## 六、版本控制规范

### 6.1 版本号格式

采用语义化版本控制 (Semantic Versioning)：
```
MAJOR.MINOR.PATCH[-PRERELEASE][+BUILD]

示例：
1.0.0        - 正式发布版
1.0.0-beta.1 - 测试版
1.0.1+build.123 - 开发版
```

### 6.2 当前版本信息

- **项目版本**: 1.0.0-alpha
- **API版本**: 1.0
- **兼容性**: 向后兼容于1.0.x系列

### 6.3 分支管理

- **main**: 稳定发布版本
- **develop**: 开发版本
- **feature/***: 功能开发分支
- **hotfix/***: 紧急修复分支

## 七、错误处理规范

### 7.1 错误分类标准

| 类别 | 范围 | 严重级别 |
|------|------|----------|
| Audio (1000-1999) | 音频相关错误 | Error/Warning |
| File (2000-2999) | 文件I/O错误 | Error/Warning |
| Plugin (3000-3999) | 插件系统错误 | Error |
| Config (4000-4999) | 配置错误 | Error |
| Memory (5000-5999) | 内存错误 | Critical |
| Thread (6000-6999) | 线程错误 | Error |
| General (9000-9999) | 通用错误 | Error |

### 7.2 Result类型使用规范

```cpp
// 返回值的成功情况
Result<int> getCount();           // 可能为空
Result<std::string> getName();    // 可能为空

// Void返回值
VoidResult initialize();          // 成功/失败
VoidResult shutdown();             // 成功/失败

// 错误处理模式
auto result = audioEngine.initialize();
if (result.isError()) {
    logError(result.getError());
    return result;
}
```

## 八、文档引用规范

### 8.1 跨文档引用

当需要在其他文档中引用定义时，使用以下格式：

```markdown
播放器模式定义，参见 [REQUIREMENTS/TERMINOLOGY_AND_STANDARDS.md](REQUIREMENTS/TERMINOLOGY_AND_STANDARDS.md#12-播放器模式定义)

API参考，参见 [DESIGN/API_DOCUMENTATION.md](DESIGN/API_DOCUMENTATION.md)
```

### 8.2 避免重复

- **禁止**: 在多个文档中重复详细定义
- **推荐**: 简要说明并引用权威文档
- **示例**:
  - 不推荐：完整复制播放器模式列表
  - 推荐："支持5种播放器模式，详见术语表"

## 九、更新维护

### 9.1 文档更新流程

1. 术语定义修改 → 更新本文档
2. 相关文档同步更新引用
3. 提交PR时一并更新
4. 定期审查（每季度）

### 9.2 版本控制

- 本文档版本：v1.0.0
- 最后更新：2024年12月17日
- 维护者：项目架构团队

---

**重要提醒**：本文档是所有其他文档的权威参考源，任何冲突以本文档为准。其他文档应通过引用而非复制来使用这些定义。