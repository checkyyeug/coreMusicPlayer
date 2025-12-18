# coreMusicPlayer - Code Documentation

## Project Structure

```
coreMusicPlayer/
├── CMakeLists.txt          # CMake构建配置文件
├── README.md               # 项目说明文档
├── REQUIREMENTS/           # 需求和任务清单
│   └── TASK.md             # 开发任务清单
├── include/                # 头文件目录
│   ├── core/               # 核心模块头文件
│   ├── audio/              # 音频处理模块头文件
│   ├── utils/              # 工具类头文件
│   └── platform/         # 平台相关头文件
├── src/                    # 源代码目录
│   ├── core/               # 核心模块源文件
│   ├── audio/              # 音频处理模块源文件
│   ├── utils/              # 工具类源文件
│   ├── foobar/             # Foobar2000集成模块源文件
│   └── platform/         # 平台相关源文件
├── config/                 # 配置文件目录
│   └── default.json        # 默认配置文件
└── tests/                  # 测试代码目录
```

## Core Module Documentation

### UnifiedMusicPlayer
The main player class that manages different playback strategies.

#### Key Methods:
- `initialize()` - Initialize the player
- `set_strategy(const std::string& strategy_name)` - Switch to a specific playback strategy
- `play()`, `stop()`, `pause()` - Basic playback controls
- `set_volume(float volume)` - Set audio volume
- `get_status()` - Get current player status

### StrategyFactory
Factory class for creating different player strategies.

#### Key Methods:
- `create_strategy(const std::string& name)` - Create a strategy by name
- `initialize_default_strategies()` - Register all default strategies
- `get_registered_strategies()` - Get list of available strategies

## Audio Module Documentation

### AudioEngine
Main audio engine interface and implementation.

#### Key Methods:
- `initialize()` - Initialize the audio engine
- `play_audio(const AudioBuffer& buffer, const AudioFormat& format)` - Play audio data
- `set_volume(float volume)` - Set audio volume
- `get_status()` - Get current engine status

### DeviceManager
Manages audio devices.

#### Key Methods:
- `initialize()` - Initialize device manager
- `enumerate_devices()` - Enumerate available audio devices
- `select_output_device(const std::string& device_name)` - Select output device
- `get_devices()` - Get list of all devices

### SampleRateConverter
Handles audio sample rate conversion.

#### Key Methods:
- `convert()` - Convert audio from one sample rate to another
- `set_quality(int quality)` - Set conversion quality level

## Foobar2000 Integration Documentation

### FoobarPluginManager
Manages Foobar2000 plugins.

#### Key Methods:
- `initialize(const std::string& pluginPath)` - Initialize plugin manager
- `loadPlugin(const std::string& filename)` - Load a specific plugin
- `unloadPlugin(const std::string& filename)` - Unload a specific plugin
- `getAvailablePlugins()` - Get list of available plugins

### FoobarInputAdapter
Adapts Foobar2000 input plugins.

#### Key Methods:
- `open(const std::string& filename)` - Open audio file
- `decode(float* buffer, size_t frames)` - Decode audio data
- `getMetadata()` - Get audio metadata

### FoobarDSPAdapter
Adapts Foobar2000 DSP plugins.

#### Key Methods:
- `initialize(const AudioFormat& format)` - Initialize DSP processor
- `process(float* buffer, size_t frames)` - Process audio data
- `reset()` - Reset DSP processor state

## Utility Classes Documentation

### PerformanceCounter
Performance measurement utility.

#### Key Methods:
- `start_timer(const std::string& name)` - Start a timer
- `stop_timer(const std::string& name)` - Stop a timer
- `get_elapsed_ms(const std::string& name)` - Get elapsed time for a timer
- `print_report()` - Print performance report

### Result<T>
Template class for error handling.

#### Key Methods:
- `has_value()` - Check if result has a value
- `is_error()` - Check if result is an error
- `value()` - Get successful value
- `error()` - Get error message