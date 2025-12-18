# coreMusicPlayer API Reference

## Table of Contents
1. [Overview](#1-overview)
2. [Core Types](#2-core-types)
3. [Player API](#3-player-api)
4. [Audio Engine API](#4-audio-engine-api)
5. [Plugin System API](#5-plugin-system-api)
6. [Configuration API](#6-configuration-api)
7. [Error Handling API](#7-error-handling-api)
8. [Examples](#8-examples)

## 1. Overview

coreMusicPlayer provides a modern C++17 API that uses type-safe error handling through `Result<T>` types instead of exceptions. The API is designed to be thread-safe where appropriate and provides both high-level convenience APIs and low-level control APIs.

### Key Principles
- **Result-based Error Handling**: All functions return `Result<T>` instead of throwing exceptions
- **Thread Safety**: All public APIs are thread-safe with documented threading requirements
- **RAII**: Resource management through RAII patterns
- **Modern C++**: Uses C++17 features including `std::optional`, `std::variant`, and structured bindings

### Headers Structure
```cpp
// Core headers
#include "core/music_player.h"
#include "core/config_manager.h"
#include "core/feature_manager.h"

// Audio processing
#include "audio/audio_buffer.h"
#include "audio/audio_format.h"
#include "audio/sample_rate_converter.h"

// Plugin system
#include "plugin/plugin_manager.h"
#include "plugin/plugin_interface.h"

// Platform abstraction
#include "platform/audio_output.h"
```

## 2. Core Types

### 2.1 Result Type

All API functions return `Result<T>` for error handling:

```cpp
namespace core {

template<typename T>
class Result {
public:
    static Result<T> success(T value);
    static Result<T> error(Error err);

    bool isSuccess() const noexcept;
    bool isError() const noexcept;

    const T& getValue() const;  // Throws if error
    T takeValue();              // Throws if error
    const Error& getError() const;  // Throws if success

    // Monadic operations
    template<typename F> auto map(F&& func) -> Result<decltype(func(getValue()))>;
    template<typename F> auto flatMap(F&& func) -> decltype(func(getValue()));
    T valueOr(T&& defaultValue) const;
};

// Specialization for void
template<>
class Result<void> {
public:
    static Result<void> success();
    static Result<void> error(Error err);

    bool isSuccess() const noexcept;
    bool isError() const noexcept;
    void getValue() const;  // Throws if error
    const Error& getError() const;
};

using VoidResult = Result<void>;

} // namespace core
```

### 2.2 Audio Format

```cpp
namespace audio {

struct AudioFormat {
    uint32_t sampleRate;      // 8000 - 192000
    uint16_t channels;        // 1 - 8
    uint16_t bitsPerSample;   // 16, 24, or 32
    bool isFloat;            // true for 32-bit float

    static AudioFormat getDefault();
    static AudioFormat cdQuality();    // 44100, 2, 16, false
    static AudioFormat highRes();      // 96000, 2, 24, false

    bool isValid() const;
    size_t getFrameSize() const;
    size_t getBytesPerSecond() const;
};

} // namespace audio
```

### 2.3 Audio Buffer

```cpp
namespace audio {

class AudioBuffer {
public:
    AudioBuffer(size_t channels, size_t frames);
    AudioBuffer(const AudioFormat& format, size_t frames);

    // Properties
    size_t getChannels() const noexcept;
    size_t getFrames() const noexcept;
    size_t getSize() const noexcept;  // Total samples
    const AudioFormat& getFormat() const noexcept;

    // Data access
    float* getData() noexcept;
    const float* getData() const noexcept;
    float* getChannelData(size_t channel) noexcept;
    const float* getChannelData(size_t channel) const noexcept;

    // Operations
    VoidResult resize(size_t frames);
    void clear();
    void applyGain(float gain);
    VoidResult mix(const AudioBuffer& other, float gain = 1.0f);

    // Conversions
    VoidResult toPlanar();
    VoidResult toInterleaved();

    // Move and copy semantics
    AudioBuffer(const AudioBuffer& other);
    AudioBuffer(AudioBuffer&& other) noexcept;
    AudioBuffer& operator=(const AudioBuffer& other);
    AudioBuffer& operator=(AudioBuffer&& other) noexcept;
};

} // namespace audio
```

## 3. Player API

### 3.1 UnifiedMusicPlayer

The main player class that supports multiple operational modes:

```cpp
namespace core {

class UnifiedMusicPlayer {
public:
    enum class PlayerMode {
        Legacy,      // Basic WAV playback only
        Complete,    // All features enabled
        Realtime,    // Low-latency optimized
        Production,  // Maximum stability
        MultiFormat  // Default mode, balanced
    };

    explicit UnifiedMusicPlayer(PlayerMode mode = PlayerMode::MultiFormat);
    ~UnifiedMusicPlayer();

    // Lifecycle
    VoidResult initialize(const audio::AudioFormat& format = audio::AudioFormat::getDefault());
    VoidResult shutdown();

    // File operations
    VoidResult loadFile(const std::string& filename);
    VoidResult closeFile();
    bool isFileLoaded() const;
    std::string getCurrentTrack() const;

    // Playback control
    VoidResult play();
    VoidResult pause();
    VoidResult stop();
    VoidResult seek(double position);  // Position in seconds
    VoidResult seekPercent(double percent);  // 0.0 - 1.0

    // Volume control
    VoidResult setVolume(double volume);  // 0.0 - 1.0
    double getVolume() const;

    // Position information
    double getPosition() const;  // Current position in seconds
    double getDuration() const;  // Total duration in seconds
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

    // Mode management
    VoidResult setMode(PlayerMode mode);
    PlayerMode getCurrentMode() const;

    // Feature management
    VoidResult enableFeature(FeatureManager::Feature feature);
    VoidResult disableFeature(FeatureManager::Feature feature);
    bool isFeatureEnabled(FeatureManager::Feature feature) const;

    // Callbacks
    using StateChangeCallback = std::function<void(PlayerState)>;
    using PositionCallback = std::function<void(double)>;
    using TrackEndCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const Error&)>;

    void setStateChangeCallback(StateChangeCallback callback);
    void setPositionCallback(PositionCallback callback);
    void setTrackEndCallback(TrackEndCallback callback);
    void setErrorCallback(ErrorCallback callback);

    // Statistics
    struct Statistics {
        uint64_t tracksPlayed;
        uint64_t totalPlayTime;  // seconds
        double averageLatency;
        uint32_t bufferUnderruns;
        uint32_t errors;
    };

    Statistics getStatistics() const;
    VoidResult resetStatistics();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace core
```

### 3.2 Legacy Compatibility Wrappers

For backward compatibility, wrapper executables can be created:

```cpp
namespace core::legacy {

// Wrapper for music_player_legacy executable
class LegacyPlayer {
public:
    LegacyPlayer() : player_(UnifiedMusicPlayer::PlayerMode::Legacy) {}

    // Legacy API using bool returns
    bool initialize(int sampleRate = 44100, int channels = 2);
    bool loadFile(const char* filename);
    bool play();
    bool pause();
    bool stop();
    void setVolume(float volume);  // 0.0 - 1.0

    // Legacy state
    enum State { STOPPED, PLAYING, PAUSED };
    State getState() const;
    float getPosition() const;  // seconds

private:
    UnifiedMusicPlayer player_;
};

} // namespace core::legacy
```

## 4. Audio Engine API

### 4.1 AudioEngine

Low-level audio processing engine:

```cpp
namespace audio {

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();

    // Initialization
    VoidResult initialize(const AudioConfig& config);
    VoidResult shutdown();

    // Device management
    struct DeviceInfo {
        std::string id;
        std::string name;
        uint32_t sampleRates;
        uint16_t minChannels;
        uint16_t maxChannels;
    };

    Result<std::vector<DeviceInfo>> enumerateDevices() const;
    VoidResult setDevice(const std::string& deviceId);
    std::string getCurrentDevice() const;

    // Format management
    VoidResult setFormat(const AudioFormat& format);
    AudioFormat getCurrentFormat() const;
    bool isFormatSupported(const AudioFormat& format) const;

    // Stream management
    VoidResult start();
    VoidResult stop();
    VoidResult pause();
    bool isRunning() const;

    // Audio I/O (internal use)
    void audioCallback(float* output, size_t frames) noexcept;

    // Buffer management
    struct BufferConfig {
        size_t bufferSize;        // Frames per buffer
        size_t bufferCount;       // Number of buffers
        bool enableSimd;         // SIMD optimization
        bool lockMemory;         // Lock pages in RAM
    };

    VoidResult configureBuffers(const BufferConfig& config);

    // Performance monitoring
    struct PerformanceMetrics {
        double cpuUsage;          // Percentage
        size_t memoryUsage;       // Bytes
        double averageLatency;    // Milliseconds
        uint32_t xruns;           // Underruns/overruns
    };

    PerformanceMetrics getPerformanceMetrics() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace audio
```

### 4.2 Decoder Manager

```cpp
namespace audio {

class DecoderManager {
public:
    DecoderManager();
    ~DecoderManager();

    // Decoder management
    struct DecoderInfo {
        std::string name;
        std::string version;
        std::vector<std::string> supportedExtensions;
        std::vector<std::string> supportedMimeTypes;
    };

    VoidResult registerDecoder(const std::string& format,
                              std::unique_ptr<AudioDecoder> decoder);
    VoidResult unregisterDecoder(const std::string& format);

    Result<std::vector<DecoderInfo>> getRegisteredDecoders() const;

    // Decoding
    Result<AudioFormat> detectFormat(const std::string& filename);
    Result<std::unique_ptr<AudioBuffer>> decodeFile(const std::string& filename);

    // Lazy loading
    VoidResult preloadDecoder(const std::string& format);
    VoidResult unloadDecoder(const std::string& format);
    VoidResult unloadUnusedDecoders(std::chrono::minutes idleThreshold = std::chrono::minutes(5));

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace audio
```

### 4.3 Sample Rate Converter

```cpp
namespace audio {

class SampleRateConverter {
public:
    enum class Quality {
        Fast,       // Linear interpolation
        Good,       // Cubic interpolation
        Best,       // High-quality sinc
        High        // Ultra-high quality sinc
    };

    SampleRateConverter();
    ~SampleRateConverter();

    // Conversion
    Result<AudioBuffer> convert(const AudioBuffer& input,
                                double inputRate,
                                double outputRate,
                                Quality quality = Quality::Good);

    // Convenience overloads
    Result<AudioBuffer> convert(const AudioBuffer& input,
                                int outputRate,
                                Quality quality = Quality::Good);

    // Configuration
    VoidResult setQuality(Quality quality);
    Quality getQuality() const;

    // SIMD optimization
    VoidResult enableSimd(bool enable);
    bool isSimdEnabled() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace audio
```

## 5. Plugin System API

### 5.1 Plugin Manager

```cpp
namespace plugin {

class PluginManager {
public:
    PluginManager();
    ~PluginManager();

    // Plugin lifecycle
    enum class PluginType {
        Decoder,
        DSP,
        Output,
        Visualization
    };

    struct PluginInfo {
        std::string id;
        std::string name;
        std::string version;
        std::string author;
        std::string description;
        PluginType type;
    };

    VoidResult loadPlugin(const std::string& path);
    VoidResult unloadPlugin(const std::string& pluginId);
    VoidResult reloadPlugin(const std::string& pluginId);

    Result<std::vector<PluginInfo>> getLoadedPlugins() const;
    Result<PluginInfo> getPluginInfo(const std::string& pluginId) const;

    // Plugin interaction
    template<typename T>
    Result<T*> getPlugin(const std::string& pluginId) const;

    // Foobar2000 compatibility
    VoidResult scanFoobarPlugins(const std::string& directory);
    Result<std::vector<PluginInfo>> getFoobarPlugins() const;
    Result<std::shared_ptr<core::AudioDecoder>> createFoobarDecoder(const std::string& pluginName);
    Result<std::shared_ptr<core::AudioProcessor>> createFoobarDSP(const std::string& pluginName);
    Result<std::shared_ptr<core::AudioOutput>> createFoobarOutput(const std::string& pluginName);

    // Hot reload
    VoidResult enableHotReload(bool enable);
    bool isHotReloadEnabled() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace plugin
```

### 5.2 Plugin Interface

```cpp
namespace plugin {

// Base plugin interface
class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual PluginInfo getInfo() const = 0;
    virtual VoidResult initialize() = 0;
    virtual void cleanup() = 0;

    // Plugin capabilities
    virtual bool canRealtime() const { return false; }
    virtual bool wantsAudio() const { return false; }
};

// Decoder plugin interface
class IDecoderPlugin : public IPlugin {
public:
    virtual bool canDecode(const std::string& filename) = 0;
    virtual Result<audio::AudioFormat> getFormat(const std::string& filename) = 0;
    virtual Result<std::unique_ptr<audio::AudioBuffer>> decode(const std::string& filename) = 0;
};

// DSP plugin interface
class IDSPPlugin : public IPlugin {
public:
    struct DSPContext {
        const float* input;
        float* output;
        size_t frames;
        audio::AudioFormat format;
    };

    virtual VoidResult process(DSPContext& context) = 0;
    virtual bool canProcessInPlace() const { return true; }
};

// Plugin factory function
extern "C" {
    IPlugin* createPlugin();
    void destroyPlugin(IPlugin* plugin);
}

} // namespace plugin
```

## 6. Configuration API

### 6.1 Config Manager

```cpp
namespace core {

class ConfigManager {
public:
    explicit ConfigManager(const std::string& configFile = "config.json");
    ~ConfigManager();

    // Configuration access
    template<typename T>
    Result<T> get(const std::string& key, const T& defaultValue = T{}) const;

    template<typename T>
    VoidResult set(const std::string& key, const T& value);

    bool hasKey(const std::string& key) const;
    VoidResult removeKey(const std::string& key);

    // File operations
    VoidResult load();
    VoidResult save();
    VoidResult reload();

    // Schema validation
    VoidResult setSchema(const std::string& schemaFile);
    Result<bool> validate() const;

    // Notifications
    using ChangeCallback = std::function<void(const std::string& key)>;
    void setChangeCallback(ChangeCallback callback);

    // Sections
    VoidResult createSection(const std::string& section);
    Result<std::vector<std::string>> getSections() const;

    // Import/Export
    VoidResult import(const std::string& filename);
    VoidResult export(const std::string& filename) const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace core
```

### 6.2 Feature Manager

```cpp
namespace core {

class FeatureManager {
public:
    enum class Feature : uint32_t {
        // Audio features
        BasicPlayback = 1 << 0,
        VolumeControl = 1 << 1,
        SampleRateConversion = 1 << 2,
        SIMDOptimization = 1 << 3,
        AdvancedDSP = 1 << 4,

        // Format support
        WAVSupport = 1 << 10,
        MP3Support = 1 << 11,
        FLACSupport = 1 << 12,
        OGGSupport = 1 << 13,
        MultiFormat = 1 << 14,

        // System features
        PluginSupport = 1 << 20,
        RealtimeMode = 1 << 21,
        LowLatency = 1 << 22,
        ErrorRecovery = 1 << 23,
        AudioVisualization = 1 << 24,

        // Advanced features
        AdvancedBuffering = 1 << 30,
        HotPluginReload = 1 << 31,
        NetworkStreaming = 1 << 32
    };

    FeatureManager();
    ~FeatureManager();

    // Feature control
    VoidResult enableFeature(Feature feature);
    VoidResult disableFeature(Feature feature);
    bool isEnabled(Feature feature) const;

    // Feature sets
    using FeatureSet = uint64_t;

    VoidResult enableFeatures(FeatureSet features);
    VoidResult disableFeatures(FeatureSet features);
    FeatureSet getEnabledFeatures() const;

    // Dependencies and conflicts
    Result<std::vector<Feature>> getDependencies(Feature feature) const;
    Result<std::vector<Feature>> getConflicts(Feature feature) const;

    // Validation
    Result<bool> validateConfiguration() const;
    Result<std::string> getValidationError() const;

    // Presets
    VoidResult applyPreset(const std::string& presetName);
    Result<std::vector<std::string>> getAvailablePresets() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace core
```

## 7. Error Handling API

### 7.1 Error Types

```cpp
namespace core {

enum class ErrorCategory : int {
    // Audio errors (1000-1999)
    AudioInitialization = 1000,
    AudioDeviceNotFound = 1001,
    AudioFormatUnsupported = 1002,
    AudioBufferUnderrun = 1003,
    AudioBufferOverrun = 1004,

    // File errors (2000-2999)
    FileNotFound = 2000,
    FileAccessDenied = 2001,
    FileCorrupted = 2002,
    UnsupportedFormat = 2003,

    // Plugin errors (3000-3999)
    PluginLoadFailed = 3000,
    PluginInitializationFailed = 3001,
    PluginRuntimeError = 3002,
    PluginSecurityViolation = 3003,

    // Configuration errors (4000-4999)
    ConfigParseError = 4000,
    ConfigValidationFailed = 4001,
    ConfigWriteFailed = 4002,

    // Memory errors (5000-5999)
    OutOfMemory = 5000,
    MemoryLeak = 5001,
    InvalidMemoryAccess = 5002,

    // Thread errors (6000-6999)
    ThreadCreationFailed = 6000,
    ThreadDeadlock = 6001,
    ThreadTimeout = 6002,

    // General errors (9000-9999)
    Unknown = 9000,
    NotImplemented = 9001,
    InvalidArgument = 9002,
    OperationFailed = 9003
};

enum class ErrorSeverity {
    Debug,      // Debug information
    Info,       // Informational
    Warning,    // Recoverable error
    Error,      // Standard error
    Critical    // Unrecoverable error
};

class Error {
public:
    Error(ErrorCategory category, std::string message,
          ErrorSeverity severity = ErrorSeverity::Error);

    // Accessors
    ErrorCategory getCategory() const noexcept;
    const std::string& getMessage() const noexcept;
    ErrorSeverity getSeverity() const noexcept;

    // Context
    template<typename T>
    Error& addContext(const std::string& key, const T& value);
    std::string getContext(const std::string& key) const;

    // Conversion
    std::string toString() const;

    // Factory methods
    static Error fromCurrentException();
    static Error fromSystemError(int errorCode);

    bool isRecoverable() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace core
```

### 7.2 Error Handlers

```cpp
namespace core {

using ErrorHandler = std::function<bool(const Error&)>;

class ErrorHandlerRegistry {
public:
    static ErrorHandlerRegistry& instance();

    void registerHandler(ErrorCategory category, ErrorHandler handler);
    void unregisterHandler(ErrorCategory category);

    bool handleError(const Error& error);

    // Built-in handlers
    static bool logErrorHandler(const Error& error);
    static bool popupErrorHandler(const Error& error);
    static bool silentErrorHandler(const Error& error);
};

} // namespace core
```

## 8. Examples

### 8.1 Basic Playback

```cpp
#include "core/music_player.h"
#include <iostream>

int main() {
    // Create player with default mode
    core::UnifiedMusicPlayer player;

    // Initialize
    auto result = player.initialize();
    if (result.isError()) {
        std::cerr << "Failed to initialize: " << result.getError().getMessage() << std::endl;
        return 1;
    }

    // Load file
    result = player.loadFile("music.mp3");
    if (result.isError()) {
        std::cerr << "Failed to load file: " << result.getError().getMessage() << std::endl;
        return 1;
    }

    // Play
    result = player.play();
    if (result.isError()) {
        std::cerr << "Failed to play: " << result.getError().getMessage() << std::endl;
        return 1;
    }

    // Wait for completion
    player.setTrackEndCallback([]() {
        std::cout << "Playback finished" << std::endl;
    });

    return 0;
}
```

### 8.2 Error Handling

```cpp
void handlePlaybackError(core::UnifiedMusicPlayer& player) {
    player.setErrorCallback([](const core::Error& error) {
        if (error.isRecoverable()) {
            std::cout << "Recoverable error: " << error.getMessage() << std::endl;
            // Attempt recovery
            return true;
        } else {
            std::cerr << "Critical error: " << error.getMessage() << std::endl;
            return false;
        }
    });
}
```

### 8.3 Plugin Usage

```cpp
#include "plugin/plugin_manager.h"

int main() {
    plugin::PluginManager pluginManager;

    // Scan for plugins
    auto result = pluginManager.scanFoobarPlugins("./plugins");
    if (result.isError()) {
        std::cerr << "Failed to scan plugins: " << result.getError().getMessage() << std::endl;
    }

    // Get list of plugins
    auto plugins = pluginManager.getLoadedPlugins();
    if (plugins.isError()) {
        std::cerr << "Failed to get plugins: " << plugins.getError().getMessage() << std::endl;
    }

    for (const auto& plugin : plugins.getValue()) {
        std::cout << "Plugin: " << plugin.name << " v" << plugin.version << std::endl;
    }

    return 0;
}
```

### 8.4 Configuration Management

```cpp
#include "core/config_manager.h"

int main() {
    core::ConfigManager config("app_config.json");

    // Load configuration
    auto result = config.load();
    if (result.isError()) {
        std::cerr << "Failed to load config: " << result.getError().getMessage() << std::endl;
    }

    // Read settings
    auto sampleRate = config.get<int>("audio.sample_rate", 44100);
    auto volume = config.get<double>("audio.volume", 1.0);

    // Update settings
    config.set("audio.volume", 0.8);
    config.set("audio.enable_dsp", true);

    // Save changes
    result = config.save();
    if (result.isError()) {
        std::cerr << "Failed to save config: " << result.getError().getMessage() << std::endl;
    }

    return 0;
}
```

### 8.5 Custom Feature Management

```cpp
#include "core/feature_manager.h"

int main() {
    core::FeatureManager features;

    // Enable specific features
    features.enableFeature(core::FeatureManager::Feature::BasicPlayback);
    features.enableFeature(core::FeatureManager::Feature::MP3Support);
    features.enableFeature(core::FeatureManager::Feature::SIMDOptimization);

    // Check dependencies
    auto result = features.validateConfiguration();
    if (result.isError()) {
        std::cerr << "Invalid configuration: " << result.getError() << std::endl;
        return 1;
    }

    // Apply preset
    result = features.applyPreset("professional");
    if (result.isError()) {
        std::cerr << "Failed to apply preset: " << result.getError().getMessage() << std::endl;
    }

    return 0;
}
```

---

*This API reference provides the authoritative definition for all coreMusicPlayer APIs. Other documentation should reference these definitions rather than duplicate them.*