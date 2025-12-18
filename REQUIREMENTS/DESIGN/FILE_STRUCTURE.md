# coreMusicPlayer 文件结构规范

## 概述

本文档定义了 coreMusicPlayer 项目的标准文件结构。所有开发者应遵循此结构组织代码。

**重要说明**：本文件结构基于统一播放器架构设计，将原有的多个独立播放器整合为一个可执行文件。

## 项目根目录结构

```
coreMusicPlayer/
├── CMakeLists.txt                 # 主构建配置
├── README.md                      # 项目说明
├── LICENSE                        # GPL-3.0许可证
├── .gitignore                     # Git忽略规则
├── .github/                       # GitHub配置
│   ├── workflows/                 # CI/CD工作流
│   ├── ISSUE_TEMPLATE/            # Issue模板
│   └── pull_request_template.md   # PR模板
│
├── src/                           # 源代码目录
│   ├── core/                      # 核心功能
│   ├── audio/                     # 音频处理
│   ├── plugins/                   # 插件系统
│   ├── platform/                  # 平台特定代码
│   ├── gui/                       # Qt GUI
│   └── utils/                     # 工具类
│
├── include/                       # 公共头文件
│   ├── core/                      # 核心接口
│   ├── audio/                     # 音频接口
│   └── plugin/                    # 插件接口
│
├── tests/                         # 测试代码
│   ├── unit/                      # 单元测试
│   ├── integration/               # 集成测试
│   └── performance/               # 性能测试
│
├── docs/                          # 文档目录
├── examples/                      # 示例代码
├── tools/                         # 开发工具
├── third_party/                   # 第三方库
└── build/                         # 构建输出
```

## 源代码组织

### 3.1 src/core/ - 核心功能

```
src/core/
├── unified_music_player.h/cpp     # 统一播放器主类
├── player_strategy.h/cpp           # 策略模式接口
├── strategies/                     # 策略实现
│   ├── legacy_strategy.h/cpp
│   ├── complete_strategy.h/cpp
│   ├── realtime_strategy.h/cpp
│   ├── production_strategy.h/cpp
│   └── multi_format_strategy.h/cpp
├── feature_manager.h/cpp          # 特性管理器
├── config_manager.h/cpp            # 配置管理器
├── event_bus.h/cpp                 # 事件总线
└── utils/                          # 核心工具
    ├── result.h/cpp               # Result类型
    └── error.h/cpp                # 错误类型
```

### 3.2 src/audio/ - 音频处理

```
src/audio/
├── audio_engine.h/cpp              # 音频引擎
├── audio_buffer.h/cpp              # 音频缓冲区
├── audio_format.h                  # 音频格式定义
├── decoder_manager.h/cpp            # 解码器管理
├── sample_rate_converter.h/cpp      # 采样率转换
├── decoders/                       # 解码器实现
│   ├── wav_decoder.h/cpp
│   ├── mp3_decoder.h/cpp
│   ├── flac_decoder.h/cpp
│   └── ogg_decoder.h/cpp
├── resamplers/                     # 重采样算法
│   ├── linear_resampler.h/cpp
│   ├── cubic_resampler.h/cpp
│   └── sinc_resampler.h/cpp
├── dsp/                            # 数字信号处理
│   ├── volume_control.h/cpp
│   ├── equalizer.h/cpp
│   └── effects.h/cpp
└── simd/                           # SIMD优化
    ├── simd_ops.h
    ├── sse2_ops.cpp
    ├── avx2_ops.cpp
    └── common.cpp
```

### 3.3 src/platform/ - 平台抽象

```
src/platform/
├── audio_output.h                  # 音频输出接口
├── windows/                        # Windows实现
│   ├── wasapi_output.h/cpp
│   ├── mmdevice_manager.h/cpp
│   └── com_utils.h/cpp
├── linux/                          # Linux实现
│   ├── alsa_output.h/cpp
│   ├── pulse_output.h/cpp
│   └── device_manager.h/cpp
├── macos/                          # macOS实现
│   ├── coreaudio_output.h/cpp
│   └── audio_device_manager.h/cpp
└── common/                         # 通用平台代码
    ├── platform_utils.h
    ├── thread_manager.h
    └── file_utils.h
```

### 3.4 src/plugins/ - 插件系统

```
src/plugins/
├── plugin_manager.h/cpp            # 插件管理器
├── plugin_interface.h               # 插件接口定义
├── foobar_adapter/                   # Foobar2000适配器
│   ├── foobar_decoder_adapter.h/cpp
│   ├── foobar_output_adapter.h/cpp
│   └── foobar_sdk_wrapper.h/cpp
└── hot_reload/                      # 热重载功能
    ├── plugin_watcher.h/cpp
    └── dynamic_loader.h/cpp
```

### 3.5 src/gui/ - Qt GUI

```
src/gui/
├── main_window.h/cpp                # 主窗口
├── widgets/                         # UI组件
│   ├── player_widget.h/cpp
│   ├── playlist_widget.h/cpp
│   ├── volume_widget.h/cpp
│   └── settings_widget.h/cpp
├── models/                          # 数据模型
│   ├── playlist_model.h/cpp
│   ├── track_model.h/cpp
│   └── settings_model.h/cpp
├── views/                           # 视图组件
│   ├── playlist_view.h/cpp
│   ├── library_view.h/cpp
│   └── equalizer_view.h/cpp
├── controllers/                     # 控制器
│   ├── player_controller.h/cpp
│   ├── playlist_controller.h/cpp
│   └── settings_controller.h/cpp
├── resources/                       # Qt资源
│   ├── icons/
│   ├── themes/
│   └── ui/
└── utils/                           # GUI工具
    ├── style_helper.h/cpp
    └── theme_manager.h/cpp
```

## 头文件组织

### 4.1 include/ 目录结构

```
include/
├── core/                           # 公共核心接口
│   ├── music_player.h
│   ├── player_mode.h
│   └── feature_manager.h
├── audio/                          # 音频接口
│   ├── audio_engine.h
│   ├── audio_buffer.h
│   └── decoder_interface.h
├── plugin/                         # 插件接口
│   ├── plugin_interface.h
│   └── foobar_interface.h
├── platform/                       # 平台接口
│   └── audio_output.h
└── core/                           # 内部头文件
    ├── result.h
    └── error.h
```

### 4.2 头文件包含规范

```cpp
// 1. 系统头文件
#include <memory>
#include <vector>
#include <thread>

// 2. 第三方库头文件
#include <QString>
#include <QWidget>

// 3. 项目内部头文件（使用相对路径）
#include "audio_buffer.h"
#include "player_strategy.h"

// 4. 前向声明（在头文件中）
class AudioDecoder;
struct AudioConfig;
```

## 测试代码组织

### 5.1 测试目录结构

```
tests/
├── CMakeLists.txt                   # 测试构建配置
├── test_main.cpp                    # 测试入口
├── unit/                           # 单元测试
│   ├── audio/
│   │   ├── audio_buffer_test.cpp
│   │   ├── decoder_test.cpp
│   │   └── resampler_test.cpp
│   ├── core/
│   │   ├── player_test.cpp
│   │   ├── feature_manager_test.cpp
│   │   └── config_manager_test.cpp
│   └── plugins/
│       ├── plugin_manager_test.cpp
│       └── foobar_adapter_test.cpp
├── integration/                   # 集成测试
│   ├── playback_test.cpp
│   ├── plugin_test.cpp
│   └── cross_platform_test.cpp
├── performance/                   # 性能测试
│   ├── latency_test.cpp
│   ├── cpu_usage_test.cpp
│   └── memory_test.cpp
├── fixtures/                      # 测试数据
│   ├── audio/
│   │   ├── test.wav
│   │   ├── test.mp3
│   │   └── test.flac
│   └── configs/
└── utils/                         # 测试工具
    ├── test_helper.h/cpp
    ├── mock_objects.h/cpp
    └── performance_timer.h
```

## 构建系统组织

### 6.1 CMake 模块结构

```
cmake/
├── compiler.cmake                 # 编译器配置
├── options.cmake                  # 构建选项
├── dependencies.cmake              # 依赖查找
├── platform/                      # 平台配置
│   ├── windows.cmake
│   ├── linux.cmake
│   └── macos.cmake
└── modules/                        # CMake模块
    ├── FindQt6.cmake
    ├── FindALSA.cmake
    └── CompilerFlags.cmake
```

### 6.2 CMakeLists.txt 层次结构

```cmake
# 根目录 CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(coreMusicPlayer VERSION 1.0.0 LANGUAGES CXX)

include(cmake/options.cmake)
include(cmake/compiler.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(src)
add_subdirectory(tests)
```

## 文档组织

```
docs/
├── api/                           # API文档
│   ├── core/
│   ├── audio/
│   └── plugin/
├── design/                        # 设计文档
├── user/                          # 用户文档
├── developer/                     # 开发者文档
└── images/                        # 文档图片
    ├── architecture/
    ├── screenshots/
    └── diagrams/
```

## 与旧结构的主要变化

### 8.1 从多文件到统一架构

**旧结构问题**：
- 5个独立的可执行文件
- 重复的音频处理代码
- 不一致的GUI实现
- 多种构建配置

**新结构优势**：
- 统一的可执行文件
- 策略模式实现5种模式
- 共享的核心组件
- 清晰的模块分离

### 8.2 目录重组

| 旧位置 | 新位置 | 说明 |
|--------|--------|------|
| `src/audio/` → `src/audio/` | 保留并重组 | 统一音频处理模块 |
| `qt_gui/` → `src/gui/` | 移动并重命名 | 标准化GUI目录 |
| `src/core/` → `src/core/` | 新建 | 核心功能模块 |
| `src/platform/` → `src/platform/` | 保留 | 平台抽象层 |

## 迁移指南

### 9.1 代码迁移

```bash
# 1. 创建新的目录结构
mkdir -p src/{core,audio,plugins,platform,gui}
mkdir -p include/{core,audio,plugin,platform}

# 2. 迁移现有文件
mv src/audio/decoders/* src/audio/decoders/
mv src/audio/simd_optimizations.* src/audio/simd/

# 3. 合并播放器代码
# 将各个player_xxx.cpp整合为strategy实现
```

### 9.2 构建更新

```cmake
# 更新主CMakeLists.txt
add_executable(coreMusicPlayer
    src/main.cpp
    src/core/*.cpp
    src/audio/*.cpp
    src/plugins/*.cpp
    src/platform/${PLATFORM}/*.cpp
)

# 设置输出目录
set_target_properties(coreMusicPlayer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
```

## 更新说明

本文件最后更新：2024年12月17日

更新历史：
- v2.0.0: 重组为统一播放器架构
- v1.0.0: 基于旧结构的文档

---

**参考**：更多详细说明请参见 [TERMINOLOGY_AND_STANDARDS.md](REQUIREMENTS/TERMINOLOGY_AND_STANDARDS.md)

## 2. Core Source Files Details

### 2.1 Audio Processing (`src/audio/`)

#### sample_rate_converter.cpp/h
```cpp
class SampleRateConverter {
public:
    enum Quality { Fast, Good, Best, High };

    bool resample(const AudioBuffer& input, AudioBuffer& output,
                  double inputRate, double outputRate,
                  Quality quality = Quality::Good);

private:
    // Implementation uses:
    // - Linear interpolation (Fast)
    // - Cubic spline (Good)
    // - Kaiser-windowed sinc (Best/High)
    // - SIMD optimizations for x86/x64
};
```

#### simd_optimizations.cpp/h
```cpp
// SIMD-optimized audio processing functions
namespace simd {
    void adjust_volume_avx(float* data, size_t frames, float volume);
    void adjust_volume_sse2(float* data, size_t frames, float volume);
    void resample_cubic_sse2(const float* input, float* output, ...);
    void convert_format_simd(const void* input, void* output, ...);
}
```

### 2.2 Plugin Integration

#### foobar_plugin_manager.cpp/h
```cpp
class PluginManager {
public:
    bool loadPlugin(const std::string& pluginPath);
    void unloadPlugin(const std::string& pluginName);
    std::vector<PluginInfo> getLoadedPlugins();

    // Service registry integration
    template<typename T>
    T* getService(const char* name);
};
```

### 2.3 Audio Players

#### simple_wav_player.cpp/h
- Basic WAV file player
- Platform-specific audio output
- Simple command-line interface

#### test_wasapi.cpp
- WASAPI testing and diagnostics
- Audio device enumeration
- Latency measurement

## 3. GUI Implementation Details

### 3.1 Qt6 GUI (`qt_gui/`)

#### mainwindow.cpp/h
```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    PlayerWidget* m_playerWidget;
    QListView* m_playlistView;
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* m_playlist;

    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void connectSignals();
};
```

#### playerwidget.cpp/h
```cpp
class PlayerWidget : public QWidget {
    Q_OBJECT
signals:
    void playPauseRequested();
    void stopRequested();
    void nextTrackRequested();
    void previousTrackRequested();
    void volumeChanged(int volume);

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onPositionSliderMoved(int position);
};
```

### 3.2 Build Scripts

#### build.bat
```batch
@echo off
# Main build script with options:
#   build.bat         - Build
#   build.bat clean   - Clean
#   build.bat deploy  - Deploy DLLs
```

## 4. Configuration System (`config/`)

#### config_manager.cpp/h
```cpp
class ConfigManager {
public:
    template<typename T>
    T get(const std::string& key, const T& defaultValue = T{});

    template<typename T>
    void set(const std::string& key, const T& value);

    void load();
    void save();

private:
    nlohmann::json config_;
    std::string configFile_;
};
```

## 5. Executable Files

### 5.1 Console Applications

| Executable | Purpose | Dependencies |
|------------|---------|--------------|
| `final_wav_player.cpp` | Main WAV player | WASAPI |
| `music_player_complete.cpp` | Full-featured player | Qt6 (optional) |
| `music_player_legacy.cpp` | Legacy player | WASAPI |
| `music_player_simple_direct.cpp` | Direct WASAPI player | WASAPI |
| `simple_player.cpp` | Simple CLI player | Basic I/O |
| `multi_format_player.cpp` | Multi-format support | Various decoders |

### 5.2 Testing and Utilities

| Executable | Purpose |
|------------|---------|
| `test_simple_playback.cpp` | Playback testing |
| `simple_performance_test.cpp` | Performance benchmarks |
| `optimization_integration_example.cpp` | SIMD optimization demo |
| `check_config.cpp` | Configuration testing |
| `test_plugin_config.cpp` | Plugin configuration test |

## 6. Build Directories

### 6.1 Generated Build Directories
```
build/            # Original build (88MB)
build_clean/       # Clean build attempt
build_final/       # Final working build (5.4MB)
build_fixed/       # Fixed version build (574KB)
build_working/     # Working components build
```

### 6.2 Build Output Structure
```
build_final/
├── Release/
│   ├── final_wav_player.exe
│   ├── music_player_complete.exe
│   ├── music_player_legacy.exe
│   ├── music_player_simple_direct.exe
│   ├── optimization_integration_example.exe
│   ├── simple_performance_test.exe
│   └── test_simple_playback.exe
└── CMakeFiles/    # CMake generated files
```

## 7. Third-Party Dependencies

### 7.1 nlohmann/json (`third_party/nlohmann-json/`)
- Header-only JSON library
- Used for configuration file format
- Version: 3.11.2 (included)

### 7.2 Foobar2000 SDK
- External SDK (not included)
- Path: `foobar2000_sdk/`
- Used for plugin compatibility

## 8. Platform-Specific Files

### 8.1 Windows
- WASAPI implementation files
- Windows-specific build scripts
- Visual Studio project files

### 8.2 Linux
- ALSA implementation files
- Platform abstraction stubs
- Linux-specific makefiles

## 9. Documentation Files

### 9.1 Root Documentation
```
README.md                    # Project overview
WARNING_FIXES.md            # Build warning fixes
BUILD_GUIDE.md              # Build instructions (now in docs/)
```

### 9.2 Moved Documentation (`docs/`)
```
docs/
├── DEPENDENCIES.md          # External dependencies
├── ENGINEERING_SUMMARY.md   # Project summary
├── FOOBAR2000_COMPATIBILITY_ANALYSIS.md
├── MUSIC_PLAYER_SUCCESS.md
├── PERFORMANCE_OPTIMIZATION_SUMMARY.md
└── README.md
```

## 10. Configuration Files

### 10.1 CMake Configuration
```
CMakeLists.txt               # Main CMake file (simplified)
CMakeLists_simplified.txt    # Backup simplified version
qt_gui/CMakeLists.txt        # Qt GUI CMake
gui/CMakeLists.txt           # Alternative GUI CMake
```

### 10.2 Build Scripts
```
build_final.bat              # Main build script
build_windows.bat            # Windows-specific script
build_gui_all.bat            # GUI build script
cleanup_build_dirs.bat       # Cleanup utility
```

## 11. Test Assets

### 11.1 Audio Files
```
test_audio.wav               # Basic test audio
test_1khz.wav               # 1kHz test tone
test_stereo.wav              # Stereo test file
simple_test.wav             # Simple test
```

### 11.2 Generated Test Audio (`src/audio/`)
Various test files at different sample rates and formats for testing resampling and format conversion.

## 12. Implementation Checklist

For recreating this project from scratch:

1. **Set up build environment**
   - Install CMake 3.10+
   - Install Visual Studio 2019+ (Windows) or GCC 9+ (Linux)
   - Install Qt6 (for GUI)

2. **Create directory structure**
   ```
   mkdir -p src/{audio,core,platform,compat}
   mkdir -p {qt_gui,gui,config}
   mkdir -p docs third_party
   ```

3. **Core files to create**
   - `CMakeLists.txt` (use simplified version)
   - Audio processing files in `src/audio/`
   - GUI files in `qt_gui/`
   - Configuration system

4. **Build and test**
   - Run `build_final.bat` (Windows) or make (Linux)
   - Test with provided audio files
   - Verify all executables work

This file structure provides a complete blueprint for recreating the entire coreMusicPlayer project with all its features and components.